#ifndef WORKER_HPP
#define WORKER_HPP

#define PORT_NUM_MAX 65535

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <sstream>

#include "Webserv.hpp"
#include "Location.hpp"
#include "Response.hpp"
#include "Request.hpp"

class Location;

class Worker
{
private:
	int server_socket;
	int port;
	std::string root;
	std::string index_page;
	std::vector<std::string> server_names;
	std::map<int, std::string> error_pages;
	size_t client_max_body_size;
	std::vector<Location> locations;

    bool is_connected(int fd, int kque, struct kevent event);
    void terminateConnection(int fd, int kque, struct kevent event);

public:
	Worker();
	~Worker();
    void run();
    void handle_request(int fd, int kque, struct kevent event);
	void set_server_socket(int server_socket);
	int get_server_socket();
	void set_port(int port);
	int get_port();
	void set_root(std::string root);
	const std::string &get_root() const;
	void set_index_page(std::string &index_page);
	const std::string &get_index_page() const;
	void add_server_name(std::string &server_name);
	const std::vector<std::string> &get_server_names() const;
	void set_client_max_body_size(size_t size);
	size_t get_client_max_body_size() const;
	void	add_locations(const Location& location);
	const	std::vector<Location>& get_locations() const;
	void add_error_page(int error_code, std::string &error_page);
	const std::map<int, std::string> &get_error_page() const;
};

std::vector<std::string>::iterator set_worker(Worker& worker, std::vector<std::string> lines, std::vector<std::string>::iterator& lineIt);
Worker set_worker_info(std::vector<std::string>& lines);

#endif
