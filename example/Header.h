#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>

#include <nlohmann/json.hpp>

// 使用 nlohmann::json 库进行 JSON 处理
using json = nlohmann::json;

/**
 * @brief 报头类
 *
 * 用于序列化和反序列化报头数据。
 * 报头长度为 28 字节。
 */
class Header
{
public:
    // 报头结构体
    int32_t identifier;
    long long sendTime;
    int32_t messageLength;
    int32_t serialNumber;
    int32_t checkBit;
    int32_t type;

    // 打印数据
    std::string toString();

    // 序列化报头
    static std::string serialize(const Header &header);

    // 反序列化报头
    static Header deserialize(const std::string &serialized);

    static long long htonll(long long value);

    static long long ntohll(long long value);
};

#endif // HEADER_H
