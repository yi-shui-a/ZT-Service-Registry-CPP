#ifndef UDPCONNECTOR_H
#define UDPCONNECTOR_H


#include <iostream>
#include <fstream>
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <nlohmann/json.hpp>
#include "../config/Config.h"
#include "../model/entity/Header.h"
#include "../controller/RequestController.h"
// #include "../service/RequestService.h"
// #include "../service/ResponseService.h"


// 使用 nlohmann/json 库
using json = nlohmann::json;

int SERVER_CHANNEL = 1;
int MANAGE_CHANNEL = 2;

// 通信函数
void connector(Config* config, int channel);

// 解析请求消息
Header parseHeader(std::string& msg);
json parseContent(std::string& msg);

// 处理请求消息
json handleRequest(Header& header,json& content);
// 封装返回消息
std::string generateResponse(json content); 

// void receive_server(void *arg);





# endif