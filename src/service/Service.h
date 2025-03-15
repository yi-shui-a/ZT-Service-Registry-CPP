#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include <iostream>
#include "../model/entity/Header.h"
#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

// 定义 Service 抽象类
class Service {
public:
    // 纯虚函数，用于检查数据正确性
    virtual bool isValidData(const json& content) = 0;

    // 纯虚函数，用于处理数据
    virtual json processData(Header &header, const json& content) = 0;

    // 实函数，用于构造返回值
    json formatResponse(json data = json::object(), int status = 200, std::string message = "");

    // 虚析构函数，确保正确释放派生类对象
    virtual ~Service() {}
};

#endif // SERVICE_H
