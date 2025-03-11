#include "Config.h"

// 初始化静态成员变量
Config* Config::instance = nullptr;
// 配置文件路径
std::string Config::configFilePath = "";
// 仓库数据持久化间隔 /s
size_t Config::DATABASE_PERSISTENCE_INTERVAL = 5;
// 

size_t Config::READ_BUFFER_SIZE = 16;
size_t Config::CORE_BUFFER_SIZE = 1024;
size_t Config::READ_TIME_INTERTAL = 1;
size_t Config::HEARTBEAT_TIME_INTERTAL = 30;
size_t Config::STANDBY_HEARTBEAT_TIME_INTERTAL = 5;
size_t Config::SERVICE_INSTANCE_TIMEOUT = 90;
std::string Config::ADDRESS = "127.0.0.1";
size_t Config::SERVER_PORT = 8888;
size_t Config::MANAGE_PORT = 8889;
std::string Config::DATABASE_NAME = "config/database_test.json";
std::string Config::LOCKFILE = "config/standby.json";

// 私有构造函数
Config::Config(const std::string& filePath) {
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
    try {
        READ_BUFFER_SIZE = configData["READ_BUFFER_SIZE"];
    } catch (const std::exception& e) {
        std::cout << "config not exist READ_BUFFER_SIZE" << '\n';
    }
    try{
        CORE_BUFFER_SIZE = configData["CORE_BUFFER_SIZE"];
    }

            CORE_BUFFER_SIZE = configData["CORE_BUFFER_SIZE"];
        READ_TIME_INTERTAL = configData["READ_TIME_INTERTAL"];
        HEARTBEAT_TIME_INTERTAL = configData["HEARTBEAT_TIME_INTERTAL"];
        STANDBY_HEARTBEAT_TIME_INTERTAL = configData["STANDBY_HEARTBEAT_TIME_INTERTAL"];
        SERVICE_INSTANCE_TIMEOUT = configData["SERVICE_INSTANCE_TIMEOUT"];
        DATABASE_PERSISTENCE_INTERVAL = configData["DATABASE_PERSISTENCE_INTERVAL"];
        ADDRESS = configData["ADDRESS"];
        SERVER_PORT = configData["SERVER_PORT"];
        MANAGE_PORT = configData["MANAGE_PORT"];
        DATABASE_NAME = configData["DATABASE_NAME"];
}

// 获取单例实例的静态方法
Config* Config::getInstance(const std::string& filePath) {
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
    configData["READ_TIME_INTERTAL"] = READ_TIME_INTERTAL;
    configData["HEARTBEAT_TIME_INTERTAL"] = HEARTBEAT_TIME_INTERTAL;
    configData["STANDBY_HEARTBEAT_TIME_INTERTAL"] = STANDBY_HEARTBEAT_TIME_INTERTAL;
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
    return READ_TIME_INTERTAL;
}

void Config::setReadTimeInterval(size_t interval) {
    READ_TIME_INTERTAL = interval;
}

size_t Config::getHeartbeatTimeInterval() const {
    return HEARTBEAT_TIME_INTERTAL;
}

void Config::setHeartbeatTimeInterval(size_t interval) {
    HEARTBEAT_TIME_INTERTAL = interval;
}

size_t Config::getStandbyHeartbeatTimeInterval() const {
    return STANDBY_HEARTBEAT_TIME_INTERTAL;
}

void Config::setStandbyHeartbeatTimeInterval(size_t interval) {
    STANDBY_HEARTBEAT_TIME_INTERTAL = interval;
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

void Config::setAddress(const std::string& address) {
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

void Config::setDatabaseName(const std::string& name) {
    DATABASE_NAME = name;
}

std::string Config::getLockFile() const{
    return LOCKFILE;
}

void Config::setLockFile(const std::string& lockfile) {
    LOCKFILE = lockfile;
}
