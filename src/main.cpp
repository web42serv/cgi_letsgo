#include "../inc/Webserv.hpp"

#define CONF_DEFAULT_PATH "./default.conf"

int main(int argc, char *argv[])
{
	//signal(SIGPIPE, SIG_IGN); // write 뭐가 잘안되어서 error날 시 테스트용
	if (argc > 2)
	{
		std::cerr << "Invalid number of arguments." << std::endl;
		return (1);
	}
	Webserv app;

	if (argc == 2)
		app.confFileParse(argv[1]);
	else
		app.confFileParse(CONF_DEFAULT_PATH);
	app.init();
	app.run();

	return (0);
}
