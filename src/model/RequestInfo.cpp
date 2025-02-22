#include "RequestInfo.h"

RequestInfo::RequestInfo(json request) : request(request)
{
}

uint8_t RequestInfo::getRequestType()
{
    if (!request.contains("header"))
    {
        throw std::runtime_error("request header is not exist");
    }
    if (!request["header"].contains("type"))
    {
        throw std::runtime_error("request header's type is not exist");
    }

    return request["header"]["type"];
}

std::string RequestInfo::exportRequest(){
    return request.dump(4);
}

json RequestInfo::getRequestHeader(){
    if (!request.contains("header"))
    {
        throw std::runtime_error("request header is not exist");
    }
    return request["header"];
}
