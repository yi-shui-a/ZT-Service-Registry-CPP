#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>
#include <unistd.h>          // 包含 close 函数的定义
#include <arpa/inet.h>       // For socket and network functions
#include <nlohmann/json.hpp> // For JSON parsing

#include "Header.h"

// 使用 nlohmann/json 库
using json = nlohmann::json;

json configData;

// UDP 广播函数
void udpBroadcastAndReceive(const std::string &message, const std::string &broadcastAddress, int target_port)
{
    // 创建UDP套接字
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        throw std::runtime_error("无法创建套接字");
    }

    // 设置套接字选项以允许广播
    int broadcastEnable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)))
    {
        close(sockfd);
        throw std::runtime_error("无法设置套接字选项以允许广播");
    }

    // 设置目标地址
    struct sockaddr_in targetAddr;
    memset(&targetAddr, 0, sizeof(targetAddr));
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_port = htons(target_port);
    if (inet_pton(AF_INET, broadcastAddress.c_str(), &targetAddr.sin_addr) <= 0)
    {
        close(sockfd);
        throw std::runtime_error("无效的广播地址");
    }

    // 发送广播消息
    ssize_t sentBytes = sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&targetAddr, sizeof(targetAddr));
    if (sentBytes < 0)
    {
        close(sockfd);
        throw std::runtime_error("发送广播消息失败");
    }

    // 设置超时以便在指定时间内等待响应
    struct timeval tv;
    tv.tv_sec = 5; // 5秒超时
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
        close(sockfd);
        throw std::runtime_error("无法设置接收超时");
    }

    // 接收单播响应
    char buffer[8 * 1024];
    struct sockaddr_in fromAddr;
    socklen_t fromAddrLen = sizeof(fromAddr);
    ssize_t recvBytes = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&fromAddr, &fromAddrLen);
    if (recvBytes < 0)
    {
        close(sockfd);
        throw std::runtime_error("接收响应失败或超时");
    }

    // 确保缓冲区以null结尾
    buffer[recvBytes] = '\0';

    // 将接收到的数据分为两部分
    const int prefixLength = 28; // 前28个字节
    if (recvBytes < prefixLength)
    {
        close(sockfd);
        throw std::runtime_error("接收到的数据不足28字节");
    }

    // 输出接收到的消息
    std::cout << "接收到来自 " << inet_ntoa(fromAddr.sin_addr) << ":" << ntohs(fromAddr.sin_port) << " 的消息: " << buffer << std::endl;
    std::cout << "已接收 " << recvBytes << " 字节数据" << std::endl;
    std::string responseMessage(buffer, recvBytes);
    std::string responseHeaderStr = responseMessage.substr(0, 28);
    std::string responseContentStr = responseMessage.substr(28);
    std::cout << "responseHeader:\n"
              << Header::deserialize(responseHeaderStr).toString() << std::endl;
    std::cout << "responseContet:\n"
              << responseContentStr << std::endl;

    // 关闭套接字
    close(sockfd);
}

// 从 JSON 文件读取数据
std::string readJsonFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open JSON file" << std::endl;
        return "";
    }

    // 解析 JSON 文件
    json jsonData;
    file >> jsonData;

    // 将 JSON 数据转换为字符串
    return jsonData.dump(); // dump() 返回 JSON 的字符串表示
}

// 获取当前毫秒级时间戳
long long getCurrentTimeMillis()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto res = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << "res: " << res << std::endl;
    return res;
}

std::string formatResponse(std::string contentStr)
{
    Header header = Header();
    header.identifier = 22;
    header.sendTime = getCurrentTimeMillis();
    header.messageLength = contentStr.length();
    header.serialNumber = 1;
    header.checkBit = 1;
    header.type = configData["header"]["type"];;
    std::string headerStr = Header::serialize(header);

    return headerStr + contentStr;
}

int main()
{
    // 读取配置 JSON 文件
    std::string configFilePath = "example.json"; // 配置文件的路径
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        std::cerr << "Error: Could not open config JSON file" << std::endl;
        return 1;
    }
    // 解析配置 JSON 文件configData
    configFile >> configData;

    // JSON 文件路径
    // std::string jsonFilePath = "../message_example/register.json";
    std::string jsonFilePath = configData["jsonFilePath"];

    // 从 JSON 文件读取数据
    std::string contentStr = readJsonFile(jsonFilePath);
    if (contentStr.empty())
    {
        std::cerr << "Error: JSON data is empty" << std::endl;
        return 1;
    }

    // 构造 string 数据
    std::string resString = formatResponse(contentStr);

    // 广播地址和端口
    std::string broadcastAddress = "255.255.255.255"; // 广播地址
    int port = 10450;                                 // 目标端口

    // 广播 JSON 数据并接收返回值
    udpBroadcastAndReceive(resString, broadcastAddress, port);

    return 0;
}