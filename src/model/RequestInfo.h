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
    // 获取发出时间
    time_t getSendTime();
    // 以string形式导出request
    std::string exportRequest();
    // 获取请求消息的消息头
    json getRequestHeader();
    // 获取请求消息的json实例
    json getRequestInstance();

    
    // 判断消息是否为空
    bool isEmpty();
    bool isBodyEmpty();
    bool isHeaderEmpty();
    

};

#endif // REQUESTINFO_H

