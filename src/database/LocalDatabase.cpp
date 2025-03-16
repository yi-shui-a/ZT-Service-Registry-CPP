#include "LocalDatabase.h"

// 初始化静态实例指针
LocalDatabase *LocalDatabase::instance = nullptr;

// 获取单例实例的静态方法
LocalDatabase *LocalDatabase::getInstance(const std::string &filePath)
{
    if (instance == nullptr)
    {
        instance = new LocalDatabase(filePath);
        // // 检查字符串长度是否为0, 如果是，使用默认构造函数
        // if (filePath.empty()) {
        //     // 如果没有传入 filePath，使用默认构造函数（这里假设需要修改默认构造函数以支持文件路径）
        //     instance = new LocalDatabase();
        // } else {
        //     instance = new LocalDatabase(filePath);
        // }
    }
    return instance;
}

// 从 JSON 字符串初始化
LocalDatabase::LocalDatabase(const std::string &filePath)
{
    // 检查字符串长度是否为0, 如果是，使用默认构造函数
    if (filePath.empty())
    {
        initDatabase();
    }
    else
    {
        std::ifstream file(filePath);
        if (file.is_open())
        {
            // 文件存在，读取并解析
            try
            {
                file >> database;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Failed to parse JSON file: " << e.what() << std::endl;
                initDatabase();
            }
            file.close();

            // file >> database;
            // file.close();
        }
        else
        {
            // 文件不存在，创建新的 JSON 对象
            initDatabase();
        }
    }
    // 启动定时器
    startTimer();
}

// 定时器函数
void LocalDatabase::startTimer()
{
    timerThread = std::thread([this]()
                              {
        while (true) {
            // 模拟每 10 秒执行一次操作
            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
            // 在这里对 database 执行相关操作
            std::cout << "Current database: \n" << database.dump(4) << std::endl;
        } });
    timerThread.detach();
}

void LocalDatabase::initDatabase()
{
    database = json::object();
    json services = json::array();
    database["services"] = services;
}

// 获取 JSON 对象
json& LocalDatabase::getJsonDatabase()
{
    return database;
}

// 设置 JSON 对象
void LocalDatabase::setJsonDatabase(const json &obj)
{
    database = obj;
}

// 获取 JSON 字符串
std::string LocalDatabase::getJsonString() const
{
    return database.dump(4);
}

// 设置 JSON 字符串
void LocalDatabase::setJsonString(const std::string &jsonStr)
{
    database = json::parse(jsonStr);
}

// 保存 JSON 数据到文件
bool LocalDatabase::saveToFile(const std::string &filePath) const
{
    std::ofstream file(filePath);
    if (file.is_open())
    {
        file << database.dump(4); // 4 表示缩进为 4 个空格
        file.close();
        return true;
    }
    return false;
}
