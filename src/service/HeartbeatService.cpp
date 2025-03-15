#include "HeartbeatService.h"

// 实现 isValidData 函数
bool HeartbeatService::isValidData(const json &content)
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
        if (!element.contains("role"))
            return false;
    }
    std::cout << "INFO: RegisterService: Validate data finished" << std::endl;
    return true;
}

// 实现 processData 函数
json HeartbeatService::processData(Header &header, const json &content)
{
    // 遍历json数组
    for (const json &element : content["services_list"])
    {
        // 如果本就存在该instance，修改关键信息
        if (LocalDatabaseDao::existInstance(element["service_name"], content["server_name"]))
        {
            LocalDatabaseDao::updateInstance(element["service_name"], content["server_name"], content["address"], content["port"], header.sendTime, element["role"]);
        }
        else
        {
            // 如果不存在该instance，添加instance
            if (LocalDatabaseDao::existService(element["service_name"]))
            {
                if (!LocalDatabaseDao::existInstanceList(element["service_name"]))
                {
                    LocalDatabaseDao::addInstanceList(element["service_name"]);
                }
            }
            else
            {
                LocalDatabaseDao::addService(element["service_name"]);
            }
            LocalDatabaseDao::addInstance(element["service_name"], content["server_name"], content["address"], content["port"], header.sendTime, element["role"]);
        }
    }
    // 构造返回的json对象
    json responseJson;
    responseJson["server_name"] = content["server_name"];
    std::cout << "INFO: RegisterService: Process data finished" << std::endl;
    return responseJson;
}
