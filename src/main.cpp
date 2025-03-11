#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#include "config/ConfigUtil.cpp"
#include "config/Config.h"
#include "com/UDPConnector.h"
#include "utils/Util.h"
// #include "utils/DBUtil.h"
#include "service/DBService.h"
// #include "UDPConnector.h"
// #include "JsonProcess.h"

using json = nlohmann::json;

// 获取设备锁
void getMainLock(Config *config);
void monitorDatabase(Config *config, DBService dBService);

int main(int argc, char *argv[])
{
    // 获取配置文件路径
    std::string configPath;
    try
    {
        // 先判断json中是否存在configPath属性
        if (!ConfigUtil::parsePathFromArgs(argc, argv).contains("configPath"))
        {
            configPath = "config.json";
        }
        else
        {
            configPath = ConfigUtil::parsePathFromArgs(argc, argv)["configPath"];
        }
        std::cout << "got configPath!!!" << std::endl
                  << "configPath: " << configPath << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << "configPath load fail" << '\n';
        exit(EXIT_FAILURE);
    }

    // 加载配置文件，设置全局变量。
    Config *config;
    try
    {

        // 获取 Config 单例实例
        config = Config::getInstance(configPath);
        // 获取设备锁
        // 如果能获取，作为主份持续运行;如果不能获取，作为备份，持续心跳，等待成为主份
        getMainLock(config);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "config load error" << std::endl;
        return EXIT_FAILURE;
    }

    // 定义一个全局的数据库变量
    DBService dBService(config->getDatabaseName());
    // 清空数据库
    // Util::clearFile(config->getDatabaseName());

    // 创建各个线程和数据库线程，开始运行
    try
    {
        std::thread thread_server(&UDPconnector, config, 1);
        std::cout << "thread_server created successfully. Continuing program execution." << std::endl;
        std::thread thread_manage(&UDPconnector, config, 2);
        std::cout << "thread_manage created successfully. Continuing program execution." << std::endl;

        // 定时进行数据监控
        monitorDatabase(config, dBService);

        // 线程结束
        thread_server.join();
        thread_manage.join();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << "thread create error" << '\n';
        return EXIT_FAILURE;
    }
}

void monitorDatabase(Config *config, DBService dBService)
{
    int temp_array[2] = {config->getDatabasePersistenceInterval(), config->getServiceInstanceTimeout()};
    int check_interval = Util::gcdMultiple(temp_array, 2);
    time_t start_time;
    time(&start_time);
    size_t database_persistence_interval_count = 0;
    size_t service_instance_time_count = 0;

    while (true)
    {
        sleep(check_interval);
        time_t current_time;
        time(&current_time);
        if ((current_time - start_time) / config->getDatabasePersistenceInterval() >= database_persistence_interval_count)
        {
            std::cout << "finished save database" << std::endl;
            dBService.saveDatabase();
            database_persistence_interval_count += 1;
        }
        if ((current_time - start_time) >= 5)
        {
            dBService.updateServiceInstanceStatus(current_time, config->getServiceInstanceTimeout());
            service_instance_time_count += 1;
        }
    }
}

int try_lock_file(const std::string &lockfile)
{
    int fd = open(lockfile.c_str(), O_CREAT | O_RDWR, 0666);
    if (fd < 0)
    {
        throw std::runtime_error("open lockfile error");
    }

    struct flock fl;
    std::memset(&fl, 0, sizeof(fl));
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;

    if (fcntl(fd, F_SETLK, &fl) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            close(fd);
            return 0;
        }
        else
        {
            close(fd);
            throw std::runtime_error("fcntl error");
        }
    }

    return fd;
}

void unlock_file(int fd)
{
    struct flock fl;
    std::memset(&fl, 0, sizeof(fl));
    fl.l_type = F_UNLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;

    fcntl(fd, F_SETLK, &fl);
    close(fd);
}

void getMainLock(Config *config)
{
    std::string lockfile = config->getLockFile();
    time_t STANDBY_HEARTBEAT_TIME_INTERTAL = config->getStandbyHeartbeatTimeInterval();
    int fd;

    while (true)
    {
        fd = try_lock_file(lockfile);
        if (fd > 0)
        {
            std::cout << "[INFO] " << config->getAddress() << " Became PRIMARY." << std::endl;
            break;
        }
        else if (fd == 0)
        {
            std::cout << "[INFO] " << config->getAddress() << " Became BACKUP, monitoring MAIN." << std::endl;
        }
        else
        {
            std::cout << "[ERROR] " << config->getAddress() << " Failed to check lock." << std::endl;
        }

        sleep(STANDBY_HEARTBEAT_TIME_INTERTAL);
        fd = try_lock_file(lockfile);
        if (fd > 0)
        {
            std::cout << "[INFO] PRIMARY server timeout, " << config->getAddress() << " becoming PRIMARY." << std::endl;
            break;
        }
    }
}
