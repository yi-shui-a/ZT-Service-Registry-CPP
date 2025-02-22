#ifndef RESPONSESERVICE_H
#define RESPONSESERVICE_H

#include <iostream>
#include <fstream>
#include <string>

#include "../model/ResponseInfo.h"

#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class ResponseService {
private:
    ResponseInfo response;

public:
    // 构造函数
    ResponseService(ResponseInfo response);
    

};

#endif // RESPONSESERVICE_H