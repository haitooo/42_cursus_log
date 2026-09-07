/*
** A compiled CGI. This binary is what cgi_path names, and the requested
** .bin file arrives as argv[1] -- the same shape the 42 cgi_tester uses.
**
** It proves the executor is not tied to script interpreters: the "script"
** here (greeting.bin) is plain data with no shebang and no execute bit,
** because CgiExecutor only requires R_OK on it.
*/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static std::string env(const char *name)
{
	const char *value = std::getenv(name);

	return (value ? std::string(value) : std::string());
}

// Reads the file the server handed us. The path is relative and resolves
// because the child chdir()s into the script's directory (subject IV.3).
static std::string readAll(const std::string &path, bool &ok)
{
	std::ifstream file(path.c_str(), std::ios::binary);

	if (!file.is_open())
	{
		ok = false;
		return (std::string());
	}

	std::ostringstream buffer;

	buffer << file.rdbuf();
	ok = true;
	return (buffer.str());
}

int main(int argc, char **argv)
{
	const std::string script = (argc > 1 ? argv[1] : "");
	bool ok = false;
	std::string payload = readAll(script, ok);

	if (!ok)
	{
		// A non-zero exit makes the server answer 502, which is what a CGI
		// that cannot produce its output should do.
		std::cerr << "hello_bin: cannot read " << script << std::endl;
		return (1);
	}

	std::string body;

	body += "hello from a compiled binary\n";
	body += "interpreter=cgi-bin/hello_bin argc=";
	{
		std::ostringstream oss;

		oss << argc;
		body += oss.str();
	}
	body += " argv1=" + script + "\n";
	body += "method=" + env("REQUEST_METHOD")
		+ " query=" + env("QUERY_STRING")
		+ " path_info=" + env("PATH_INFO") + "\n";
	body += "script_filename=" + env("SCRIPT_FILENAME") + "\n";
	body += "payload=" + payload;

	std::ostringstream head;

	head << "Content-Type: text/plain\r\n"
		 << "Content-Length: " << body.size() << "\r\n\r\n";
	std::cout << head.str() << body;
	std::cout.flush();
	return (0);
}
