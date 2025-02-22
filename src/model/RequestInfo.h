#ifndef REQUESTINFO_H
#define REQUESTINFO_H

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class RequestInfo {
private:
    json request;

public:
    // 构造函数
    RequestInfo(json request);
    // 获取请求类型
    uint8_t getRequestType();
    // 以string形式导出request
    std::string exportRequest();
    // 获取请求消息的消息头
    json getRequestHeader();
    // 获取请求消息的消息体
    // json getRequestBody();

    
    
    

};

#endif // REQUESTINFO_H

