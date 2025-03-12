#include "Header.h"

// 序列化报头
std::string Header::serialize(const HeaderData& header) {
    std::string serialized;
    serialized.resize(sizeof(HeaderData));

    // 转换为网络字节序
    int32_t identifierNet = htonl(header.identifier);
    int32_t messageLengthNet = htonl(header.messageLength);
    int32_t serialNumberNet = htonl(header.serialNumber);
    int32_t checkBitNet = htonl(header.checkBit);
    int32_t typeNet = htonl(header.type);

    // 对于 long long 类型，需要手动处理字节序
    long long sendTimeNet = (long long)htonl((uint32_t)(header.sendTime >> 32)) << 32 | htonl((uint32_t)header.sendTime);

    // 复制数据到序列化字符串
    memcpy(&serialized[0], &identifierNet, sizeof(int32_t));
    memcpy(&serialized[sizeof(int32_t)], &sendTimeNet, sizeof(long long));
    memcpy(&serialized[sizeof(int32_t) + sizeof(long long)], &messageLengthNet, sizeof(int32_t));
    memcpy(&serialized[sizeof(int32_t) * 2 + sizeof(long long)], &serialNumberNet, sizeof(int32_t));
    memcpy(&serialized[sizeof(int32_t) * 3 + sizeof(long long)], &checkBitNet, sizeof(int32_t));
    memcpy(&serialized[sizeof(int32_t) * 4 + sizeof(long long)], &typeNet, sizeof(int32_t));

    return serialized;
}

// 反序列化报头
Header::HeaderData Header::deserialize(const std::string& serialized) {
    HeaderData header;

    // 提取数据
    int32_t identifierNet;
    long long sendTimeNet;
    int32_t messageLengthNet;
    int32_t serialNumberNet;
    int32_t checkBitNet;
    int32_t typeNet;

    memcpy(&identifierNet, &serialized[0], sizeof(int32_t));
    memcpy(&sendTimeNet, &serialized[sizeof(int32_t)], sizeof(long long));
    memcpy(&messageLengthNet, &serialized[sizeof(int32_t) + sizeof(long long)], sizeof(int32_t));
    memcpy(&serialNumberNet, &serialized[sizeof(int32_t) * 2 + sizeof(long long)], sizeof(int32_t));
    memcpy(&checkBitNet, &serialized[sizeof(int32_t) * 3 + sizeof(long long)], sizeof(int32_t));
    memcpy(&typeNet, &serialized[sizeof(int32_t) * 4 + sizeof(long long)], sizeof(int32_t));

    // 转换回主机字节序
    header.identifier = ntohl(identifierNet);
    header.messageLength = ntohl(messageLengthNet);
    header.serialNumber = ntohl(serialNumberNet);
    header.checkBit = ntohl(checkBitNet);
    header.type = ntohl(typeNet);

    // 对于 long long 类型，需要手动处理字节序
    header.sendTime = (long long)ntohl((uint32_t)(sendTimeNet >> 32)) << 32 | ntohl((uint32_t)sendTimeNet);

    return header;
}
