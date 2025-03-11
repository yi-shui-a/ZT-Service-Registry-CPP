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
#include "../model/RequestInfo.h"
#include "../model/ResponseInfo.h"
#include "../controller/RequestController.h"
// #include "../service/RequestService.h"
// #include "../service/ResponseService.h"


// 使用 nlohmann/json 库
using json = nlohmann::json;

int SERVER_CHANNEL = 1;
int MANAGE_CHANNEL = 2;


void UDPconnector(Config* config, int channel);
// void receive_server(void *arg);





# endif