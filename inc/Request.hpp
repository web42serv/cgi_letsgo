#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sstream>

#include "Transaction.hpp"

class Request : public Transaction
{
private:
    HttpMethod httpMethod;
    std::string host;
    std::string path;

public:
    Request();
    ~Request();

    void setPath(const std::string &path);
    std::string getPath() const;
	std::string getBody() const;
    std::string getHost() const;
	std::string getContentType() const;
	int	getContentLength() const;
    HttpMethod getHttpMethod() const;
    std::string getHttpMethodString() const;
    void parsingFromData(std::string data);
};


#endif
