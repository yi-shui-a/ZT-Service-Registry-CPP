#ifndef DBUTIL_H
#define DBUTIL_H

#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class DBUtil {
public:
    // 初始化数据库
    // static json initialDatabase(const std::string &filepath);

    // 保存数据库
    // static int saveDatabase(const json &database, const std::string &filename);
};

#endif // DBUTIL_H