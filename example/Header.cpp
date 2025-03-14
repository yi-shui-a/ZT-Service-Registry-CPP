#include "Header.h"

// 序列化报头
std::string Header::serialize(const Header& header) {
    std::stringstream ss;

    // 将各个字段转换为网络字节序（大端序）
    int32_t identifier = htonl(header.identifier);
    int32_t messageLength = htonl(header.messageLength);
    int32_t serialNumber = htonl(header.serialNumber);
    int32_t checkBit = htonl(header.checkBit);
    int32_t type = htonl(header.type);

    // 将 long long 类型的 sendTime 转换为网络字节序
    long long sendTime = header.sendTime;
    sendTime = htonll(sendTime); // 需要自定义 htonll 函数

    // 将各个字段写入字符串流
    ss.write(reinterpret_cast<const char*>(&identifier), sizeof(identifier));
    ss.write(reinterpret_cast<const char*>(&sendTime), sizeof(sendTime));
    ss.write(reinterpret_cast<const char*>(&messageLength), sizeof(messageLength));
    ss.write(reinterpret_cast<const char*>(&serialNumber), sizeof(serialNumber));
    ss.write(reinterpret_cast<const char*>(&checkBit), sizeof(checkBit));
    ss.write(reinterpret_cast<const char*>(&type), sizeof(type));

    // 返回序列化后的字符串
    return ss.str();
}

// 反序列化报头
Header Header::deserialize(const std::string& serialized) {
    Header header;
    std::stringstream ss(serialized);

    // 从字符串流中读取各个字段
    ss.read(reinterpret_cast<char*>(&header.identifier), sizeof(header.identifier));
    ss.read(reinterpret_cast<char*>(&header.sendTime), sizeof(header.sendTime));
    ss.read(reinterpret_cast<char*>(&header.messageLength), sizeof(header.messageLength));
    ss.read(reinterpret_cast<char*>(&header.serialNumber), sizeof(header.serialNumber));
    ss.read(reinterpret_cast<char*>(&header.checkBit), sizeof(header.checkBit));
    ss.read(reinterpret_cast<char*>(&header.type), sizeof(header.type));

    // 将各个字段转换为主机字节序（小端序）
    header.identifier = ntohl(header.identifier);
    header.messageLength = ntohl(header.messageLength);
    header.serialNumber = ntohl(header.serialNumber);
    header.checkBit = ntohl(header.checkBit);
    header.type = ntohl(header.type);

    // 将 long long 类型的 sendTime 转换为主机字节序
    header.sendTime = ntohll(header.sendTime); // 需要自定义 ntohll 函数

    return header;
}

// 自定义 htonll 和 ntohll 函数（用于处理 long long 类型的字节序转换）
long long Header::htonll(long long value) {
    if (htonl(1) == 1) {
        return value; // 大端序系统，无需转换
    }
    // 小端序系统，需要转换
    char* ptr = reinterpret_cast<char*>(&value);
    std::reverse(ptr, ptr + sizeof(value));
    return value;
}

long long Header::ntohll(long long value) {
    return htonll(value); // 与 htonll 相同
}

std::string Header::toString(){
    json data;
    data["identifier"] = identifier;
    data["sendTime"] = sendTime;
    data["messageLength"] = messageLength;
    data["serialNumber"] = serialNumber;
    data["checkBit"] = checkBit;
    data["type"] = type;
    return data.dump(4);
}