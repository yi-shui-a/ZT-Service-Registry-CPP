#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class DBService {
private:
    std::string databasePath;
    json database;

public:
    // 构造函数
    DBService(const std::string& filePath);
    // 保存数据库到文件
    void saveDatabase();
    // 清空持久化数据库
    void clearPersistenceDatabase();
    // 清空内存数据库
    void clearDatabase();
    
    // 更新服务实例状态
    void updateServiceInstanceStatus(time_t cur_time,size_t timeoutIntertal);

};

#endif // DBSERVICE_H