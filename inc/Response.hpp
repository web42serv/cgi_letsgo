#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstring>
#include <fcntl.h>

#include "Transaction.hpp"
#include "Worker.hpp"
#include "Request.hpp"
#include "CgiHandler.hpp"

class Worker;

class Response : public Transaction
{
private:
	int statusCode;
	std::string getStatusMessage(int code);
  void readFileToBody(const std::string &filePath);
	void generateBody(Worker &worker, const Request &request);
  void generateBody_AutoIndexing(const std::string &path, const Request &request);
  int checkPath(std::string &filePath);
  std::vector<std::string> getFilesInDirectory(const std::string &dirPath);

public:
    Response();
    ~Response();

	void parsingFromRequest(Worker &worker, const Request &request);
	void send(int fd);
    void handleBodySizeLimit();
    void setBody(const std::string body);

    void handleGET(Worker &worker, const Request &request);
    void handlePOST(Worker &worker, const Request &request);
    void handlePUT(Worker &worker, const Request &request);
    void handleDELETE(Worker &worker, const Request &request);

};

#endif
