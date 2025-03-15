#ifndef REQUESTCONTROLLER_H
#define REQUESTCONTROLLER_H

#include <iostream>
#include <fstream>
#include <string>

#include "../model/entity/Header.h"
#include "../service/Service.h"
#include "../service/RegisterService.h"

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
    static json handleService(Service& service, Header &header, json &content);

    static json handleRegister(Header& header,json& content);
    static json handleMetaRegister(Header& header,json& content);
    static json handleQuery(Header& header,json& content);
    static json handleHeartbeat(Header& header,json& content);
    
};

#endif // REQUESTCONTROLLER_H
