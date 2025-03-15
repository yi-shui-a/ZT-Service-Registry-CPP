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

class LocalDatabaseDao
{
    // private:

public:
    static bool existService(const std::string &service);
    static bool existInstanceList(const std::string &service);
    static bool existMetadata(const std::string &service);
    static bool existInstance(const std::string &service, const std::string &server);
    static bool existInstanceMetadata(const std::string &service, const std::string &server);

    static std::string getInstanceId(std::string &service, std::string &server);
    static std::vector<std::string> getInstanceIdList(std::string &service);
    static json getMetadata(std::string &service);
    static json getInstanceMetadata(std::string &service, std::string &instanceId);
    static json getInstanceServer(std::string &service, std::string &instanceId);
    static json getInstanceAddress(std::string &service, std::string &instanceId);
    static json getInstancePort(std::string &service, std::string &instanceId);
    static json getInstanceHeartbeatTime(std::string &service, std::string &instanceId);
    static json getInstanceStatus(std::string &service, std::string &instanceId);
    static json getInstanceRole(std::string &service, std::string &instanceId);

    static bool addService(std::string service);
    static bool addInstanceList(std::string service);
    static std::string addInstance(std::string service, std::string server, std::string address, int port, long long heartbeatTime,int role);
    static bool addMetadata(std::string service);
    static bool addMetadata(std::string service, json metadata);
    static bool addInstanceMetadata(std::string service, std::string server);
    static bool addInstanceMetadata(std::string service, std::string server, json metadata);



    static json setMetadata(std::string &service, json metadata);
    static json setInstanceMetadata(std::string &service, std::string &instanceId, json metadata);

    static bool updateService(std::string service);
    static bool updateMetadata(std::string service, json metadata);
    static bool updateInstance(std::string service, std::string server, std::string address, int port, long long heartbeatTime ,int role);
    static bool updateInstanceMetadata(std::string service, std::string server, json metadata);
    static bool updateInstanceHeartbeatTime(std::string service, std::string server, long long heartbeatTime);

    static int getServiceCount();
    static int getServiceInstanceCount(std::string &service);

    static std::string generateInstanceId(std::string &service, std::string &server, long long heartbeatTime);
};

#endif // LOCALDATABASEDAO_H
