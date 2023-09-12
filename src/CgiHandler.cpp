#include "../inc/CgiHandler.hpp"

CgiHandler::CgiHandler() {}

CgiHandler::~CgiHandler() {}

void CgiHandler::generateProcess(const Request &request)
{
    int fd[2];

    this->cgi_path = "./html" + request.getPath();

	std::cout << this->cgi_path.c_str() << std::endl;

	if (access(this->cgi_path.c_str(), F_OK) != 0)
		throw std::runtime_error("404 not found");

	if (pipe(fd) < 0)
			throw std::runtime_error("pipe failed");

	int read_fd = dup(0);

    pid_t pid = fork();
    if (pid == -1)
        throw std::runtime_error("fork failed");
    else if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        fillEnv(request);
        execve(this->cgi_path.c_str(), NULL, &env_vec[0]);
        throw std::runtime_error("cgi execute failed");
    }
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		char buff[128];
		read(fd[0], &buff, 128);
		std::cout << buff << std::endl;
		dup2(STDIN_FILENO, read_fd);
		close(fd[0]);
	}

}

void CgiHandler::fillEnv(const Request &request)
{
    env["CONTENT_LENGTH"] = "1000";
    env["CONTENT_TYPE"] = "text/plain";
    env["PATH_INFO"] = this->cgi_path;
    env["QUERY_STRING"] = "name=subcho";
    env["REQUEST_METHOD"] = request.getHttpMethodString();
	env["SERVER_PROTOCOL"] = "HTTP/1.1";

	std::map<std::string, std::string>::iterator it;
	for(it = this->env.begin(); it != this->env.end(); ++it)
	{
		std::string env_str = it->first + "=" + it->second;
		this->env_vec.push_back(const_cast<char*> (env_str.c_str()));
	}
	this->env_vec.push_back(0);
}

void CgiHandler::executeCgi(const Request &request)
{
    try
    {
        generateProcess(request);
        //sendCgiResult();
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return;
    }
}

// void CgiHandler::sendCgiResult()
// {

// }