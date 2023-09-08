#include "../inc/Worker.hpp"

// port의 default 값 80
// root는 default root 존재함 (우리가 설정)
// index의 default 값 "index.html", "index.htm"
// server_name은 비어있을 경우 default server 찾아감, default server는 첫 번째 server(Worker.at(0))
// client_max_body_size가 0일 경우 제한 X
// location이 0일 경우 기본 경로인 '/' 경로에 대한 모든 요청에 적용

Worker::Worker() : server_socket(0), port(80), root("./html"), index_page("./index.html"), server_names(0),
				   client_max_body_size(0)
{
}

void Worker::run()
{
	sockaddr_in serverAddr;

	this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->server_socket == -1)
		throw std::runtime_error("socket error");

	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 임시로 일단은 다 받음.
	serverAddr.sin_port = htons(this->port);

	//port 재사용시 bind error 없애는 옵션 추가.
	int option = 1;
	setsockopt(this->server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	if (bind(this->server_socket, reinterpret_cast<const sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1)
		throw std::runtime_error("bind error");
	listen(this->server_socket, LISTEN_BACKLOG_NUM);
	// tcp 연결 완료.
}

Worker::~Worker()
{
}

void Worker::set_server_socket(int server_socket)
{
	this->server_socket = server_socket;
}

int Worker::get_server_socket()
{
	return this->server_socket;
}

void Worker::set_port(int port)
{
	this->port = port;
}

int Worker::get_port()
{
	return this->port;
}

void Worker::set_root(std::string root)
{
	this->root = root;
}

const std::string &Worker::get_root() const
{
	return this->root;
}

void Worker::set_index_page(std::string &index_page)
{
	this->index_page = index_page;
}

const std::string &Worker::get_index_page() const
{
	return this->index_page;
}

void Worker::add_server_name(std::string &server_name)
{
	this->server_names.push_back(server_name);
}

const std::vector<std::string> &Worker::get_server_names() const
{
	return this->server_names;
}

void Worker::set_client_max_body_size(size_t size)
{
	this->client_max_body_size = size;
}

size_t Worker::get_client_max_body_size() const
{
	return this->client_max_body_size;
}

 void Worker::add_locations(const Location& location) {
     this->locations.push_back(location);
 }

 const std::vector<Location>& Worker::get_locations() const {
     return this->locations;
 }

void	Worker::add_error_page(int err_no, std::string& error_page)
{
    this->error_pages[err_no] =  error_page;
}

const std::map<int, std::string> &Worker::get_error_page() const
{
	return this->error_pages;
}

//tcp 연결 끊김 감지 및 처리
bool Worker::is_connected(int fd, int kque, struct kevent event)
{
	if (event.flags & EV_EOF)
	{
        this->terminateConnection(fd, kque, event);
		return false;
	}
	return true;
}

void Worker::terminateConnection(int fd, int kque, struct kevent event)
{
    struct kevent temp_event;
    EV_SET(&temp_event, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(kque, &temp_event, 1, NULL, 0, NULL);
    close(fd);
    std::cout << fd << " : close!" << std::endl;
}

void Worker::handle_request(int fd, int kque, struct kevent event)
{
	if (is_connected(fd, kque, event) == false)
		return;
	char data[1024];
	if (read(fd, data, 1024) == -1)
		throw std::runtime_error("read error");


	std::string s = data;
	std::cout << fd << " : connected! - " << s.substr(0, s.find('\n')) << std::endl; // 첫줄만 출력ㅈ
	//std::cout << s << std::endl;

	Request request;
	Response response;

    try
    {
        request.parsingFromData(s);
    }
    catch (std::runtime_error &e)  // body size limit 넘어간 경우 처리
    {
        std::cout << e.what() << std::endl;
        response.handleBodySizeLimit();
        response.send(fd);
        this->terminateConnection(fd, kque, event);
        return;
    }
    response.parsingFromRequest(*this, request);
	response.send(fd);
}


