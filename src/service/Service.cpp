#include "Service.h"

// 构造返回值的函数
json Service::formatResponse(json data, int status, std::string message)
{
    std::cout << "RegisterService: Formatting response..." << std::endl;
    json res;
    res["status"] = 200;
    res["message"] = message;
    res["data"] = data;
    return res;
}
