#include "RequestService.h"

RequestService::RequestService(RequestInfo request) : request(request)
{
}

// uint8_t RequestService::getRequestType()
// {
//     if (!request.contains("header"))
//     {
//         throw std::runtime_error("request header is not exist");
//     }
//     if (!request["header"].contains("type"))
//     {
//         throw std::runtime_error("request header's type is not exist");
//     }

//     return request["header"]["type"];
// }

// std::string RequestService::exportRequest(){
//     return request.dump(4);
// }
