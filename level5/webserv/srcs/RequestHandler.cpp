#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <fstream>
#include <sstream>

#include "HttpStatus.hpp"
#include "Random.hpp"
#include "RequestHandler.hpp"

RequestHandler::RequestHandler(const IConfig &config, ICgiExecutor &cgiExecutor, SessionStore &sessionStore)
	: _config(config),
	  _cgi(cgiExecutor),
	  _sessionStore(sessionStore)
{
}

RequestHandler::~RequestHandler()
{
}

/*
** Every handler starts from its own response value.
**
** An earlier revision kept this on a RequestHandler member. Server owns one
** RequestHandler for every connection, so headers set while serving one
** request (Allow, Location, Content-Type) survived into the next response --
** and makeError() is called by Server without going through handle(), so
** nothing reset it. Returning a fresh value keeps requests independent.
**
** Connection is decided by Server, which owns the socket lifetime; a handler
** cannot know whether the connection is about to be closed.
*/
static HandleResult	asResult(const HttpResponse &response)
{
	HandleResult	result;

	result.response = response;
	return (result);
}

HttpResponse	RequestHandler::newResponse()
{
	HttpResponse	response;

	response.version = HTTP_VERSION_1_1;
	return (response);
}

/*
** HEAD builds the same response as GET and then drops only the body.
**
** Content-Length must keep the value GET would send (RFC 7231 4.3.2), so the
** body is dropped here, after route() returns and every handler has already
** set it. Doing it per branch would miss autoindex, error pages or CGI.
*/
HandleResult	RequestHandler::handle(ServerId id, const HttpRequest &req)
{
	HandleResult	result = route(id, req);

	if (!result.pending && req.method == "HEAD")
		result.response.body.clear();
	return (result);
}

void	RequestHandler::finishCgi(ServerId id, const HttpRequest &req,
	bool ok, int errorStatus, HttpResponse &response)
{
	RequestTarget		target;
	ResolvedLocation	loc;

	/*
	** Both failure paths replace the response wholesale. The CGI body is
	** dropped first: assigning over it would keep its capacity, so an error
	** page would carry a hundred megabytes of dead buffer with it.
	*/
	if (!splitTarget(req.path, target)
		|| _config.resolveLocation(id, target.path, loc) == false)
	{
		std::string().swap(response.body);
		response = makeError(id, ok ? STATUS_INTERNAL_SERVER_ERROR : errorStatus);
		return ;
	}
	if (!ok)
	{
		std::string().swap(response.body);
		response = makeErrorResponse(id, errorStatus, loc);
		return ;
	}

	response.version = HTTP_VERSION_1_1;
	if (response.reasonPhrase.empty())
		response.reasonPhrase = reasonPhraseOf(response.statusCode);
	if (req.method == "HEAD")
		std::string().swap(response.body);
}

HandleResult	RequestHandler::route(ServerId id, const HttpRequest &req)
{
	RequestTarget	target;

	if (!splitTarget(req.path, target))
		return (asResult(makeError(id, STATUS_BAD_REQUEST)));

	ResolvedLocation	loc;

	if (_config.resolveLocation(id, target.path, loc) == false)
		return (asResult(makeError(id, STATUS_NOT_FOUND)));

	if (loc.isMethodAllowed(req.method) == false)
		return (asResult(makeErrorResponse(id, STATUS_METHOD_NOT_ALLOWED, loc)));

	if (loc.clientMaxBodySize < req.body.size())
		return (asResult(makeErrorResponse(id, STATUS_PAYLOAD_TOO_LARGE, loc)));

	if (loc.redirect.enabled)
		return (asResult(handleRedirect(loc)));
	
	if (loc.locationPath == SESSION_ROUTE)
		return (asResult(handleSession(id, req, loc)));

	if (loc.cgi.enabled
		&& scriptEndOf(target.path, loc.cgi.extension) != std::string::npos)
		return (handleCgi(id, req, target, loc));

	if (req.method == "GET" || req.method == "HEAD")
		return (asResult(handleGet(id, target, loc)));
	if (req.method == "POST")
		return (asResult(handlePost(id, req, target, loc)));
	if (req.method == "DELETE")
		return (asResult(handleDelete(id, target, loc)));

	return (asResult(makeErrorResponse(id, STATUS_NOT_IMPLEMENTED, loc)));
}

