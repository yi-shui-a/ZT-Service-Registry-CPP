#ifndef RESPONSEINFO_H
#define RESPONSEINFO_H

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class ResponseInfo {
private:
    json response;

public:
    // 构造函数
    ResponseInfo(json request);
    // 获取请求类型
    uint8_t getResponseType();
    // 以string形式导出response
    std::string exportRequest();
    // 获取response
    json getResponse();
    // 获取response的消息体部分
    json getResponseBody();

    // 判断消息是否为空
    bool isEmpty();
    
    

};

#endif // RESPONSEINFO_H

