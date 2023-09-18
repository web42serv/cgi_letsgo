#include "../inc/Request.hpp"
#include "iostream"

Request::Request()
{

}

Request::~Request()
{

}

std::string Request::getPath() const
{
    return this->path;
}

void Request::setPath(const std::string &path)
{
    this->path = path;
}

void Request::parsingFromData(std::string data)
{
    // max size_error 가정
    //throw std::runtime_error("client body size error");

    //test code
	std::stringstream stream;
	stream.str(data);

    std::string method;
    stream >> method;
    stream >> this->path;

	std::string s;
	while (stream >> s)
	{
		if (s == "Content-Length:")
			stream >> this->contentLength;
		if (s == "Content-Type:")
			stream >> this->contentType;
	}

	size_t bodyStart = data.find("\r\n\r\n") + 4; // 빈 줄 다음부터 본문
    this->body = data.substr(bodyStart, this->contentLength);


    this->host = "localhost:8080";
    if (method == "GET")
        this->httpMethod = GET;
    else if (method == "POST")
        this->httpMethod = POST;
    else if (method == "PUT")
        this->httpMethod = PUT;
    else if (method == "DELETE")
        this->httpMethod = DELETE;
}

std::string Request::getHost() const
{
    return this->host;
}

Transaction::HttpMethod Request::getHttpMethod() const
{
    return this->httpMethod;
}

std::string Request::getContentType() const
{
	return this->contentType;
}

std::string Request::getBody() const
{
	return this->body;
}

int	Request::getContentLength() const
{
	return this->contentLength;
}

std::string Request::getHttpMethodString() const
{
    switch (this->httpMethod)
    {
        case GET:
            return "GET";
        case POST:
            return "POST";
        case PUT:
            return "PUT";
        case DELETE:
            return "DELETE";
    }
}