bool	RequestHandler::splitTarget(const std::string &requestTarget,
	RequestTarget &out)
{
	const std::size_t	queryPos = requestTarget.find('?');
	std::string			rawPath = requestTarget.substr(0, queryPos);

	out.query.clear();
	if (queryPos != std::string::npos)
		out.query = requestTarget.substr(queryPos + 1);

	std::string	decoded;

	if (!decodePercent(rawPath, decoded))
		return (false);
	if (decoded.empty() || decoded[0] != '/')
		return (false);
	return (normalizePath(decoded, out.path));
}

bool	RequestHandler::decodePercent(const std::string &in, std::string &out)
{
	out.clear();
	for (std::size_t i = 0; i < in.size(); ++i)
	{
		if (in[i] != '%')
		{
			out += in[i];
			continue ;
		}
		if (i + 2 >= in.size())
			return (false);

		int	value = 0;

		for (std::size_t k = 1; k <= 2; ++k)
		{
			const char	c = in[i + k];

			value *= 16;
			if (c >= '0' && c <= '9')
				value += c - '0';
			else if (c >= 'a' && c <= 'f')
				value += c - 'a' + 10;
			else if (c >= 'A' && c <= 'F')
				value += c - 'A' + 10;
			else
				return (false);
		}
		if (value == 0)
			return (false);
		out += static_cast<char>(value);
		i += 2;
	}
	return (true);
}

bool	RequestHandler::normalizePath(const std::string &in, std::string &out)
{
	std::vector<std::string>	segments;
	std::size_t					i = 0;

	while (i < in.size())
	{
		std::size_t	end = in.find('/', i);

		if (end == std::string::npos)
			end = in.size();

		const std::string	segment = in.substr(i, end - i);

		if (segment == "..")
		{
			if (segments.empty())
				return (false);
			segments.pop_back();
		}
		else if (!segment.empty() && segment != ".")
			segments.push_back(segment);
		i = end + 1;
	}

	out = "/";
	for (std::size_t k = 0; k < segments.size(); ++k)
	{
		out += segments[k];
		if (k + 1 < segments.size())
			out += "/";
	}
	// A trailing '/' means "directory", so keep it.
	if (in.size() > 1 && in[in.size() - 1] == '/'
		&& out[out.size() - 1] != '/')
		out += "/";
	return (true);
}

HttpResponse	RequestHandler::handleGet(ServerId id,
	const RequestTarget &target, const ResolvedLocation &loc)
{
	HttpResponse	response = newResponse();

	std::string	filePath = _config.mapUriToPath(loc, target.path);
	struct stat	st;

	if (stat(filePath.c_str(), &st) == -1)
		return (makeErrorResponse(id, STATUS_NOT_FOUND, loc));

	if (S_ISDIR(st.st_mode))
	{
		bool	hasIndexFile = false;

		if (loc.index.empty() == false)
		{
			const std::string	indexPath = joinPath(filePath, loc.index);

			if (stat(indexPath.c_str(), &st) == 0 && S_ISREG(st.st_mode))
			{
				filePath = indexPath;
				hasIndexFile = true;
			}
		}
		if (hasIndexFile == false)
		{
			if (loc.autoindex)
				return (makeAutoindexResponse(id, target, loc));
			/*
			** Which of the two errors this is depends on whether an index was
			** asked for at all. A location that names one is promising a file
			** at this URI, so the file being absent is 404 -- nothing is
			** being withheld. A location that names none is refusing to list
			** the directory, which is 403, as nginx answers.
			**
			** The distinction is not cosmetic. The tester requests
			** /directory/Yeah under a location carrying
			** `index youpi.bad_extension`, expects 404, and treats a 403 as
			** fatal: the run stops there and every check after it goes
			** unmeasured.
			*/
			if (loc.index.empty() == false)
				return (makeErrorResponse(id, STATUS_NOT_FOUND, loc));
			return (makeErrorResponse(id, STATUS_FORBIDDEN, loc));
		}
	}
	else if (S_ISREG(st.st_mode) == false)
		return (makeErrorResponse(id, STATUS_FORBIDDEN, loc));

	if (access(filePath.c_str(), R_OK) == -1)
		return (makeErrorResponse(id, STATUS_FORBIDDEN, loc));

	std::ifstream	file(filePath.c_str(), std::ios::binary);

	if (file.is_open() == false)
		return (makeErrorResponse(id, STATUS_INTERNAL_SERVER_ERROR, loc));

	std::stringstream	buffer;

	buffer << file.rdbuf();

	response.statusCode = STATUS_OK;
	response.reasonPhrase = REASON_OK;
	response.body = buffer.str();
	response.headers[FIELD_CONTENT_TYPE] = contentTypeOf(filePath);
	response.headers[FIELD_CONTENT_LENGTH] = toString(response.body.size());
	return (response);
}

