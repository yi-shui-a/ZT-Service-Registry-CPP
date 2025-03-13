#include "Service.h"

// 构造返回值的函数
json Service::formatResponse(json data = json::object(), int status = 200, std::string message = ""){
    // 这里可以添加具体的返回值构造逻辑
    std::cout << "RegisterService: Formatting response..." << std::endl;
    return data;
}