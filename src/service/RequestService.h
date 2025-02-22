#ifndef REQUESTSERVICE_H
#define REQUESTSERVICE_H

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class RequestService {
private:
    json request;

public:
    // 构造函数
    RequestService(json request);
    // 获取请求类型
    uint8_t getRequestType();
    // 以string形式导出request
    std::string exportRequest();
    
    

};

#endif // REQUESTSERVICE_H