HttpResponse	RequestHandler::handlePost(ServerId id, const HttpRequest &req,
	const RequestTarget &target, const ResolvedLocation &loc)
{
	HttpResponse	response = newResponse();

	if (loc.uploadStore.empty())
		return (makeErrorResponse(id, STATUS_FORBIDDEN, loc));
	
	std::map<std::string, std::string>::const_iterator contentTypeIt
			= req.headers.find("content-type");
	if(contentTypeIt != req.headers.end() &&
		contentTypeIt->second.find("multipart/form-data") != std::string::npos)
		return handleMultipartForm(id, req, target, loc);

	const std::string	suffix = randomHexToken(UPLOAD_TOKEN_BYTES);

	if (suffix.empty())
		return (makeErrorResponse(id, STATUS_INTERNAL_SERVER_ERROR, loc));

	const std::string	fileName = "upload_" + suffix;
	const std::string	filePath = joinPath(loc.uploadStore, fileName);

	std::ofstream	file(filePath.c_str(), std::ios::binary);

	if (file.is_open() == false)
		return (makeErrorResponse(id, STATUS_INTERNAL_SERVER_ERROR, loc));
	file.write(req.body.c_str(), static_cast<std::streamsize>(req.body.size()));
	file.close();
	if (file.fail())
		return (makeErrorResponse(id, STATUS_INTERNAL_SERVER_ERROR, loc));

	response.statusCode = STATUS_CREATED;
	response.reasonPhrase = REASON_CREATED;
	response.headers[FIELD_LOCATION] = joinPath(target.path, fileName);
	response.headers[FIELD_CONTENT_LENGTH] = "0";
	return (response);
}

#define boundaryChars "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'()+_,-./:=?"

bool	RequestHandler::extractBoundary(const HttpRequest &req, std::string &boundary)
{
	const std::string maltipartStr = CONTENT_TYPE_MULTIPART_FORM_DATA;
	const std::string boundaryStr = "boundary=";

	std::string contentType = req.headers.at("content-type");
	std::size_t multipartPos = contentType.find(maltipartStr);
	if(multipartPos != 0)
		return false;
	if(contentType.size() < maltipartStr.size() + 1 + boundaryStr.size() + 1)
		return false;
	if(contentType[maltipartStr.size()] != ';')
		return false;

	std::size_t boundaryPos = contentType.find("boundary=");
	if (boundaryPos == std::string::npos)
		return false;
	if(contentType.find_first_not_of(" \t", maltipartStr.size() + 1) != boundaryPos)
		return false;
	if(boundaryPos + boundaryStr.size() >= contentType.size())
		return false;

	boundary = contentType.substr(boundaryPos + boundaryStr.size());
	if(boundary.empty())
		return false;
	if(boundary.size() > 70)
		return false;
	if(boundary.find_first_not_of(boundaryChars) != std::string::npos)
		return false;
	return true;
}

void	RequestHandler::resetMultipartPart(MultipartPart &part)
{
	part.name.clear();
	part.filename.clear();
	part.isFile = false;
	part.body.clear();
}

