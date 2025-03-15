#ifndef METADATASERVICE_H
#define METADATASERVICE_H

#include "Service.h"
#include "../dao/LocalDatabaseDao.h"
#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class MetadataService : public Service {
public:
    // 检查数据正确性的函数
    bool isValidData(const json& content);

    // 处理数据的函数
    json processData(Header &header, const json& content);

    // 处理请求报文的services_list数组中的单个元素，只保留instance_metadata部分
    json processInstanceMetadata(const json& service);
};

#endif // METADATASERVICE_H
