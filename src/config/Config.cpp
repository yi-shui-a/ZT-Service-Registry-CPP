#include "Config.h"

// 初始化静态成员变量
Config *Config::instance = nullptr;
// 配置文件路径
std::string Config::configFilePath = "";
// 仓库数据持久化间隔 /ms
size_t Config::DATABASE_PERSISTENCE_INTERVAL = 5 * 60;
// 读取缓冲区大小 /B
size_t Config::READ_BUFFER_SIZE = 16 * 1024;
// 核心缓冲区大小 /B
size_t Config::CORE_BUFFER_SIZE = 4 * 1024 *1024;
// 读取时间间隔 /ms
size_t Config::READ_TIME_INTERVAL = 1 * 60;
// 心跳时间间隔 /ms
size_t Config::HEARTBEAT_TIME_INTERVAL = 30 *60;
// 主备心跳时间间隔 /ms
size_t Config::STANDBY_HEARTBEAT_TIME_INTERVAL = 5 * 60;
// 服务实例超时时间 /ms
size_t Config::SERVICE_INSTANCE_TIMEOUT = 90 *60;
// 地址
std::string Config::ADDRESS = "127.0.0.1";
size_t Config::SERVER_PORT = 10450;
size_t Config::MANAGE_PORT = 10451;
std::string Config::DATABASE_NAME = "config/database_test.json";
std::string Config::LOCKFILE = "config/standby.json";

// 私有构造函数
Config::Config(const std::string &filePath) {
    configFilePath = filePath;
    json configData;
    std::ifstream file(filePath);
    if (file.is_open()) {
        file >> configData;
        file.close();
    } else {
        std::cout << "Failed to open config file: " << filePath << std::endl;
        std::cout << "SUCCESS: config created" << std::endl;
        return;
    }
    if (configData.contains("READ_BUFFER_SIZE")) {
        READ_BUFFER_SIZE = configData["READ_BUFFER_SIZE"];
    }
    if (configData.contains("CORE_BUFFER_SIZE")) {
        CORE_BUFFER_SIZE = configData["CORE_BUFFER_SIZE"];
    }
    if (configData.contains("READ_TIME_INTERVAL")) {
        READ_TIME_INTERVAL = configData["READ_TIME_INTERVAL"];
    }
    if (configData.contains("HEARTBEAT_TIME_INTERVAL")) {
        HEARTBEAT_TIME_INTERVAL = configData["HEARTBEAT_TIME_INTERVAL"];
    }
    if (configData.contains("STANDBY_HEARTBEAT_TIME_INTERVAL")) {
        STANDBY_HEARTBEAT_TIME_INTERVAL = configData["STANDBY_HEARTBEAT_TIME_INTERVAL"];
    }
    if (configData.contains("SERVICE_INSTANCE_TIMEOUT")) {
        SERVICE_INSTANCE_TIMEOUT = configData["SERVICE_INSTANCE_TIMEOUT"];
    }
    if (configData.contains("DATABASE_PERSISTENCE_INTERVAL")) {
        DATABASE_PERSISTENCE_INTERVAL = configData["DATABASE_PERSISTENCE_INTERVAL"];
    }
    if (configData.contains("ADDRESS")) {
        ADDRESS = configData["ADDRESS"];
    }
    if (configData.contains("SERVER_PORT")) {
        SERVER_PORT = configData["SERVER_PORT"];
    }
    if (configData.contains("MANAGE_PORT")) {
        MANAGE_PORT = configData["MANAGE_PORT"];
    }
    if (configData.contains("DATABASE_NAME")) {
        DATABASE_NAME = configData["DATABASE_NAME"];
    }
}

// 获取单例实例的静态方法
Config *Config::getInstance(const std::string &filePath) {
    if (instance == nullptr) {
        instance = new Config(filePath);
    }
    return instance;
}

// 保存配置到文件
void Config::saveConfig() {
    json configData;
    configData["READ_BUFFER_SIZE"] = READ_BUFFER_SIZE;
    configData["CORE_BUFFER_SIZE"] = CORE_BUFFER_SIZE;
    configData["READ_TIME_INTERVAL"] = READ_TIME_INTERVAL;
    configData["HEARTBEAT_TIME_INTERVAL"] = HEARTBEAT_TIME_INTERVAL;
    configData["STANDBY_HEARTBEAT_TIME_INTERVAL"] = STANDBY_HEARTBEAT_TIME_INTERVAL;
    configData["SERVICE_INSTANCE_TIMEOUT"] = SERVICE_INSTANCE_TIMEOUT;
    configData["DATABASE_PERSISTENCE_INTERVAL"] = DATABASE_PERSISTENCE_INTERVAL;
    configData["ADDRESS"] = ADDRESS;
    configData["SERVER_PORT"] = SERVER_PORT;
    configData["MANAGE_PORT"] = MANAGE_PORT;
    configData["DATABASE_NAME"] = DATABASE_NAME;

    std::ofstream file(configFilePath);
    if (file.is_open()) {
        file << configData.dump(4);
        file.close();
    } else {
        std::cerr << "Failed to save config file: " << configFilePath << std::endl;
    }
}

// Getter 和 Setter 方法
size_t Config::getReadBufferSize() const {
    return READ_BUFFER_SIZE;
}

void Config::setReadBufferSize(size_t size) {
    READ_BUFFER_SIZE = size;
}

size_t Config::getCoreBufferSize() const {
    return CORE_BUFFER_SIZE;
}

void Config::setCoreBufferSize(size_t size) {
    CORE_BUFFER_SIZE = size;
}

size_t Config::getReadTimeInterval() const {
    return READ_TIME_INTERVAL;
}

void Config::setReadTimeInterval(size_t interval) {
    READ_TIME_INTERVAL = interval;
}

size_t Config::getHeartbeatTimeInterval() const {
    return HEARTBEAT_TIME_INTERVAL;
}

void Config::setHeartbeatTimeInterval(size_t interval) {
    HEARTBEAT_TIME_INTERVAL = interval;
}

size_t Config::getStandbyHeartbeatTimeInterval() const {
    return STANDBY_HEARTBEAT_TIME_INTERVAL;
}

void Config::setStandbyHeartbeatTimeInterval(size_t interval) {
    STANDBY_HEARTBEAT_TIME_INTERVAL = interval;
}

size_t Config::getServiceInstanceTimeout() const {
    return SERVICE_INSTANCE_TIMEOUT;
}

void Config::setServiceInstanceTimeout(size_t timeout) {
    SERVICE_INSTANCE_TIMEOUT = timeout;
}

size_t Config::getDatabasePersistenceInterval() const {
    return DATABASE_PERSISTENCE_INTERVAL;
}

void Config::setDatabasePersistenceInterval(size_t interval) {
    DATABASE_PERSISTENCE_INTERVAL = interval;
}


std::string Config::getAddress() const {
    return ADDRESS;
}

void Config::setAddress(const std::string &address) {
    ADDRESS = address;
}

size_t Config::getServerPort() const {
    return SERVER_PORT;
}

void Config::setServerPort(size_t port) {
    SERVER_PORT = port;
}

size_t Config::getManagePort() const {
    return MANAGE_PORT;
}

void Config::setManagePort(size_t port) {
    MANAGE_PORT = port;
}

std::string Config::getDatabaseName() const {
    return DATABASE_NAME;
}

void Config::setDatabaseName(const std::string &name) {
    DATABASE_NAME = name;
}

std::string Config::getLockFile() const {
    return LOCKFILE;
}

void Config::setLockFile(const std::string &lockfile) {
    LOCKFILE = lockfile;
}