bool	RequestHandler::parseMultipartHeaders(const std::string &headersStr, MultipartPart &part)
{
	std::map<std::string, std::string> headers;
	std::size_t headerPos = 0;

	while (headerPos < headersStr.size())
	{
		std::size_t lineEndPos = headersStr.find("\r\n", headerPos);
		if (lineEndPos == std::string::npos)
			lineEndPos = headersStr.size();
		std::string headerLine = headersStr.substr(headerPos, lineEndPos - headerPos);
		if(lineEndPos == headersStr.size())
			headerPos = lineEndPos;
		else
			headerPos = lineEndPos + 2;
		std::size_t colonPos = headerLine.find(':');
		if (colonPos == std::string::npos)
			return false;
		std::string headerName = headerLine.substr(0, colonPos);
		std::string headerValue = headerLine.substr(colonPos + 1);
		toLower(headerName);
		headers[headerName] = headerValue;
	}

	std::map<std::string, std::string>::const_iterator contentDispositionIt
		= headers.find("content-disposition");
	if(contentDispositionIt == headers.end())
		return false;
	std::string contentDisposition = contentDispositionIt->second;
	std::vector<std::string> tokens;
	std::size_t start = 0;
	std::size_t semicolonPos = contentDisposition.find(';');
	while (semicolonPos != std::string::npos)
	{
		std::string token = contentDisposition.substr(start, semicolonPos - start);
		std::size_t firstNonSpace = token.find_first_not_of(" \t");
		std::size_t lastNonSpace = token.find_last_not_of(" \t");
		if (firstNonSpace != std::string::npos && lastNonSpace != std::string::npos)
			tokens.push_back(token.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1));
		start = semicolonPos + 1;
		semicolonPos = contentDisposition.find(';', start);
	}
	std::string lastToken = contentDisposition.substr(start);
	std::size_t firstNonSpace = lastToken.find_first_not_of(" \t");
	std::size_t lastNonSpace = lastToken.find_last_not_of(" \t");
	if (firstNonSpace != std::string::npos && lastNonSpace != std::string::npos)
		tokens.push_back(lastToken.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1));
	
	if(tokens.empty() || tokens[0] != "form-data")
		return false;

	for(std::size_t i = 1; i < tokens.size(); ++i)
	{
		std::string token = tokens[i];
		std::size_t equalPos = token.find('=');
		if(equalPos == std::string::npos)
			continue;
		std::string key = token.substr(0, equalPos);
		std::string value = token.substr(equalPos + 1);
		if(value.size() < 2 || value[0] != '"' || value[value.size() - 1] != '"')
			continue;
		value = value.substr(1, value.size() - 2);
		if(key == "name")
			part.name = value;
		else if(key == "filename")
			part.filename = value;
	}
	
	// ? is it error if name or filename is empty? or just ignore it?
	if(part.name.empty())
		return false;
	if(part.filename.find("/") != std::string::npos)
		return false;
	if(part.filename.empty() == false)
		part.isFile = true;

	return true;
}

HttpResponse	RequestHandler::handleMultipartForm(ServerId id, const HttpRequest &req,
	const RequestTarget &target, const ResolvedLocation &loc)
{
	HttpResponse	response = newResponse();

	std::string boundary;
	if(extractBoundary(req, boundary) == false)
		return (makeErrorResponse(id, STATUS_BAD_REQUEST, loc));

	const std::string dashBoundary = "--" + boundary;
	const std::string delimiter = "\r\n" + dashBoundary;
	const std::string closeDelimiter = delimiter + "--";
	
	if(req.body.find(dashBoundary) != 0)
		return (makeErrorResponse(id, STATUS_BAD_REQUEST, loc));
	std::size_t pos = dashBoundary.size();
	pos = req.body.find_first_not_of(" \t", pos);
	if(pos == std::string::npos)
		return (makeErrorResponse(id, STATUS_BAD_REQUEST, loc));
	if(req.body.find("\r\n", pos) != pos)
		return (makeErrorResponse(id, STATUS_BAD_REQUEST, loc));
	// pos += 2;

	std::size_t closePos = req.body.find(closeDelimiter, pos);
	if(closePos == std::string::npos)
		return (makeErrorResponse(id, STATUS_BAD_REQUEST, loc));
	std::size_t delimiterPos = req.body.find(delimiter, pos);

	while (delimiterPos != std::string::npos && delimiterPos <= closePos)
	{
		if(req.body.find("\r\n", pos) != pos)
			return (makeErrorResponse(id, STATUS_BAD_REQUEST, loc));
		pos += 2;
		std::string part = req.body.substr(pos, delimiterPos - pos);
		pos = delimiterPos + delimiter.size();
		delimiterPos = req.body.find(delimiter, pos);
		
		std::size_t headerEndPos = part.find("\r\n\r\n");
		if (headerEndPos == std::string::npos)
			return (makeErrorResponse(id, STATUS_BAD_REQUEST, loc));
		std::string headersStr = part.substr(0, headerEndPos);
		std::string bodyStr = part.substr(headerEndPos + 4);
		
		// parse multipart headers
		MultipartPart multipartPart;
		resetMultipartPart(multipartPart);
		if(parseMultipartHeaders(headersStr, multipartPart) == false)
			return (makeErrorResponse(id, STATUS_BAD_REQUEST, loc));
		if(multipartPart.isFile == false)
			continue;

		if(access(joinPath(loc.uploadStore, multipartPart.filename).c_str(), F_OK) == 0)
			multipartPart.filename = randomHexToken(UPLOAD_TOKEN_BYTES)
				+ "_" + multipartPart.filename;
		const std::string	filePath = joinPath(loc.uploadStore, multipartPart.filename);
		std::ofstream	file(filePath.c_str(), std::ios::binary);
		if (file.is_open() == false)
			return (makeErrorResponse(id, STATUS_INTERNAL_SERVER_ERROR, loc));
		file.write(bodyStr.data(), static_cast<std::streamsize>(bodyStr.size()));
		file.close();
		if (file.fail())
			return (makeErrorResponse(id, STATUS_INTERNAL_SERVER_ERROR, loc));
	}
	
	response.statusCode = STATUS_CREATED;
	response.reasonPhrase = REASON_CREATED;
	response.headers[FIELD_LOCATION] = target.path; // Return the directory path
	response.headers[FIELD_CONTENT_LENGTH] = "0";
	return (response);
}

