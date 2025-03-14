#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <arpa/inet.h> // For socket and network functions
#include <nlohmann/json.hpp> // For JSON parsing

// 使用 nlohmann/json 库
using json = nlohmann::json;

// UDP 广播函数
void udpBroadcast(const std::string& message, const std::string& broadcastAddress, int port) {
    // 创建 UDP 套接字
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error: Could not create socket" << std::endl;
        return;
    }

    // 设置套接字为广播模式
    int broadcastEnable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
        std::cerr << "Error: Could not set socket to broadcast mode" << std::endl;
        close(sockfd);
        return;
    }

    // 设置目标地址
    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(port); // 目标端口
    inet_pton(AF_INET, broadcastAddress.c_str(), &destAddr.sin_addr); // 广播地址

    // 发送数据
    ssize_t bytesSent = sendto(sockfd, message.c_str(), message.size(), 0,
                               (struct sockaddr*)&destAddr, sizeof(destAddr));
    if (bytesSent < 0) {
        std::cerr << "Error: Failed to send data" << std::endl;
    } else {
        std::cout << "Broadcasted message: " << message << std::endl;
    }

    // 关闭套接字
    close(sockfd);
}

// 从 JSON 文件读取数据
std::string readJsonFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open JSON file" << std::endl;
        return "";
    }

    // 解析 JSON 文件
    json jsonData;
    file >> jsonData;

    // 将 JSON 数据转换为字符串
    return jsonData.dump(); // dump() 返回 JSON 的字符串表示
}

int main() {
    // JSON 文件路径
    std::string jsonFilePath = "data.json";

    // 从 JSON 文件读取数据
    std::string jsonString = readJsonFile(jsonFilePath);
    if (jsonString.empty()) {
        std::cerr << "Error: JSON data is empty" << std::endl;
        return 1;
    }

    // 广播地址和端口
    std::string broadcastAddress = "255.255.255.255"; // 广播地址
    int port = 12345; // 目标端口

    // 广播 JSON 数据
    udpBroadcast(jsonString, broadcastAddress, port);

    return 0;
}