#include "UDPConnector.h"

void connector(Config *config, int channel)
{
    // 设置端口
    int PORT;
    if (channel == SERVER_CHANNEL)
    {
        PORT = config->getServerPort();
        std::cout << "Server port: " << PORT << std::endl;
    }
    else if (channel == MANAGE_CHANNEL)
    {
        PORT = config->getManagePort();
        std::cout << "Manage port: " << PORT << std::endl;
    }
    else
    {
        throw std::invalid_argument("Invalid channel");
    }

    int sockfd;
    struct sockaddr_in register_center_addr;

    // 创建UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // 增大操作系统级别的接收缓冲区
    int rcvbuf_size = config->getCoreBufferSize(); // 默认设置为 4MB，或根据需要调整大小
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_size, sizeof(rcvbuf_size)) < 0)
    {
        perror("Failed to set socket receive buffer size");
    }

    // 初始化客户端地址结构
    memset(&register_center_addr, 0, sizeof(register_center_addr));
    register_center_addr.sin_family = AF_INET;

    // 自动选择ip
    register_center_addr.sin_addr.s_addr = INADDR_ANY;
    // 使用inet_addr将字符串转换为网络字节序的IPv4地址
    // 手动选择ip
    // register_center_addr.sin_addr.s_addr = inet_addr(connect_struct.ADDRESS);
    register_center_addr.sin_port = htons(PORT);
    // 绑定socket
    if (bind(sockfd, (struct sockaddr *)&register_center_addr, sizeof(register_center_addr)) < 0)
    {
        perror("Bind failed");
        close(sockfd);
        exit(2);
    }

    std::cout << "Registration Center is ready to receive broadcast messages..." << std::endl;
    std::cout << "Registration Center: " << config->getAddress() << " : " << PORT << std::endl;

    // 准备接收消息
    char *buffer = (char *)malloc(config->getReadBufferSize()); // 动态分配内存
    struct sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr);
    ssize_t received_len;

    while (1)
    {
        /**
         *
         * @brief 以阻塞的方式接收udp数据包
         *
         */
        // 清空 buffer，避免残留数据影响
        memset(buffer, 0, config->getReadBufferSize());
        // 阻塞接收数据
        received_len = recvfrom(sockfd, buffer, config->getReadBufferSize(), 0, (struct sockaddr *)&sender_addr, &addr_len);
        if (received_len < 0)
        {
            perror("Failed to receive message");
            continue;
        }

        buffer[received_len] = '\0'; // 确保字符串以NULL结尾

        // std::cout << "received_len: " << received_len << std::endl;
        // std::cout << "Received message:\n"
        //           << buffer << std::endl;

        /**
         *
         * @brief 解析dataStr中的数据
         *
         * @param dataStr 接收到的数据包
         *
         * @return 1. 返回Header对象
         * @return 2. 返回报文json对象
         *
         * @attention 报头是序列化后的数据(长度为28字节)，报文正文是json字符串
         *
         */
        std::string dataStr(buffer, received_len);
        std::string headerStr = dataStr.substr(0, 28);
        std::string contentStr = dataStr.substr(28);
        Header header = parseHeader(headerStr);
        std::cout << "received header: "<< std::endl << header.toString() << std::endl;
        std::cout << "received content: "<< std::endl << contentStr << std::endl;
        json contentJson = parseContent(contentStr);

        /**
         *
         * @brief 处理接收到的请求数据
         *
         */
        json responseContentJson;
        try
        {
            responseContentJson = handleRequest(header, contentJson);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cerr << "request content error" << '\n';
            std::cerr << "INFO: restart to receive information." << '\n';
            continue;
        }
        // 没有返回值，可能是收到了不需要处理的消息类型或者heartbeat，直接continue；
        if (responseContentJson.is_null())
        {
            continue;
        }

        /**
         *
         * @brief 封装responseContentJson，生成返回内容
         *
         * +
         * + 构造报头
         * @return std:string
         *
         */
        std::string responseMessage = formatResponse(header,responseContentJson);
        // 生成回复消息
        if (sendto(sockfd, responseMessage.c_str(), responseMessage.size(), 0, (struct sockaddr *)&sender_addr, addr_len) < 0)
        {
            perror("Failed to send response");
        }
        else
        {
            char temp_addr[20] = {0};
            struct in_addr addrptr = sender_addr.sin_addr;
            inet_ntop(AF_INET, &addrptr, temp_addr, sizeof temp_addr);
            std::cout << "Response sent to sender" << std::endl;
            std::cout << "IP: " << temp_addr << std::endl;
            std::cout << "PORT: " << ntohs(sender_addr.sin_port) << std::endl;
            std::string responseHeaderStr = responseMessage.substr(0, 28);
            std::string responseContentStr = responseMessage.substr(28);
            std::cout << "responseHeader:\n"
                      << parseHeader(responseHeaderStr).toString() << std::endl;
            std::cout << "responseContent:\n"
                      << responseContentStr << std::endl;
        }
    }

    free(buffer);
    close(sockfd);
}

json handleRequest(Header &header, json &content)
{
    json resJson;
    switch (header.type)
    {
    case 1:
        printf("mission: 1\n");
        try
        {
            resJson = RequestController::handleRegister(header, content);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "WARNNING: handleRegister error" << std::endl;
            throw;
        }
        break;
    case 3:
        printf("mission: 3\n");
        try
        {
            resJson = RequestController::handleMetaRegister(header, content);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "WARNNING: handleMetaRegister error" << std::endl;
            throw;
        }
        break;
    case 5:
        printf("mission: 5\n");
        try
        {
            resJson = RequestController::handleQuery(header, content);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "WARNNING: handleQuery error" << std::endl;
            throw;
        }
        break;
    case 7:
        printf("mission: 7\n");
        // processHeartbeat(database, data_buffer);
        try
        {
            resJson = RequestController::handleHeartbeat(header, content);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "WARNNING: handleHeartbeat error" << std::endl;
            throw;
        }
        break;
    default:
        printf("Received error message");
        // throw std::runtime_error("Received error message");
        break;
    }

    return resJson;
}

Header parseHeader(std::string &msg)
{
    try
    {
        return Header::deserialize(msg);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << "Header parse error" << std::endl;
    }
    return Header();
}
json parseContent(std::string &msg)
{
    try
    {
        return json::parse(msg);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << "Content parse error" << std::endl;
    }
    return json::object();
}

std::string formatResponse(Header resquestHeader, json content)
{
    std::string contentStr = content.dump(4);
    Header header = Header();
    header.identifier = 22;
    header.sendTime = Util::getCurrentTimeMillis();
    header.messageLength = contentStr.length();
    header.serialNumber = 1;
    header.checkBit = 1;
    header.type = TYPE_MAP[resquestHeader.type];
    std::string headerStr = Header::serialize(header);


    return headerStr + contentStr;
}
