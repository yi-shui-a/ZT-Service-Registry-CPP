#ifndef HEARTBEATSERVICE_H
#define HEARTBEATSERVICE_H

#include "Service.h"
#include "../dao/LocalDatabaseDao.h"
#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class HeartbeatService : public Service {
public:
    // 检查数据正确性的函数
    bool isValidData(const json& content);

    // 处理数据的函数
    json processData(Header &header, const json& content);

};

#endif // HEARTBEATSERVICE_H