HttpResponse	RequestHandler::handleDelete(ServerId id,
	const RequestTarget &target, const ResolvedLocation &loc)
{
	HttpResponse	response = newResponse();

	const std::string	filePath = _config.mapUriToPath(loc, target.path);
	struct stat			st;

	if (stat(filePath.c_str(), &st) == -1)
		return (makeErrorResponse(id, STATUS_NOT_FOUND, loc));
	if (S_ISREG(st.st_mode) == false)
		return (makeErrorResponse(id, STATUS_FORBIDDEN, loc));
	if (access(filePath.c_str(), W_OK) == -1)
		return (makeErrorResponse(id, STATUS_FORBIDDEN, loc));
	if (std::remove(filePath.c_str()) != 0)
		return (makeErrorResponse(id, STATUS_INTERNAL_SERVER_ERROR, loc));

	response.statusCode = STATUS_NO_CONTENT;
	response.reasonPhrase = REASON_NO_CONTENT;
	return (response);
}

HttpResponse	RequestHandler::handleRedirect(const ResolvedLocation &loc)
{
	HttpResponse	response = newResponse();

	response.statusCode = loc.redirect.statusCode;
	response.reasonPhrase = reasonPhraseOf(loc.redirect.statusCode);
	response.headers[FIELD_LOCATION] = loc.redirect.target;
	response.headers[FIELD_CONTENT_LENGTH] = "0";
	return (response);
}

HandleResult	RequestHandler::handleCgi(ServerId id, const HttpRequest &req,
	const RequestTarget &target, const ResolvedLocation &loc)
{
	const std::size_t	scriptEnd = scriptEndOf(target.path, loc.cgi.extension);
	HandleResult		result;

	result.cgi.scriptPath = _config.mapUriToPath(loc, target.path.substr(0, scriptEnd));

	if (result.cgi.scriptPath.empty())
		return (asResult(makeErrorResponse(id, STATUS_NOT_FOUND, loc)));

	/*
	** A script that is not there is deliberately not rejected: the interpreter
	** is the program that runs, and it answers whatever it is handed, which is
	** what the subject's POST to the non-existent youpla.bla expects. Only a
	** path that exists and cannot be used as a script is refused.
	**
	** Executability is likewise not required. The child runs
	**     execve(interpreterPath, {interpreterPath, scriptPath})
	** so the script arrives as an argument, never as the program; the
	** subject's YoupiBanane fixture is plain files the evaluator creates by
	** hand and never chmods. R_OK is the permission that has to hold.
	*/
	struct stat	st;

	if (stat(result.cgi.scriptPath.c_str(), &st) == 0)
	{
		if (S_ISREG(st.st_mode) == false)
			return (asResult(makeErrorResponse(id, STATUS_FORBIDDEN, loc)));
		if (access(result.cgi.scriptPath.c_str(), R_OK) == -1)
			return (asResult(makeErrorResponse(id, STATUS_FORBIDDEN, loc)));
	}
	if (access(loc.cgi.interpreterPath.c_str(), X_OK) == -1)
		return (asResult(makeErrorResponse(id, STATUS_INTERNAL_SERVER_ERROR, loc)));

	result.cgi.interpreterPath = loc.cgi.interpreterPath;
	result.cgi.queryString = target.query;
	result.cgi.pathInfo = target.path.substr(scriptEnd);
	result.cgi.serverHost = _config.listenAddress(id).host;
	result.cgi.serverPort = _config.listenAddress(id).port;
	result.pending = true;
	(void)req;
	return (result);
}

bool	RequestHandler::isCgiRoute(ServerId id, const HttpRequest &req)
{
	RequestTarget		target;
	ResolvedLocation	loc;

	if (!splitTarget(req.path, target))
		return (false);
	if (_config.resolveLocation(id, target.path, loc) == false)
		return (false);
	if (loc.isMethodAllowed(req.method) == false)
		return (false);
	if (loc.redirect.enabled)
		return (false);
	return (loc.cgi.enabled
		&& scriptEndOf(target.path, loc.cgi.extension) != std::string::npos);
}

