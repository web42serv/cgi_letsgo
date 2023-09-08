#ifndef LOCATION_HPP
#define LOCATION_HPP

#define METHOD_GET 0
#define METHOD_POST 1
#define METHOD_PUT 2
#define METHOD_DELETE 3

#include <iostream>
#include <string.h>
#include <map>
#include <algorithm>
#include <sstream>
#include <vector>

class Worker;

#include "Worker.hpp"

class Location {
private:
	std::string uri;
    std::string root;
    std::string index;
    bool auto_index;
	int	redir_status_code;
	std::string redir_uri;
    std::map<int, bool> limit_excepts;

public:
    Location();
    ~Location();
	void set_uri(const std::string& uri);
	void set_root(const std::string& root);
	void set_index(const std::string& index);
	void set_redir_status_code(int status);
	void set_redir_uri(const std::string& uri);
	void set_limit_excepts(int method, bool access);
	void set_auto_index(bool access);
	const std::string& get_uri() const;
	const std::string& get_root() const;
	const std::string& get_index() const;
	int get_redir_status_code() const;
	const std::string& get_redir_uri() const;
	const std::map<int, bool> get_limit_excepts() const;
	bool get_auto_index() const;
};

std::vector<std::string>::iterator set_location_token(Location& location, std::vector<std::string> lines, std::vector<std::string>::iterator& lineIt);
std::vector<std::string>::iterator set_location(Worker& worker, std::vector<std::string> lines, std::vector<std::string>::iterator& lineIt);
void	print_worker_info(Worker& worker);

#endif
