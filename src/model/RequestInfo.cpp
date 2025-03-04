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
    uint8_t res = static_cast<uint8_t>(request["header"]["type"]);

    return res;
}

time_t RequestInfo::getSendTime(){
    if (!request.contains("header"))
    {
        throw std::runtime_error("request header is not exist");
    }
    if (!request["header"].contains("type"))
    {
        throw std::runtime_error("request header's type is not exist");
    }

    return static_cast<time_t>(request["header"]["type"]);
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

json RequestInfo::getRequestInstance(){
    if (request == NULL)
    {
        throw std::runtime_error("request is not exist");
    }
    return request;
}

bool RequestInfo::isEmpty(){
    if(request == NULL){
        return true;
    }
    else{
        return false;
    }
}
bool RequestInfo::isHeaderEmpty(){
    if(request["header"] == NULL){
        return true;
    }else{
        return false;
    }
}

