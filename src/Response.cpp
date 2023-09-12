#include "../inc/Response.hpp"

Response::Response()
{
}

Response::~Response()
{

}

// 나중에 헤더 있을 경우만 추가하도록 수정
void Response::send(int fd)
{
    std::string toSend;

    // 임시 하드코딩
    toSend += "HTTP/" + this->httpVersion;
    toSend += " " + std::to_string(this->statusCode);
    toSend += " " + this->getStatusMessage(this->statusCode);
    toSend += "\r\n";

    if (!this->contentType.empty())
	    toSend += "Content-Type: " + this->contentType + "\r\n";
	if (!this->body.empty())
        toSend += "Content-Length: " + std::to_string(this->body.size()) + "\r\n";
	if (!this->connection.empty())
        toSend += "Connection: " + this->connection + "\r\n";
	//header end

    toSend += "\r\n";

    toSend += this->body;
    fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC); // write함수 non-block으로 변환
    if (write(fd, toSend.c_str(), toSend.size()) == -1)
        throw std::runtime_error("write error");
}

std::string Response::getStatusMessage(int code)
{
	switch (code)
	{
		case OK:
			return "OK";
		case CREATED:
			return "Created";
		case ACCEPTED:
			return "Accepted";
		case NO_CONTENT:
			return "No Content";
		case BAD_REQUEST:
			return "Bad Request";
		case UNAUTHORIZED:
			return "Unauthorized";
		case FORBIDDEN:
			return "Forbidden";
		case NOT_FOUND:
			return "Not Found";
		case INTERNAL_SERVER_ERROR:
			return "Internal Server Error";
		case NOT_IMPLEMENTED:
			return "Not Implemented";
		case SERVICE_UNAVAILABLE:
			return "Service Unavailable";
        case CONTENT_TOO_LARGE:
            return "Content Too Large";
		default:
			return "Unknown";
	}
}

void Response::readFileToBody(const std::string &filePath)
{
    std::ifstream fin;
    fin.open(filePath.c_str());
    if (fin.fail())
        throw std::runtime_error("file open error");
    std::string line;
    while (getline(fin, line))
    {
        line += "\r\n";
        this->body += line;
    }
    fin.close();
}

void Response::generateBody(Worker &worker, const Request &request)
{
	std::string filePath = worker.get_root();
	filePath += request.getPath();

	if (this->checkPath(filePath) == 1) // autoindex 해야하는 상황
	{
		this->generateBody_AutoIndexing(filePath, request);
		return;
	}
    this->readFileToBody(filePath);
}

void Response::generateBody_AutoIndexing(const std::string &path, const Request &request)
{
	this->readFileToBody(AUTO_INDEX_HTML_PATH); // 템플릿 전부 읽기

	size_t toInsert;

	//title 지정
	toInsert = this->body.find("</title>");
	this->body.insert(toInsert, "Index of " + request.getPath());

	//head 지정
	toInsert = this->body.find("</h1>");
	this->body.insert(toInsert, "Index of " + request.getPath());

	// files 이름 지정
	std::string requestPath = request.getPath();
	if (requestPath.back() != '/') // request path가 '/'로 끝나지 않는 directory일 때 버그 방지
		requestPath.push_back('/');
	std::vector<std::string> fileNames = getFilesInDirectory(path);
	for (size_t i = 0; i < fileNames.size(); i++)
	{
		toInsert = this->body.rfind("<hr>");
		this->body.insert(toInsert,std::string() +
                          "\t<pre>" + "<a href=\"" + requestPath
						  + fileNames[i] + "\">" + fileNames[i] + "</a></pre>\r\n");
	}
}

void Response::parsingFromRequest(Worker &worker, const Request &request)
{
   if (request.getPath().find("cgi") != std::string::npos)
   {
		CgiHandler cgi;
		cgi.executeCgi(request);
		this->statusCode = OK;
		this->connection = "keep-alive";
		this->contentType = "text/html";
		this->httpVersion = "1.1";
		this->location = "";
		this->body = "cgi success";
		return ;
   }

    HttpMethod method = request.getHttpMethod();

    switch (method)
    {
        case GET:
            handleGET(worker, request);
            break;
        case POST:
            handlePOST(worker, request);
            break;
        case PUT:
            handlePUT(worker, request);
            break;
        case DELETE:
            handleDELETE(worker, request);
            break;
        default:
            // 올바르지 않은 http 메서드에 대한 에러 처리 함수 (인자는 아직 미정)
            // handleInvalidMethod();
            break;
    }
}

int Response::checkPath(std::string &filePath)
{
    struct stat buf;

    if (stat(filePath.c_str(), &buf) == -1) // 해당 경로에 파일이 존재 안하면 404Page
    {
        filePath = ERROR_PAGE_404_PATH;
        this->statusCode = NOT_FOUND;
    } else if (S_ISDIR(buf.st_mode)) // 경로일 때
    {
        if (/* directoryListing  == */ true) // 나중에 directory listing 추가해야 함.
        {
            return 1;// directory listing 추후 처리
        } else
        {
            filePath = ERROR_PAGE_404_PATH;
            this->statusCode = NOT_FOUND;
        }
    }
    return 0;
}

std::vector<std::string> Response::getFilesInDirectory(const std::string &dirPath)
{
    DIR *dir_info;
    struct dirent *dir_entry;
    std::vector<std::string> ret;

    if ((dir_info = opendir(dirPath.c_str())) == NULL)
        throw std::runtime_error("opendir error");

    while ((dir_entry = readdir(dir_info)))
    {
        if (std::strcmp(dir_entry->d_name, ".") == 0)
            continue;
        if (dir_entry->d_type == DT_DIR) // directory 일 경우 뒤에 / 추가.
            ret.push_back(std::string(dir_entry->d_name) + "/");
        else
            ret.push_back(dir_entry->d_name);
    }
    closedir(dir_info);

    return ret;
}

void Response::handleBodySizeLimit()
{
    this->statusCode = CONTENT_TOO_LARGE;
    this->connection = "close";
    this->readFileToBody(ERROR_PAGE_413_PATH);
    this->contentLength = this->body.size();
    this->contentType = "text/html";
}

void Response::handleGET(Worker &worker, const Request &request)
{
    this->statusCode = OK;
    this->connection = "keep-alive";
    this->contentType = "text/html";
    this->httpVersion = "1.1";
    this->location = "";
    try
    {
        generateBody(worker, request);
    }
    catch (std::runtime_error &e)
    {
        // 에러 처리 필요
    }
}

void Response::handlePOST(Worker &worker, const Request &request) {}

void Response::handlePUT(Worker &worker, const Request &request) {}

void Response::handleDELETE(Worker &worker, const Request &request) {}
