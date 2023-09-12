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
    std::vector<char*> argv;
    char* envp[18];
    std::map<std::string, std::string> env;

public:
    CgiHandler();
    ~CgiHandler();

    void generateProcess(const Request &request);
    void fillEnv(const Request &request);
    void convertEnv();
    void executeCgi(const Request &request);
    void sendCgiResult();
};

#endif
