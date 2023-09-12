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
    std::map<std::string, std::string> env;
	std::vector<char*> env_vec;

public:
    CgiHandler();
    ~CgiHandler();

    void generateProcess(const Request &request);
    void fillEnv(const Request &request);
    void executeCgi(const Request &request);
    //void sendCgiResult();
};

#endif
