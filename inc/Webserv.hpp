#ifndef WEBSERV_HPP
#define WEBSERV_HPP

// Configuration
#define LISTEN_BACKLOG_NUM 5 // listen 에서 대기 큐 갯수.
#define ERROR_PAGE_404_PATH "./templates/404error.html"
#define ERROR_PAGE_413_PATH "./templates/413error.html"
#define AUTO_INDEX_HTML_PATH "./templates/autoindex.html"

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

#include "Worker.hpp"

class Worker;

class Webserv
{
private:
	int kque;
	std::vector<Worker> workers;
	std::map<int, size_t> socket_to_worker;

	void handleNewConnection(int sockFd, size_t worker_idx);
	void handleExistingConnection(int fd, struct kevent event);

public:
	Webserv();
	~Webserv();

	void addWorker(const Worker &worker);
	void confFileParse(std::string conf_file);
	void init();
	void run();

};

#endif
