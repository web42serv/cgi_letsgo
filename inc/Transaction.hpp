#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>

class Transaction
{
protected:
	std::string httpVersion;
	int contentLength;
	std::string connection;
	std::string location; // 300번대 응답에서 redirect 시 사용.
	std::string contentType;
	std::string body;

    // 임시.. 나중에 다 추가해야 함.
    enum HttpStatusCode
    {
        OK = 200,
        CREATED = 201,
        ACCEPTED = 202,
        NO_CONTENT = 204,
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        CONTENT_TOO_LARGE = 413,
        INTERNAL_SERVER_ERROR = 500,
        NOT_IMPLEMENTED = 501,
        SERVICE_UNAVAILABLE = 503
    };

    enum HttpMethod
    {
        GET,
        POST,
        PUT,
        DELETE
    };

    // 필요 없다고 판단되는 type은 추후 삭제할 예정
    enum MimeType
    {
        APP_OCTET,          // default binary data
        APP_PDF,
        APP_JSON,
        APP_ZIP,
        APP_XML,
        APP_RSS_XML,
        AUDIO_MPEG,
        AUDIO_WAV,
        AUDIO_OGG,
        IMAGE_JPEG,
        IMAGE_PNG,
        IMAGE_GIF,
        IMAGE_SVG_XML,
        TEXT_PLAIN,         // default text data
        TEXT_CSV,
        TEXT_HTML,
        TEXT_CSS,
        TEXT_JAVASCRIPT,
        VIDEO_MP4,
        MULTIPART_FORM,     // used in POST HTML Forms
        MESSAGE_RFC822
    };

public:

};


#endif