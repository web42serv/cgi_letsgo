#include "../inc/CgiHandler.hpp"

CgiHandler::CgiHandler() {}

CgiHandler::~CgiHandler() {}


std::string CgiHandler::generateProcess(const Request &request)
{
  int to_cgi[2];
  int from_cgi[2];
  
  this->cgi_path = "./html" + request.getPath();

  int query_index = this->cgi_path.find("?", 0);
  std::string query_string = this->cgi_path.substr(query_index + 1, this->cgi_path.size() - query_index);
  this->cgi_path = this->cgi_path.substr(0, query_index);

  if (access(this->cgi_path.c_str(), F_OK) != 0)
    throw std::runtime_error("404 not found");

  if (pipe(to_cgi) < 0 || pipe(from_cgi) < 0)
    throw std::runtime_error("pipe failed");

  pid_t pid = fork();
  if (pid == -1)
    throw std::runtime_error("fork failed");
  else if (pid == 0)
  {
    // Child process (CGI script)
    close(to_cgi[1]);
    close(from_cgi[0]);

    // Redirect stdin and stdout
    dup2(to_cgi[0], STDIN_FILENO);
    dup2(from_cgi[1], STDOUT_FILENO);

    // Close unused pipe ends
    close(to_cgi[0]);
    close(from_cgi[1]);

    fillEnv(request, query_string);

    if (request.getHttpMethodString() == "POST")
    {
      // Read POST data from parent process through stdin
      char buffer[1024];
      ssize_t bytesRead;
      while ((bytesRead = read(to_cgi[1], buffer, sizeof(buffer))) > 0)
      {
	  	std::cerr << to_cgi[0] << std::endl;
        // Process POST data as needed
		write(to_cgi[1], buffer, bytesRead);
      }
	  close(to_cgi[1]);
    }
    execve(this->cgi_path.c_str(), NULL, envp.data());
    throw std::runtime_error("cgi execute failed");
  }
  else
  {
    // Parent process (Server)
    close(to_cgi[0]);
    close(from_cgi[1]);

    // Send POST data to CGI script through to_cgi pipe
    if (request.getHttpMethodString() == "POST")
    {
      write(to_cgi[1], request.getBody().c_str(), request.getBody().length());
	  close(to_cgi[1]);
    }

    char buff[2048];
    ssize_t bytesRead;
    std::string body;

    // Read CGI script's output from from_cgi pipe
    while ((bytesRead = read(from_cgi[0], buff, sizeof(buff))) > 0)
    {
      body.append(buff, bytesRead);
    }
    close(from_cgi[0]);

    return body;
  }
  return "generate process";
}

void CgiHandler::fillEnv(const Request &request, std::string query_string)
{
	env["CONTENT_LENGTH"] = "1000";
	env["CONTENT_TYPE"] = request.getContentType();
	env["PATH_INFO"] = this->cgi_path;
	if (request.getHttpMethodString() == "GET")
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