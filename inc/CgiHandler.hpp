#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unistd.h>

#include "Request.hpp"

class CgiHandler {
private:

	std::string cgi_path;
  std::vector<char*> envp;
  std::map<std::string, std::string> env;
	std::vector<char*> env_vec;

public:
  CgiHandler();
  ~CgiHandler();

  std::string generateProcess(const Request &request);
  void fillEnv(const Request &request);
  void convertEnv();
  std::string executeCgi(const Request &request);
  //void sendCgiResult();
};

#endif