HttpResponse	RequestHandler::makeError(ServerId id, int statusCode)
{
	const ResolvedLocation	empty;

	return (makeErrorResponse(id, statusCode, empty));
}

HttpResponse	RequestHandler::makeErrorResponse(ServerId id, int statusCode,
	const ResolvedLocation &loc)
{
	HttpResponse	response = newResponse();

	response.statusCode = statusCode;
	response.reasonPhrase = reasonPhraseOf(statusCode);

	if (statusCode == STATUS_METHOD_NOT_ALLOWED)
	{
		std::string	allow;

		for (std::size_t i = 0; i < loc.methods.size(); ++i)
		{
			if (i != 0)
				allow += ", ";
			allow += loc.methods[i];
		}
		response.headers[FIELD_ALLOW] = allow;
	}

	std::string	errorPagePath;

	if (_config.errorPagePath(id, statusCode, errorPagePath))
	{
		std::ifstream	errorPageFile(errorPagePath.c_str(), std::ios::binary);

		if (errorPageFile.is_open())
		{
			std::stringstream	buffer;

			buffer << errorPageFile.rdbuf();
			response.body = buffer.str();
			response.headers[FIELD_CONTENT_TYPE] = CONTENT_TYPE_HTML;
			response.headers[FIELD_CONTENT_LENGTH]
				= toString(response.body.size());
			return (response);
		}
	}

	std::stringstream	body;

	body << "<html><head><title>" << statusCode << " "
		 << response.reasonPhrase << "</title></head>"
		 << "<body><h1>" << statusCode << " " << response.reasonPhrase
		 << "</h1></body></html>";
	response.body = body.str();
	response.headers[FIELD_CONTENT_TYPE] = CONTENT_TYPE_HTML;
	response.headers[FIELD_CONTENT_LENGTH] = toString(response.body.size());
	return (response);
}

HttpResponse	RequestHandler::makeAutoindexResponse(ServerId id,
	const RequestTarget &target, const ResolvedLocation &loc)
{
	HttpResponse	response = newResponse();

	const std::string	dirPath = _config.mapUriToPath(loc, target.path);
	DIR					*dir = opendir(dirPath.c_str());

	if (dir == NULL)
		return (makeErrorResponse(id, STATUS_FORBIDDEN, loc));

	std::string	base = target.path;

	if (base.empty() || base[base.size() - 1] != '/')
		base += "/";

	std::stringstream	body;

	body << "<html><head><title>Index of " << escapeHtml(target.path)
		 << "</title></head><body><h1>Index of " << escapeHtml(target.path)
		 << "</h1><hr><pre>" << std::endl
		 << "<a href=\"../\">../</a>" << std::endl;

	struct dirent	*entry = readdir(dir);

	while (entry != NULL)
	{
		std::string	name = entry->d_name;

		if (name == ".")
		{
			entry = readdir(dir);
			continue ;
		}

		std::string	href;

		if (name == "..")
		{
			entry = readdir(dir);
			continue ;
		}
		else
		{
			struct stat	st;

			if (stat(joinPath(dirPath, name).c_str(), &st) == 0
				&& S_ISDIR(st.st_mode))
				name += "/";
			href = base + name;
		}

		// File names are attacker controlled, so never embed them raw.
		body << "<a href=\"" << escapeHtml(href) << "\">"
			 << escapeHtml(name) << "</a>";
		body << std::string(name.size() < 20 ? 20 - name.size() : 1, ' ');
		body << std::endl;
		entry = readdir(dir);
	}
	if (closedir(dir) == -1)
		return (makeErrorResponse(id, STATUS_INTERNAL_SERVER_ERROR, loc));
	body << "</body></html>";

	response.statusCode = STATUS_OK;
	response.reasonPhrase = REASON_OK;
	response.body = body.str();
	response.headers[FIELD_CONTENT_TYPE] = CONTENT_TYPE_HTML;
	response.headers[FIELD_CONTENT_LENGTH] = toString(response.body.size());
	return (response);
}

/*
** End offset of the first path segment carrying the CGI extension, or npos.
**
** The offset is taken while scanning rather than by looking the segment up
** again with find(), which would return the wrong position when an earlier
** segment contains it as a substring (`/xx.py.txt/x.py`).
*/
std::size_t	RequestHandler::scriptEndOf(const std::string &path,
	const std::string &extension)
{
	if (extension.empty())
		return (std::string::npos);

	std::size_t	i = 0;

	while (i < path.size())
	{
		std::size_t	end = path.find('/', i);

		if (end == std::string::npos)
			end = path.size();
		if (end > i && fileExtensionOf(path.substr(i, end - i)) == extension)
			return (end);
		i = end + 1;
	}
	return (std::string::npos);
}

