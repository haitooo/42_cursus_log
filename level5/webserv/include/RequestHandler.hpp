#ifndef REQUEST_HANDLER_HPP
# define REQUEST_HANDLER_HPP

# include <string>
# include <vector>

# include "ICgiExecutor.hpp"
# include "IConfig.hpp"
# include "IRequestHandler.hpp"
# include "SessionStore.hpp"

# define SESSION_COOKIE_NAME "webserv_sid"
# define SESSION_ROUTE "/session"

# define UPLOAD_TOKEN_BYTES 12

class RequestHandler : public IRequestHandler
{
	public:
		RequestHandler(const IConfig &config, ICgiExecutor &cgiExecutor, SessionStore &sessionStore);
		~RequestHandler();

		HandleResult	handle(ServerId id, const HttpRequest &request);
		void			finishCgi(ServerId id, const HttpRequest &request,
							bool ok, int errorStatus,
							HttpResponse &response);

		// Turns an error settled during parsing (400/501/505/413 ...) into a
		// response without resolving a location. Called when RequestParser
		// returns PARSE_ERROR.
		HttpResponse	makeError(ServerId id, int statusCode);
		bool			isCgiRoute(ServerId id, const HttpRequest &request);

	private:
		// The request-target split apart. `path` is already percent-decoded and
		// normalised: no `.` or `..` segments remain.
		struct RequestTarget
		{
			std::string	path;
			std::string	query;
		};

		typedef struct MultipartPartStruct {
			std::string name;
			std::string filename;
			bool isFile;
			std::string body;
		} MultipartPart;

		const IConfig	&_config;
		ICgiExecutor	&_cgi;
		SessionStore	&_sessionStore;
		RequestHandler(const RequestHandler &other);
		RequestHandler	&operator=(const RequestHandler &other);

		HandleResult	route(ServerId id, const HttpRequest &req);
		HttpResponse	handleGet(ServerId id, const RequestTarget &target,
							const ResolvedLocation &loc);
		HttpResponse	handlePost(ServerId id, const HttpRequest &req,
							const RequestTarget &target,
							const ResolvedLocation &loc);
		HttpResponse	handleMultipartForm(ServerId id, const HttpRequest &req,
							const RequestTarget &target,
							const ResolvedLocation &loc);
		static bool	extractBoundary(const HttpRequest &req,
							std::string &boundary);
		static bool	parseMultipartHeaders(const std::string &headersStr,
							MultipartPart &part);
		static void	resetMultipartPart(MultipartPart &part);
		HttpResponse	handleDelete(ServerId id, const RequestTarget &target,
							const ResolvedLocation &loc);
		HandleResult	handleCgi(ServerId id, const HttpRequest &req,
							const RequestTarget &target,
							const ResolvedLocation &loc);
		HttpResponse	handleRedirect(const ResolvedLocation &loc);
		HttpResponse	makeErrorResponse(ServerId id, int statusCode,
							const ResolvedLocation &loc);
		HttpResponse	makeAutoindexResponse(ServerId id,
							const RequestTarget &target,
							const ResolvedLocation &loc);
		HttpResponse	handleSession(ServerId id, const HttpRequest &req,
							const ResolvedLocation &loc);

		static bool			splitTarget(const std::string &requestTarget,
								RequestTarget &out);
		static bool			decodePercent(const std::string &in,
								std::string &out);
		static bool			normalizePath(const std::string &in,
								std::string &out);
		static std::size_t	scriptEndOf(const std::string &path,
								const std::string &extension);
		static std::string	fileExtensionOf(const std::string &path);
		static std::string	joinPath(const std::string &dir,
								const std::string &name);
		static std::string	escapeHtml(const std::string &raw);
		static std::string	reasonPhraseOf(int statusCode);
		static std::string	contentTypeOf(const std::string &filePath);
		static std::string	toString(std::size_t value);
		static HttpResponse	newResponse();
		static void			toLower(std::string &s);
		static bool	cookieValue(const HttpRequest &req, const std::string &name,
				std::string &out);
		static bool	isValidSessionId(const std::string &id);
};

#endif
