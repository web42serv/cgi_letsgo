#include "../inc/Worker.hpp"

Worker set_worker_info(std::vector<std::string>& lines)
{
	if (lines.size() < 3 || lines.at(0).compare("server") || lines.at(1).compare("{") || lines.at(lines.size() - 1).compare("}"))
	{
		std::cerr << "Error: Invalid Server block" << std::endl;
		exit(1);
	}
	Worker worker;
	//insert server tokens string : config, bool : appearance

	std::map<std::string, bool> server_tokens;
	server_tokens["listen"] = false;
	server_tokens["server_name"] = false;
	server_tokens["error_page"] = false;
	server_tokens["client_max_body_size"] = false;
	server_tokens["root"] = false;
	server_tokens["index"] = false;
	server_tokens["location"] = false;

	std::vector<std::string>::iterator lineIt;
		for (lineIt = lines.begin() + 2; lineIt != lines.end(); ++lineIt) {
			std::string line = *lineIt;

			if (line.compare("location") == 0)
			{
				lineIt = set_location(worker, lines, ++lineIt);
				line = *lineIt;
			}
			if (lineIt == lines.end() - 1)
				break ;
			std::map<std::string, bool>::iterator tokenIt;
			tokenIt = server_tokens.find(line);

			if (lineIt == lines.end())
				break;
			if (tokenIt != server_tokens.end()) {
				if (tokenIt->second && tokenIt->first.compare("location")) {
					std::cerr << "Error: Duplicate server token : " << line << std::endl;
					exit(1);
				} else {
					lineIt = set_worker(worker, lines, lineIt);
					tokenIt->second = true;
				}
			}
			else
			{
				std::cerr << "Error: Invalid server token:" << line << std::endl;
					exit(1);
			}
	}
	return worker;
}

std::vector<std::string>::iterator set_worker(Worker& worker, std::vector<std::string> lines, std::vector<std::string>::iterator& lineIt)
{
	const std::string& line = *lineIt;

	if (line.compare("listen") == 0)
	{
		lineIt++;
		std::stringstream ss(*lineIt);
   	 	double value = 0.0;
   	 	char suffix = '\0';
    	
		ss >> value >> suffix;

		if (value && !suffix && value >= 0 && value <= PORT_NUM_MAX) {
       		worker.set_port((int)value);
    	} else {
        	std::cout << "Error: invalid port" << std::endl;
			exit(1);
    	}
	}
	else if (line.compare("error_page") == 0)
	{
		lineIt++;
		while (lineIt != lines.end() && (*lineIt).compare(";"))
		{
			std::stringstream ss(*lineIt);
			double value = 0.0;
			char suffix = '\0';
			
			ss >> value >> suffix;

			if (value && !suffix) {
				lineIt++;
				worker.add_error_page((int)value, *lineIt);
			} else {
				std::cout << "Error: invalid err_no" << std::endl;
				exit(1);
			}
			lineIt++;
		}
		lineIt--;
	}
	else if (line.compare("server_name") == 0)
	{
		lineIt++;
		worker.add_server_name(*lineIt);
	}
	else if (line.compare("client_max_body_size") == 0)
	{
		lineIt++;
		std::stringstream ss(*lineIt);
   	 	double value = 0.0;
   	 	char suffix = '\0';
    	
		ss >> value >> suffix;

		if (!suffix && value)
			worker.set_client_max_body_size(value);
		else if (tolower(suffix) == 'm')
		{
			value *= 1000000;
			worker.set_client_max_body_size(value);
		}
		else
		{
			std::cout << "[client_max_body_size] directive invalid value" << std::endl;
			exit(1);
		}
	}
	else if (line.compare("root") == 0)
	{
		lineIt++;
		worker.set_root(*lineIt);
	}
	else if (line.compare("index") == 0)
	{
		lineIt++;
        worker.set_index_page(*lineIt);
	}
	else if (line.compare("location") == 0)
	{
		return --lineIt;
	}
	else
	{
		std::cout << "Error : invalid factor " << line << std::endl;
		exit(1);
	}
	lineIt++;
	if ((*lineIt).compare(";"))
		lineIt++;
	return lineIt;
}