std::string	RequestHandler::fileExtensionOf(const std::string &path)
{
	const std::size_t	dotPos = path.find_last_of('.');

	if (dotPos == std::string::npos)
		return ("");
	return (path.substr(dotPos));
}

std::string	RequestHandler::joinPath(const std::string &dir,
	const std::string &name)
{
	if (dir.empty())
		return (name);
	if (dir[dir.size() - 1] == '/')
		return (dir + name);
	return (dir + "/" + name);
}

std::string	RequestHandler::escapeHtml(const std::string &raw)
{
	std::string	out;

	for (std::size_t i = 0; i < raw.size(); ++i)
	{
		if (raw[i] == '&')
			out += "&amp;";
		else if (raw[i] == '<')
			out += "&lt;";
		else if (raw[i] == '>')
			out += "&gt;";
		else if (raw[i] == '"')
			out += "&quot;";
		else
			out += raw[i];
	}
	return (out);
}

std::string	RequestHandler::toString(std::size_t value)
{
	std::stringstream	ss;

	ss << value;
	return (ss.str());
}

std::string	RequestHandler::reasonPhraseOf(int statusCode)
{
	switch (statusCode)
	{
		case STATUS_OK: return (REASON_OK);
		case STATUS_CREATED: return (REASON_CREATED);
		case STATUS_NO_CONTENT: return (REASON_NO_CONTENT);
		case STATUS_MOVED_PERMANENTLY: return (REASON_MOVED_PERMANENTLY);
		case STATUS_FOUND: return (REASON_FOUND);
		case STATUS_BAD_REQUEST: return (REASON_BAD_REQUEST);
		case STATUS_FORBIDDEN: return (REASON_FORBIDDEN);
		case STATUS_NOT_FOUND: return (REASON_NOT_FOUND);
		case STATUS_METHOD_NOT_ALLOWED: return (REASON_METHOD_NOT_ALLOWED);
		case STATUS_REQUEST_TIMEOUT: return (REASON_REQUEST_TIMEOUT);
		case STATUS_LENGTH_REQUIRED: return (REASON_LENGTH_REQUIRED);
		case STATUS_PAYLOAD_TOO_LARGE: return (REASON_PAYLOAD_TOO_LARGE);
		case STATUS_URI_TOO_LONG: return (REASON_URI_TOO_LONG);
		case STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE:
			return (REASON_REQUEST_HEADER_FIELDS_TOO_LARGE);
		case STATUS_NOT_IMPLEMENTED: return (REASON_NOT_IMPLEMENTED);
		case STATUS_BAD_GATEWAY: return (REASON_BAD_GATEWAY);
		case STATUS_GATEWAY_TIMEOUT: return (REASON_GATEWAY_TIMEOUT);
		case STATUS_HTTP_VERSION_NOT_SUPPORTED:
			return (REASON_HTTP_VERSION_NOT_SUPPORTED);
		default: return (REASON_INTERNAL_SERVER_ERROR);
	}
}

std::string	RequestHandler::contentTypeOf(const std::string &filePath)
{
	const std::string	extension = fileExtensionOf(filePath);

	if (extension == ".html" || extension == ".htm")
		return (CONTENT_TYPE_HTML);
	if (extension == ".txt")
		return (CONTENT_TYPE_PLAIN);
	if (extension == ".jpg" || extension == ".jpeg")
		return (CONTENT_TYPE_JPEG);
	if (extension == ".png")
		return (CONTENT_TYPE_PNG);
	if (extension == ".gif")
		return (CONTENT_TYPE_GIF);
	if (extension == ".css")
		return (CONTENT_TYPE_CSS);
	if (extension == ".js")
		return (CONTENT_TYPE_JAVASCRIPT);
	if (extension == ".json")
		return (CONTENT_TYPE_JSON);
	if (extension == ".xml")
		return (CONTENT_TYPE_XML);
	if (extension == ".svg")
		return (CONTENT_TYPE_SVG);
	if (extension == ".mp4")
		return (CONTENT_TYPE_MP4);
	if (extension == ".mpeg")
		return (CONTENT_TYPE_MPEG);
	if (extension == ".pdf")
		return (CONTENT_TYPE_PDF);
	if (extension == ".zip")
		return (CONTENT_TYPE_ZIP);
	if (extension == ".mp3")
		return (CONTENT_TYPE_MP3);
	if (extension == ".aac")
		return (CONTENT_TYPE_AAC);
	if (extension == ".csv")
		return (CONTENT_TYPE_TEXT_CSV);
	return (CONTENT_TYPE_OCTET);
}

