#ifndef LOCALDATABASEDAO_H
#define LOCALDATABASEDAO_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../database/LocalDatabase.h"


#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;


/**
 * @brief 操作本地数据库的类
 * 
 * 所有方法均为静态方法
 * 
 */

class LocalDatabaseDao {
// private:

public:
    static bool existService(std::string& service);
    static bool existInstance(std::string& service);
    static bool existMetadata(std::string& service);
    static bool existInstanceMetadata(std::string& service, std::string& server);
    
    static std::string getInstanceId(std::string& service, std::string& server);
    static std::vector<std::string> getInstanceIdList(std::string& service);
    static json getMetadata(std::string& service);
    static json getInstanceMetadata(std::string& service, std::string& instanceId);
    static json getInstanceServer(std::string& service, std::string& instanceId);
    static json getInstanceAddress(std::string& service, std::string& instanceId);
    static json getInstancePort(std::string& service, std::string& instanceId);
    static json getInstanceHeartbeatTime(std::string& service, std::string& instanceId);
    static json getInstanceStatus(std::string& service, std::string& instanceId);
    static json getInstanceRole(std::string& service, std::string& instanceId);

    static bool addService(std::string service);
    static bool addMetadata(std::string service);


    static bool updateService(std::string service);
    static bool updateMetadata(std::string service);

    static json setMetadata(std::string& service);
    static json setInstanceMetadata(std::string& service, std::string& instanceId);

    
};

#endif // LOCALDATABASEDAO_H
