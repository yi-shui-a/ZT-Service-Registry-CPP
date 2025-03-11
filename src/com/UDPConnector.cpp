#include "UDPConnector.h"

void UDPconnector(Config *config, int channel)
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

        std::cout << "received_len: " << received_len << std::endl;
        std::cout << "Received message:\n"
                  << buffer << std::endl;

        // 将接收到的数据存储到std::string中
        std::string dataStr(buffer, received_len);
        // 处理接收到的请求数据
        std::string responseMessage;
        try
        {
            ResponseInfo responseInfo = handleRequest(dataStr);
            // 如果ResponseInfo为空，不需要回复消息
            if (responseInfo.isEmpty())
            {
                continue;
            }
            else
            {
                responseMessage = responseInfo.exportResponse();
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cerr << "INFO: restart to receive information." << '\n';
            continue;
        }

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
            std::cout << "response_message:\n"
                      << responseMessage << std::endl;
        }
    }

    free(buffer);
    close(sockfd);
}

ResponseInfo handleRequest(std::string dataStr)
{
    // 将dataBuffer转换为json对象
    json dataJson;
    try
    {
        dataJson = json::parse(dataStr);
    }
    catch (json::parse_error &e)
    {
        std::cerr << "Parse error: " << e.what() << std::endl;
        throw std::runtime_error("Parse error");
    }

    // 将json转为RequestInfo对象
    RequestInfo requestInfo = RequestInfo(dataJson);

    json resJson;
    switch (requestInfo.getRequestType())
    {
    case 1:
        printf("mission: 1\n");
        // strcpy(response_message, processRegisterMessage(database, data_buffer));
        try
        {
            resJson = RequestController::handleRegister(requestInfo);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "WARNNING: handleRegister error" << std::endl;
            throw;
        }
        break;
    case 3:
        printf("mission: 3\n");
        // strcpy(response_message, processMetaRegisterMessage(database, data_buffer));
        try
        {
            resJson = RequestController::handleMetaRegister(requestInfo);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "WARNNING: handleMetaRegister error" << std::endl;
            throw;
        }
        break;
    case 5:
        printf("mission: 5\n");
        // strcpy(response_message, processQuery(database, data_buffer));
        try
        {
            resJson = RequestController::handleQuery(requestInfo);
        }
        catch(const std::exception& e)
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
            resJson = RequestController::handleHeartbeat(requestInfo);
        }
        catch(const std::exception& e)
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

    // 构造ResponseInfo变量
    ResponseInfo responseInfo(resJson);

    return responseInfo;
}

// uint8_t getRequestType(json data)
// {
//     if (!data.contains("header"))
//     {
//         throw std::runtime_error("request header is not exist");
//     }
//     if (!data["header"].contains("type"))
//     {
//         throw std::runtime_error("request header's type is not exist");
//     }

//     return data["header"]["type"];
// }