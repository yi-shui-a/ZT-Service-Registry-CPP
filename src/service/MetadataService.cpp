#include "MetadataService.h"

// 实现 isValidData 函数
bool MetadataService::isValidData(const json &content)
{
    // 这里可以添加具体的数据验证逻辑
    if (!content.contains("server_name"))
        return false;
    if (!content.contains("address"))
        return false;
    if (!content.contains("port"))
        return false;
    if (!content.contains("services_list"))
        return false;
    // 使用范围 for 循环遍历并修改数组元素
    for (const json &element : content["services_list"])
    {
        if (!element.contains("service_name"))
            return false;
        if (!element.contains("metadata"))
            return false;
    }
    std::cout << "INFO: MetadataService: Validate data finished" << std::endl;
    return true;
}

// 实现 processData 函数
json MetadataService::processData(Header &header, const json &content)
{
    // 遍历json数组
    for (const json &element : content["services_list"])
    {
        // 如果不存在该service，添加service
        if (!LocalDatabaseDao::existService(element["service_name"]))
        {
            LocalDatabaseDao::addService(element["service_name"]);
        }
        // 如果不存在该instance，添加instance
        if (!LocalDatabaseDao::existInstance(element["service_name"], content["server_name"]))
        {
            LocalDatabaseDao::addInstance(element["service_name"], content["server_name"], content["address"], content["port"], header.sendTime, 99);
        }
        // 如果不存在该Metadata，添加Metadata
        if (!LocalDatabaseDao::existMetadata(element["service_name"]))
        {
            LocalDatabaseDao::addMetadata(element["service_name"], element["metadata"]);
        }
        else
        {
            // 如果存在该Metadata，修改Metadata
            LocalDatabaseDao::updateMetadata(element["service_name"], element["metadata"]);
        }
        // 如果不存在该instanceMetadata，添加instanceMetadata
        if (!LocalDatabaseDao::existInstanceMetadata(element["service_name"], content["server_name"]))
        {
            LocalDatabaseDao::addInstanceMetadata(element["service_name"], content["server_name"], processInstanceMetadata(element));
        }
        else
        {
            // 如果存在该instanceMetadata，修改instanceMetadata
            LocalDatabaseDao::updateInstanceMetadata(element["service_name"], content["server_name"], processInstanceMetadata(element));
        }
    }

    // 构造返回的json对象
    json responseJson;
    responseJson["server_name"] = content["server_name"];
    std::cout << "INFO: MetadataService: Process data finished" << std::endl;
    return responseJson;
}


json MetadataService::processInstanceMetadata(const json &service)
{
    json result = json::object(); // 创建一个新的 JSON 对象

    // 遍历原始 JSON 的键值对
    for (const auto &item : service.items()){
        // 跳过 metadata 和 service_name
        if (item.key()!= "metadata" && item.key()!= "service_name")
        {
            result[item.key()] = item.value(); // 将其他键值对添加到新的 JSON 对象中
        }
    }

    return result;
}