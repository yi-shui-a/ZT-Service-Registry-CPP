#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class Config {
private:
    // 私有构造函数
    Config(const std::string& filePath);

    // 静态实例指针
    static Config* instance;

    // 静态成员变量
    static std::string configFilePath;
    // 读取缓冲区大小
    static size_t READ_BUFFER_SIZE;
    //操作系统的读取缓冲区大小
    static size_t CORE_BUFFER_SIZE;
    static size_t READ_TIME_INTERVAL;
    static size_t HEARTBEAT_TIME_INTERVAL;
    // 主备检测的时间间隔
    static size_t STANDBY_HEARTBEAT_TIME_INTERVAL;
    // 微服务超时间隔
    static size_t SERVICE_INSTANCE_TIMEOUT;
    //数据库持久化间隔
    static size_t DATABASE_PERSISTENCE_INTERVAL;
    static std::string ADDRESS;
    static size_t SERVER_PORT;
    static size_t MANAGE_PORT;
    static std::string DATABASE_NAME;
    static std::string LOCKFILE;

public:
    // 删除拷贝构造函数和赋值运算符
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    // 获取单例实例的静态方法
    static Config* getInstance(const std::string& filePath);

    // 保存配置到文件
    void saveConfig();

    // Getter 和 Setter 方法
    size_t getReadBufferSize() const;
    void setReadBufferSize(size_t size);

    size_t getCoreBufferSize() const;
    void setCoreBufferSize(size_t size);

    size_t getReadTimeInterval() const;
    void setReadTimeInterval(size_t interval);

    size_t getHeartbeatTimeInterval() const;
    void setHeartbeatTimeInterval(size_t interval);

    size_t getStandbyHeartbeatTimeInterval() const;
    void setStandbyHeartbeatTimeInterval(size_t interval);

    size_t getServiceInstanceTimeout() const;
    void setServiceInstanceTimeout(size_t timeout);

    size_t getDatabasePersistenceInterval() const;
    void setDatabasePersistenceInterval(size_t interval);

    std::string getAddress() const;
    void setAddress(const std::string& address);

    size_t getServerPort() const;
    void setServerPort(size_t port);

    size_t getManagePort() const;
    void setManagePort(size_t port);

    std::string getDatabaseName() const;
    void setDatabaseName(const std::string& name);

    std::string getLockFile() const;
    void setLockFile(const std::string& name);
};

#endif // CONFIG_H