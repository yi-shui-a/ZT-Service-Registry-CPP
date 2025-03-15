#ifndef REGISTERSERVICE_H
#define REGISTERSERVICE_H

#include "Service.h"
#include "../dao/LocalDatabaseDao.h"
#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class RegisterService : public Service {
public:
    // 检查数据正确性的函数
    bool isValidData(const json& content);

    // 处理数据的函数
    json processData(Header &header, const json& content);
};

#endif // REGISTERSERVICE_H
