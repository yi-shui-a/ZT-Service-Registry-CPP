#ifndef REQUESTSERVICE_H
#define REQUESTSERVICE_H

#include <iostream>
#include <fstream>
#include <string>

#include "../model/RequestInfo.h"

#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class RequestService {
private:
    RequestInfo request;

public:
    // 构造函数
    RequestService(RequestInfo request);
    // // 获取请求类型
    // uint8_t getRequestType();
    // // 以string形式导出request
    // std::string exportRequest();

    // 将注册消息注册到数据库
    

    // 将元数据注册到数据库

    // 在数据库中查询服务

    // 更新数据库中的心跳时间


};

#endif // REQUESTSERVICE_H