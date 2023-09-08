#include "../inc/Location.hpp"

Location::Location() : uri("default"), root("default"), index("index.html"), auto_index(false)
{
	//access ok : true
	this->limit_excepts[METHOD_GET] = true;
	this->limit_excepts[METHOD_POST] = true;
	this->limit_excepts[METHOD_PUT] = true;
	this->limit_excepts[METHOD_DELETE] = true;
}

Location::~Location()
{
	
}

void Location::set_uri(const std::string& uri)
{
	this->uri = uri;
}

void Location::set_root(const std::string& root)
{
	this->root = root;
}

void Location::set_index(const std::string& index)
{
	this->index = index;
}

void Location::set_redir_status_code(int status)
{
	this->redir_status_code = status;
}

void Location::set_redir_uri(const std::string& uri)
{
	this->redir_uri = uri;
}

void Location::set_limit_excepts(int method, bool access)
{
	this->limit_excepts[method] = access;
}

void Location::set_auto_index(bool access)
{
	this->auto_index = access;
}

const std::string& Location::get_uri() const
{
	return this->uri;
}

const std::string& Location::get_root() const
{
	return this->root;
}

const std::string& Location::get_index() const
{
	return this->index;
}

int Location::get_redir_status_code() const
{
	return this->redir_status_code;
}

const std::string& Location::get_redir_uri() const
{
	return this->redir_uri;
}

const std::map<int, bool> Location::get_limit_excepts() const
{
	return this->limit_excepts;
}

bool Location::get_auto_index() const
{
	return this->auto_index;
}
