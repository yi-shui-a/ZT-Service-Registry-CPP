#ifndef QUERYSERVICE_H
#define QUERYSERVICE_H

#include "Service.h"
#include "../dao/LocalDatabaseDao.h"
#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class QueryService : public Service {
public:
    // 检查数据正确性的函数
    bool isValidData(const json& content);

    // 处理数据的函数
    json processData(Header &header, const json& content);

    // 处理请求报文的instance元素，只保留query——response报文需要的部分
    json processInstanceToQueryJson(const json& instance);

};

#endif // QUERYSERVICE_H
