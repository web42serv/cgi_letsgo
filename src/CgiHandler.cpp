#include "../inc/CgiHandler.hpp"

CgiHandler::CgiHandler() {}

CgiHandler::~CgiHandler() {}

std::string CgiHandler::generateProcess(const Request &request)
{
  int fd[2];
  

  this->cgi_path = "./html" + request.getPath();

  int query_index = this->cgi_path.find("?", 0);
  std::string query_string = this->cgi_path.substr(query_index + 1, this->cgi_path.size() - query_index);
  this->cgi_path = this->cgi_path.substr(0, query_index);

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
    fillEnv(request, query_string);
    execve(this->cgi_path.c_str(), NULL, &(envp[0]));
    throw std::runtime_error("cgi execute failed");
  }
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		char buff[1024];
    for(int i=0; i<1024; i++) buff[i] = '\0';
		read(fd[0], &buff, 1024);
    std::string body(buff);
		std::cout << buff << std::endl;
		dup2(STDIN_FILENO, read_fd);
		close(fd[0]);
      return body;
	}
  return "generate process";
}

void CgiHandler::fillEnv(const Request &request, std::string query_string)
{
	env["CONTENT_LENGTH"] = "1000";
	env["CONTENT_TYPE"] = "multipart/form-data";
	env["PATH_INFO"] = this->cgi_path;
	env["QUERY_STRING"] = query_string;
	env["REQUEST_METHOD"] = request.getHttpMethodString();
	env["SERVER_PROTOCOL"] = "HTTP/1.1";

  convertEnv();
}

void CgiHandler::convertEnv()
{
  for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); it++)
  {
    std::string concat = it->first + "=" + it->second;
	char *str = new char[concat.length() + 1];
	strcpy(str, concat.c_str());
    envp.push_back(str);
  }
 	envp.push_back(0);
	// 끝나고 envp 돌면서 delete 해주기 (+ 구조 바꿀 생각도...)
}

std::string CgiHandler::executeCgi(const Request &request)
{
  try
  {
    return generateProcess(request);
    //sendCgiResult();
  }
  catch (std::runtime_error &e)
  {
    std::cout << e.what() << std::endl;
    return "execute cgi error";
  }
}

// void CgiHandler::sendCgiResult()
// {

// }