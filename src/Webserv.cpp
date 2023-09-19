#include "../inc/Webserv.hpp"
#include "../inc/block_parser.hpp"

#define RED "\033[31m"
#define RESET "\033[0m"

Webserv::Webserv()
{
	this->kque = kqueue();
	if (this->kque == -1)
		throw std::runtime_error("kqueue error");
}

Webserv::~Webserv()
{
	close(kque);
}

void Webserv::addWorker(const Worker &worker)
{
	this->workers.push_back(worker);
}

void Webserv::confFileParse(std::string conf_file)
{
    ConfigParser	parser;
    Worker          worker;

    if (check_extension(conf_file) == -1) {
        std::cout << "File is not configuration file." << std::endl;
        return ;
    }
    if (check_block(conf_file, &parser) == -1) {
        std::cout << "configuration file is not vaild" << std::endl;
        return ;
    }
    for (int i = 0; i < parser.get_server().size(); i++) {
        worker =  set_worker_info(parser.server_[i]);
        this->workers.push_back(worker);
        print_worker_info(worker);
    }
}


void Webserv::init()
{
	struct kevent event;

	// workers들의 socket 생성
	for (size_t i = 0; i < this->workers.size(); i++)
		this->workers[i].run();

	//kque에 workers 들의 socket 등록
	for (size_t i = 0; i < workers.size(); i++)
	{
		EV_SET(&event, workers[i].get_server_socket(), EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(this->kque, &event, 1, NULL, 0, NULL) == -1)
			throw std::runtime_error("kevent attach error");
	}
}

void Webserv::run()
{
	while (true)
	{
		struct kevent events[32];
		int triggered = kevent(this->kque, NULL, 0, events, 32, NULL);
		if (triggered == -1)
        {
            std::cerr << strerror(errno) << std::endl;
            throw std::runtime_error("kevent event error");
        }
		for (int i = 0; i < triggered; i++)
		{
			bool flag = false;

			// 이벤트가 발생한 소켓에 대한 worker 찾고
			for (size_t worker_idx = 0; worker_idx < workers.size(); ++worker_idx)
			{
				if ((int) events[i].ident == workers[worker_idx].get_server_socket()) // 소켓에서 이벤트 발생
				{
					//새로운 접속 처리
					handleNewConnection(events[i].ident, worker_idx);
					flag = true;
				}
			}
			if (!flag) // 기존 접속
				handleExistingConnection(events[i].ident, events[i]);
		}
	}
}

// 새로운 접속이 이뤄지면 kque에 새로 등록하고 socket_to_worker에 기록
void Webserv::handleNewConnection(int sockFd, size_t worker_idx)
{
	struct sockaddr client_addr;
	struct kevent event;
	int client_addr_size = sizeof(client_addr);

	int newFd = accept(sockFd, (sockaddr *) &client_addr, (socklen_t *) &client_addr_size);
	if (newFd == -1)
		throw std::runtime_error("accept error");
	EV_SET(&event, newFd, EVFILT_READ, EV_ADD | EV_CLEAR, NOTE_WRITE, 0, NULL);
	kevent(this->kque, &event, 1, NULL, 0, NULL);

	socket_to_worker[sockFd] = worker_idx;
}

void Webserv::handleExistingConnection(int fd, struct kevent event)
{
	int worker_idx = socket_to_worker[fd]; // 해당 fd를 관리하는 worker 반환
	try
	{
		workers[worker_idx].handle_request(fd, this->kque, event);
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
