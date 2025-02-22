#ifndef RESPONSESERVICE_H
#define RESPONSESERVICE_H

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class ResponseService {
private:
    json response;

public:
    // 构造函数
    ResponseService(const std::string& filePath);
    

};

#endif // RESPONSESERVICE_H