void	RequestHandler::toLower(std::string &s)
{
	for (std::size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
			s[i] = static_cast<char>(s[i] - 'A' + 'a');
	}
}

HttpResponse RequestHandler::handleSession(ServerId id, const HttpRequest &req,
	const ResolvedLocation &loc)
{
	HttpResponse response = newResponse();
	time_t now = std::time(NULL);
	std::string sessionId;
	bool isNewSession = false;
	std::string body = "<html><head><title>Session</title></head><body>";

	if(cookieValue(req, SESSION_COOKIE_NAME, sessionId) && isValidSessionId(sessionId)
		&& _sessionStore.find(sessionId, now) != NULL)
	{
		_sessionStore.recordVisit(sessionId, now);
		body += "<h1>Existing Session</h1>";
		body += "<p>Session ID: " + sessionId + "</p>";
		body += "<p>Created At: " + toString(_sessionStore.find(sessionId, now)->createdAt) + "</p>";
		body += "<p>Expires At: " + toString(_sessionStore.find(sessionId, now)->expiresAt) + "</p>";
		body += "<p>Visit Count: " + toString(_sessionStore.find(sessionId, now)->visits) + "</p>";
	}
	else
	{
		isNewSession = true;
		std::string newSessionId = _sessionStore.create(now);
		if (newSessionId.empty())
			return makeErrorResponse(id, STATUS_INTERNAL_SERVER_ERROR, loc);
		body += "<h1>New Session Created</h1>";
		body += "<p>Session ID: " + newSessionId + "</p>";
		body += "<p>Created At: " + toString(_sessionStore.find(newSessionId, now)->createdAt) + "</p>";
		body += "<p>Expires At: " + toString(_sessionStore.find(newSessionId, now)->expiresAt) + "</p>";
		body += "<p>Visit Count: " + toString(_sessionStore.find(newSessionId, now)->visits) + "</p>";
		sessionId = newSessionId;
	}
	body += "</body></html>";

	response.statusCode = STATUS_OK;
	response.reasonPhrase = REASON_OK;
	response.headers[FIELD_CONTENT_TYPE] = CONTENT_TYPE_HTML;
	if (isNewSession)
	{
		std::stringstream setCookieValue;
		setCookieValue << SESSION_COOKIE_NAME << "=" << sessionId
			<< "; Path=/;" << " Max-Age=" << _sessionStore.ttlSeconds() << ";"
			<< " HttpOnly;"
			<< " SameSite=Lax";
		response.headers[FIELD_SET_COOKIE] = setCookieValue.str();
	}
	response.headers[FIELD_CONTENT_LENGTH] = toString(body.size());
	response.body = body;
	return (response);
}

bool RequestHandler::isValidSessionId(const std::string &sessionId)
{
	if (sessionId.size() != 24)
		return false;
	if(sessionId.find_first_not_of("0123456789abcdef") != std::string::npos)
		return false;
	return true;
}

bool RequestHandler::cookieValue(const HttpRequest &req, const std::string &name,
		std::string &out)
{
	std::map<std::string, std::string>::const_iterator it = req.headers.find("cookie");
	if (it == req.headers.end())
		return false;

	std::string cookies = it->second;
	std::size_t pos = 0;
	while (pos < cookies.size())
	{
		std::size_t eqPos = cookies.find('=', pos);
		if (eqPos == std::string::npos)
			break;
		std::string cookieName = cookies.substr(pos, eqPos - pos);
		std::size_t semicolonPos = cookies.find(';', eqPos);
		std::string cookieValue;
		if (semicolonPos == std::string::npos)
		{
			cookieValue = cookies.substr(eqPos + 1);
			pos = cookies.size();
		}
		else
		{
			cookieValue = cookies.substr(eqPos + 1, semicolonPos - eqPos - 1);
			pos = cookies.find_first_not_of(" \t", semicolonPos + 1);
		}
		cookieValue.erase(0, cookieValue.find_first_not_of(" \t"));
		cookieValue.erase(cookieValue.find_last_not_of(" \t") + 1);
		cookieName.erase(0, cookieName.find_first_not_of(" \t"));
		cookieName.erase(cookieName.find_last_not_of(" \t") + 1);
		if (cookieName == name)
		{
			out = cookieValue;
			return true;
		}
	}
	return false;
}
