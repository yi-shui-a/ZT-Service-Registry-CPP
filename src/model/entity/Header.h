#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <string>
#include <nlohmann/json.hpp>

// 使用 nlohmann::json 库进行 JSON 处理
using json = nlohmann::json;

class Header {
public:
    // 报头结构体
    struct HeaderData {
        int32_t identifier;
        long long sendTime;
        int32_t messageLength;
        int32_t serialNumber;
        int32_t checkBit;
        int32_t type;
    };

    // 序列化报头
    static std::string serialize(const HeaderData& header);

    // 反序列化报头
    static HeaderData deserialize(const std::string& serialized);
};

#endif // HEADER_H
