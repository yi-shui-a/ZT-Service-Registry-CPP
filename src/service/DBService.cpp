#include "DBService.h"

DBService::DBService(const std::string &filePath) : databasePath(filePath)
{

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        database["services"] = json::array();
    }

    file.seekg(0, std::ios::end);
    size_t filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string buffer(filesize, '\0');
    file.read(&buffer[0], filesize);
    file.close();

    json database = json::parse(buffer);
    if (!database.contains("services"))
    {
        database["services"] = json::array();
    }

    std::cout << "database loaded!!!" << std::endl;
}

void DBService::saveDatabase()
{
    std::string database_str = database.dump(4);
    std::cout << "save database:\n"
              << database_str << std::endl;

    std::ofstream file(databasePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file for writing");
    }

    file << database_str;
    file.close();
    std::cout << "SUCCESS: saved  database finishes" << std::endl;
}

void DBService::clearPersistenceDatabase()
{
    std::ofstream file(databasePath, std::ios::trunc);
    if (file.is_open())
    {
        // 文件成功打开并已清空内容
        file.close();
        std::cout << "The file has been successfully opened and its contents have been cleared" << std::endl;
    }
    else
    {
        std::cerr << "can not open file: " << databasePath << std::endl;
    }
}

void DBService::clearDatabase()
{
    database.clear();
    std::cout << "database have been cleared" << std::endl;
}

void DBService::updateServiceInstanceStatus(time_t cur_time,size_t timeoutIntertal)
{
    if (!database.contains("services"))
    {
        std::cout << "database have not service" << std::endl;
    }
    for (auto &service : database["services"])
    {
        if (service.contains("instances"))
        {
            for (auto &instance : service["instances"])
            {
                if (instance.contains("status"))
                {
                    std::string status = instance["status"];
                    if(cur_time - instance["heartbeat_time"] > timeoutIntertal){
                        instance["status"] = "SHUTUP";
                    }else{
                        instance["status"] = "UP";
                    }
                }
            }
        }
    }
}