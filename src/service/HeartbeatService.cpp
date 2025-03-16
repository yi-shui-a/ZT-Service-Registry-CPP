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
    }
    std::cout << "INFO: HeartbeatService: Validate data finished" << std::endl;
    return true;
}

// 实现 processData 函数
json HeartbeatService::processData(Header &header, const json &content)
{

    //遍历services_list
    for (const json &element : content["services_list"])
    {
        if(!LocalDatabaseDao::existInstance(element["service_name"], content["server_name"])){
            std::cout << "INFO: HeartbeatService: Instance " << content["server_name"] << " not exist" << std::endl;
            break;
        }
        //如果存在该instance，修改关键信息
        if(LocalDatabaseDao::updateInstanceHeartbeatTime(element["service_name"], content["server_name"], header.sendTime)){
            std::cout << "INFO: HeartbeatService: Update instance " << content["server_name"] << " heartbeat time" << std::endl;
        }else{
            std::cout << "WARNNING: HeartbeatService: Update instance " << content["server_name"] << " heartbeat time failed" << std::endl;
        }
    }

    // 构造返回的json对象
    // 返回一个空的json对象
    json responseJson = json::object();
    // responseJson["server_name"] = content["server_name"];
    std::cout << "INFO: HeartbeatService: Process data finished" << std::endl;
    return responseJson;
}
