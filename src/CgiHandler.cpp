#include "../inc/CgiHandler.hpp"

CgiHandler::CgiHandler() {}

CgiHandler::~CgiHandler() {}

void CgiHandler::generateProcess(const Request &request)
{
    int fd[2];

    if (pipe(fd) < 0)
        throw std::runtime_error("pipe failed");

    pid_t pid = fork();
    if (pid == -1)
        throw std::runtime_error("fork failed");

    std::string cgi_path = "./html" + request.getPath();

    // 유효하지 않은 path는 404 던지기

    argv.push_back(const_cast<char*>(cgi_path.c_str()));
    argv.push_back(0);
    if (pid == 0)
    {
        close(fd[1]);
        dup2(fd[0], STDOUT_FILENO);
        fillEnv(request);
        convertEnv();
        execve(argv[0], &argv[0], envp);
    }
}

void CgiHandler::fillEnv(const Request &request)
{
    env["AUTH_TYPE"] = "";
    env["CONTENT_LENGTH"] = "";
    env["CONTENT_TYPE"] = "";
    env["GATEWAY_INTERFACE"] = "";
    env["PATH_INFO"] = "";
    env["PATH_TRANSLATED"] = "";
    env["QUERY_STRING"] = "";
    env["REMOTE_ADDR"] = "";
    env["REMOTE_HOST"] = "";
    env["REMOTE_IDENT"] = "";
    env["REMOTE_USER"] = "";
    env["REQUEST_METHOD"] = request.getHttpMethodString();
    env["REQUEST_URI"] = "";
    env["SCRIPT_FILENAME"] = "";
    env["SCRIPT_NAME"] = "";
    env["SERVER_NAME"] = "";
    env["SERVER_PROTOCOL"] = "";
    env["SERVER_SOFTWARE"] = "";
}

void CgiHandler::convertEnv()
{
    int idx = 0;
    for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); it++)
    {
        std::string concat = it->first + "=" + it->second;
        envp[idx] = const_cast<char*>(concat.c_str());
        idx++;
    }
}

void CgiHandler::executeCgi(const Request &request)
{
    try
    {
        generateProcess(request);
        sendCgiResult();
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return;
    }
}

void CgiHandler::sendCgiResult()
{

}