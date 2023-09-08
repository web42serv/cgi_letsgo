#include "../inc/Worker.hpp"
#include "../inc/Location.hpp"

std::vector<std::string>::iterator set_location_token(Location& location, std::vector<std::string> lines, std::vector<std::string>::iterator& lineIt)
{
	const std::string& line = *lineIt;

	std::map<int, bool> method_tokens;
	method_tokens[METHOD_GET] = false;
	method_tokens[METHOD_POST] = false;
	method_tokens[METHOD_PUT] = false;
	method_tokens[METHOD_DELETE] = false;

	if (line.compare("limit_except") == 0)
	{
		lineIt++;
		while(lineIt != lines.end() && (*lineIt).compare(";")) {
            if (!(*lineIt).compare("GET") && !method_tokens[METHOD_GET])
                method_tokens[METHOD_GET] = true;
            else if (!(*lineIt).compare("POST") && !method_tokens[METHOD_POST])
                method_tokens[METHOD_POST] = true;
            else if (!(*lineIt).compare("PUT") && !method_tokens[METHOD_PUT])
                method_tokens[METHOD_PUT] = true;
            else if (!(*lineIt).compare("DELETE") && !method_tokens[METHOD_DELETE])
                method_tokens[METHOD_DELETE] = true;
            else {
                std::cout << "Error : invalid method or duplicate method" << std::endl;
                exit(1);
            }
            lineIt++;
        }
		std::map<int, bool>::iterator method_it;
		for (method_it = method_tokens.begin(); method_it != method_tokens.end(); ++method_it)
			location.set_limit_excepts(method_it->first, method_it->second);
        lineIt--;
	}
	else if (line.compare("root") == 0)
	{
		lineIt++;
		location.set_root(*lineIt);
	}
	else if (line.compare("index") == 0)
	{
		lineIt++;
		location.set_index(*lineIt);
	}
	else if (line.compare("autoindex") == 0)
	{
		lineIt++;
		if ((*lineIt).compare("on") == 0)
			location.set_auto_index(true);
		else if ((*lineIt).compare("off") == 0)
			location.set_auto_index(false);
		else
		{
			std::cout << "Error : invalid autoindex" << std::endl;
			exit(1);
		}
	}
	else if (line.compare("return") == 0)
	{
		lineIt++;

		std::stringstream ss(*lineIt);
   	 	double value = 0.0;
   	 	char suffix = '\0';
    	
		ss >> value >> suffix;

		//status code 정의 후 value가 해당 status code들에 속해있지 않으면 에러
		if (value && !suffix) {
       		location.set_redir_status_code(value);
    	} else {
        	std::cout << "Error: invalid redirect status code" << std::endl;
			exit(1);
    	}

		lineIt++;
		location.set_redir_uri(*lineIt);
	}
	lineIt++;
	if ((*lineIt).compare(";"))
	{
		std::cout << "Error : not exist semicolon" << std::endl;
		exit(1);
	}
	return lineIt;
}

std::vector<std::string>::iterator set_location(Worker& worker, std::vector<std::string> lines, std::vector<std::string>::iterator& lineIt)
{
	const std::string& line = *lineIt;
	Location location;
	
	location.set_root(worker.get_root());
	location.set_index(worker.get_index_page());
	location.set_uri(line);

	lineIt++;

	std::map<std::string, bool> location_tokens;
	location_tokens["limit_except"] = false;
	location_tokens["root"] = false;
	location_tokens["index"] = false;
	location_tokens["autoindex"] = false;
	location_tokens["return"] = false;

	if ((*lineIt).compare("{"))
	{
		std::cout << "Error : location block open" << std::endl;
		exit(1);
	}

	lineIt++;

	while(lineIt != lines.end() && (*lineIt).compare("}"))
	{
		const std::string& line = *lineIt;

		std::map<std::string, bool>::iterator tokenIt;
		tokenIt = location_tokens.find(line);

		if (tokenIt != location_tokens.end()) {
			if (tokenIt->second) {
					std::cerr << "Error: Duplicate location token : " << line << std::endl;
					exit(1);
				} else {
					lineIt = set_location_token(location, lines, lineIt);
					tokenIt->second = true;
				}
		}
		else
		{
			std::cerr << "Error: Invalid location token : " << line << std::endl;
				exit(1);
		}
		lineIt++;
	}
	if ((*lineIt).compare("}"))
	{
		std::cout << "Error : location block close" << std::endl;
		exit(1);
	}
	worker.add_locations(location);
	return ++lineIt;
}

void	print_worker_info(Worker& worker)
{
    std::cout << "////////////Worker////////////" << std::endl;
	std::cout << "port : " << worker.get_port() << std::endl;
	for (size_t i = 0; i < worker.get_server_names().size(); i++)
	{
		std::cout << "server_name : " << worker.get_server_names().at(i) << std::endl;
	}
	std::cout << "error_page : ";
	std::map<int, std::string>::const_iterator it;
    for (it = worker.get_error_page().begin(); it != worker.get_error_page().end(); ++it) {
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    }

	std::cout << "client_max_body_size : " << worker.get_client_max_body_size() << std::endl;
	std::cout << "root : " << worker.get_root() << std::endl;
	std::cout << "index : " << worker.get_index_page() << std::endl;

	std::vector<Location> locations = worker.get_locations();
	for (size_t i = 0; i < locations.size(); i++)
	{
        std::cout << "////////////Location////////////" << std::endl;
		std::cout << "uri : " << locations.at(i).get_uri() << std::endl;
		std::map<int, bool> limit_excepts = locations.at(i).get_limit_excepts();
		std::cout << "method get : " << limit_excepts[METHOD_GET] << std::endl;
		std::cout << "method post : " << limit_excepts[METHOD_POST] << std::endl;
		std::cout << "method put : " << limit_excepts[METHOD_PUT] << std::endl;
		std::cout << "method delete : " << limit_excepts[METHOD_DELETE] << std::endl;
		std::cout << "root : " << locations.at(i).get_root() << std::endl;
		std::cout << "index : " << locations.at(i).get_index() << std::endl;
		std::cout << "autoindex : " << locations.at(i).get_auto_index() << std::endl;
		std::cout << "redirect status code : " << locations.at(i).get_redir_status_code() << std::endl;
		std::cout << "redirect uri : " << locations.at(i).get_redir_uri() << std::endl;
        std::cout << "///////////////end Location/////////////////" << std::endl;
    }
    std::cout << "///////////////end Worker/////////////////" << std::endl;
}