#include "LocalDatabaseDao.h"

bool LocalDatabaseDao::existService(const std::string &service)
{
    LocalDatabase *database = LocalDatabase::getInstance();
    json serviceList = database->getJsonDatabase()["services"];
    for (json &element : serviceList)
    {
        if (element["service_name"] == service)
        {
            return true;
        }
    }
    return false;
}

bool LocalDatabaseDao::existInstanceList(const std::string &service)
{
    LocalDatabase *database = LocalDatabase::getInstance();
    json serviceList = database->getJsonDatabase()["services"];
    for (json &element : serviceList)
    {
        if (element["service_name"] == service)
        {
            if (element.contains("instances"))
            {
                return true;
            }
        }
    }
    return false;
}

bool LocalDatabaseDao::existMetadata(const std::string &service)
{
    LocalDatabase *database = LocalDatabase::getInstance();
    json serviceList = database->getJsonDatabase()["services"];
    for (json &element : serviceList)
    {
        if (element["service_name"] == service)
        {
            if (element.contains("metadata"))
            {
                return true;
            }
        }
    }
    return false;
}

bool LocalDatabaseDao::existInstance(const std::string &service, const std::string &server)
{
    LocalDatabase *database = LocalDatabase::getInstance();
    json serviceList = database->getJsonDatabase()["services"];
    for (json &element : serviceList)
    {
        if (element["service_name"] == service)
        {
            if (element.contains("instances"))
            {
                json instanceList = element["instances"];
                for (json &instance : instanceList)
                {
                    if (instance["server_name"] == server)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool LocalDatabaseDao::existInstanceMetadata(const std::string &service, const std::string &server)
{
    LocalDatabase *database = LocalDatabase::getInstance();
    json serviceList = database->getJsonDatabase()["services"];
    for (json &element : serviceList)
    {
        if (element["service_name"] == service)
        {
            if (element.contains("instances"))
            {
                json instanceList = element["instances"];
                for (json &instance : instanceList)
                {
                    if (instance["server_name"] == server)
                    {
                        if (instance.contains("metadata"))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool LocalDatabaseDao::addService(std::string service)
{
    if (LocalDatabaseDao::existService(service))
    {
        std::cout << "INFO: (addService) Service already exists" << std::endl;
        return false;
    }
    LocalDatabase *database = LocalDatabase::getInstance();
    json newService = json::object();
    newService["service_name"] = service;
    database->getJsonDatabase()["services"].push_back(newService);
    LocalDatabaseDao::addInstanceList(service);
    return true;
}

bool LocalDatabaseDao::addInstanceList(std::string service)
{
    if (LocalDatabaseDao::existService(service))
    {
        LocalDatabase *database = LocalDatabase::getInstance();
        json& serviceList = database->getJsonDatabase()["services"];
        for (json &element : serviceList)
        {
            if (element["service_name"] == service && !element.contains("instances"))
            {
                element["instances"] = json::array();
                return true;
            }
        }
    }
    std::cout << "INFO: (addInstanceList) Service do not exists" << std::endl;
    return false;
}

std::string LocalDatabaseDao::addInstance(std::string service, std::string server, std::string address, int port, long long heartbeatTime, int role)
{
    if (!LocalDatabaseDao::existService(service))
    {
        std::cout << "INFO: (addInstance) Service does not exist" << std::endl;
        return "";
    }
    LocalDatabase *database = LocalDatabase::getInstance();
    if (!LocalDatabaseDao::existInstanceList(service))
    {
        LocalDatabaseDao::addInstanceList(service);
    }
    json& serviceList = database->getJsonDatabase()["services"];
    for (json &element : serviceList)
    {
        if (element["service_name"] == service)
        {
            json newInstance = json::object();
            newInstance["instance_id"] = LocalDatabaseDao::generateInstanceId(service, server, heartbeatTime);
            newInstance["server_name"] = server;
            newInstance["address"] = address;
            newInstance["port"] = port;
            newInstance["heartbeat_time"] = heartbeatTime;
            newInstance["status"] = "UP";
            newInstance["role"] = role;
            newInstance["metadata"] = json::object();
            element["instances"].push_back(newInstance);
            return newInstance["instance_id"];
        }
    }

    return "ERROR: (addService) Service does not exist";
}

// json LocalDatabaseDao::setMetadata(std::string &service, json metadata);
// json LocalDatabaseDao::setInstanceMetadata(std::string &service, std::string &instanceId, json metadata);


bool LocalDatabaseDao::updateInstance(std::string service, std::string server, std::string address, int port, long long heartbeatTime ,int role){
    if(!LocalDatabaseDao::existInstance(service,server)){
        std::cout<<"INFO: (updateInstance) Instance does not exist"<<std::endl;
        return false;
    }
    LocalDatabase *database = LocalDatabase::getInstance();
    json& serviceList = database->getJsonDatabase()["services"];
    for (json &element : serviceList){
        if(element["service_name"] == service){
            json instanceList = element["instances"];
            for (json &instance : instanceList){
                if(instance["server_name"] == server){
                    instance["address"] = address;
                    instance["port"] = port;
                    instance["heartbeat_time"] = heartbeatTime;
                    instance["status"] = "UP";
                    instance["role"] = role;
                    return true;
                }
            }
        }
    }
}



std::string LocalDatabaseDao::generateInstanceId(std::string &service, std::string &server, long long heartbeatTime)
{
    std::string instanceId = server + "-" + service + "-" + std::to_string(heartbeatTime);
    return instanceId;
}