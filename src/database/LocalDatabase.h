#ifndef LOCALDATABASE_H
#define LOCALDATABASE_H

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

// 使用别名简化代码
using json = nlohmann::json;

// 封装 nlohmann/json 操作的类
class LocalDatabase {
private:
// 静态实例指针
    static LocalDatabase* instance; 
    json database;
    // 定时器线程
    std::thread timerThread;

    // 私有构造函数，防止外部实例化
    // LocalDatabase() = default;
    LocalDatabase(const std::string& filePath = "");
    // 拷贝构造函数和赋值运算符私有化，防止拷贝
    LocalDatabase(const LocalDatabase&) = delete;
    LocalDatabase& operator=(const LocalDatabase&) = delete;
    // 定时器函数
    void startTimer();
    // 初始化数据库函数
    void initDatabase();

public:
    // 获取单例实例的静态方法
    static LocalDatabase* getInstance(const std::string& filePath = "");

    // 获取 JSON 对象
    json getJsonDatabase() const;

    // 设置 JSON 对象
    void setJsonDatabase(const json& obj);

    // 获取 JSON 字符串
    std::string getJsonString() const;

    // 设置 JSON 字符串
    void setJsonString(const std::string& jsonStr);

    // 保存 JSON 数据到文件
    bool saveToFile(const std::string& filePath) const;

    // 获取 JSON 对象中的值
    template<typename T>
    T getValue(const std::string& key) const {
        if (database.contains(key)) {
            return database[key].get<T>();
        }
        return T();
    }

    // 设置 JSON 对象中的值
    template<typename T>
    void setValue(const std::string& key, const T& value) {
        database[key] = value;
    }

    // 获取多重 JSON 对象中的值
    template<typename T>
    T getNestedValue(const std::vector<std::string>& keys) const {
        json current = database;
        for (const auto& key : keys) {
            if (current.contains(key)) {
                current = current[key];
            } else {
                return T();
            }
        }
        return current.get<T>();
    }

    // 设置多重 JSON 对象中的值
    template<typename T>
    void setNestedValue(const std::vector<std::string>& keys, const T& value) {
        json* current = &database;
        for (size_t i = 0; i < keys.size() - 1; ++i) {
            const auto& key = keys[i];
            if (!current->contains(key)) {
                (*current)[key] = json::object();
            }
            current = &(*current)[key];
        }
        (*current)[keys.back()] = value;
    }


};

#endif // LOCALDATABASE_H