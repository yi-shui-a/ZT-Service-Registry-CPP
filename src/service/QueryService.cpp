#include "QueryService.h"

// 实现 isValidData 函数
bool QueryService::isValidData(const json &content)
{
    // 这里可以添加具体的数据验证逻辑
    if (!content.contains("service_name"))
        return false;
    if (!content.contains("service_num"))
        return false;
    std::cout << "INFO: QueryService: Validate data finished" << std::endl;
    return true;
}

// 实现 processData 函数
json QueryService::processData(Header &header, const json &content)
{
    // 获取instance列表
    json instanceList = LocalDatabaseDao::getInstanceList(content["service_name"]);

    // 如果instance列表为空，返回空的json对象
    if (instanceList.empty())
    {
        std::cout << "ERROR: QueryService: Instance list is empty" << std::endl;
        return json::object();
    }

    // 构造返回的json对象
    json responseJson;
    responseJson["instance_list"] = json::array();
    // 如果instance数量小于content["service_name"]，返回全部instance；否则返回content["service_num"]个instance
    if (instanceList.size() <= content["service_num"])
    {
        for (int i = 0; i < instanceList.size(); i++){
            responseJson["instance_list"].push_back(processInstanceToQueryJson(instanceList[i]));
        }
        responseJson["service_num"] = instanceList.size();
    }
    else
    {
        for (int i = 0; i < content["service_num"]; i++)
        {
            responseJson["instance_list"].push_back(processInstanceToQueryJson(instanceList[i]));
        }
        responseJson["service_num"] = content["service_num"];
    }

    std::cout << "INFO: RegisterService: Process data finished" << std::endl;
    return responseJson;
}

json QueryService::processInstanceToQueryJson(const json &instance)
{
    json result = json::object(); // 创建一个新的 JSON 对象
    // 遍历原始 JSON 的键值对
    for (const auto &item : instance.items())
    {
        // 只选择需要的键值对
        if (item.key() == "address" || item.key() == "port" || item.key() == "server_name" || item.key() == "role")
        {
            result[item.key()] = item.value(); // 将其他键值对添加到新的 JSON 对象中
        }
    }
    return result;
}