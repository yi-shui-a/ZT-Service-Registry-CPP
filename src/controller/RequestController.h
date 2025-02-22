#ifndef REQUESTCONTROLLER_H
#define REQUESTCONTROLLER_H

#include <iostream>
#include <fstream>
#include <string>

#include "../model/RequestInfo.h"
// #include "../model/ResponseInfo.h"
#include "../service/RequestService.h"
#include "../service/ResponseService.h"

#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;


/**
 * @brief 处理请求的控制器类
 * 
 * 所有方法均为静态方法
 * 接收值为request的json对象
 * 返回值是向服务器返回的一个json对象（不包含header）
 */

class RequestController {
// private:


public:
    static json handleRegister(RequestInfo request);
    static json handleMetaRegister(RequestInfo request);
    static json handleQuery(RequestInfo request);
    static json handleHeartbeat(RequestInfo request);
    
};

#endif // REQUESTCONTROLLER_H
