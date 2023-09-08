#include "../inc/block_parser.hpp"
#include "../inc/Location.hpp"
#include "../inc/Worker.hpp"

int	check_extension(std::string filename) {
	std::string	extention;
	int			index;

	index = filename.find(".");
	if (index == -1)
		return (-1);
	extention = filename.substr(index);
	if (extention.compare(".conf") != 0)
		return (-1);
	return (1);
}

int	check_block(std::string filename, ConfigParser *parser) {
	std::ifstream	ifs(filename, std::ifstream::in);

	if (ifs.is_open() == false)
		return (-1);
	parser->init_parserclass();
	parser->start_parsing(&ifs);
	parser->print_parse_error(filename);
	return (0);
}
