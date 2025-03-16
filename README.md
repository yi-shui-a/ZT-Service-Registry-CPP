# 项目概述

ZT-Service-Registry-CPP 是一个用 C++ 编写的服务注册中心项目，它提供了服务的注册、元数据管理、查询、心跳检测等功能。该项目支持多线程处理，可配置多个服务实例，并且具备主备切换机制。



## 目录结构

```plainText
ZT-Service-Registry-CPP/
├── config/
│   ├── config.json
│   ├── config_example_all.json.json
│   ├── database_test.json
│   ├── database_example_all.json
│   └── standby.json
├── example/
├── message_example/
│   ├── metadata.json
│   └── matadata_response.json
├── src/
│   ├── main.cpp
│   ├── com/
│   │   ├── UDPConnector.cpp
│   │   └── UDPConnector.h
│   ├── config/
│   │   ├── Config.cpp
│   │   ├── Config.h
│   │   └── ConfigUtil.cpp
│   ├── constant/
│   │   ├── globals.cpp
│   │   └── globals.h
│   ├── controller/
│   │   ├── RequestController.cpp
│   │   └── RequestController.h
│   ├── dao/
│   │   ├── LocalDatabaseDao.cpp
│   │   └── LocalDatabaseDao.h
│   ├── database/
│   │   ├── LocalDatabase.cpp
│   │   └── LocalDatabase.h
│   ├── model/
│   │   └── entity/
│   │       ├── Header.cpp
│   │       └── Header.h
│   ├── service/
│   │   ├── HeartbeatService.cpp
│   │   ├── HeartbeatService.h
│   │   ├── MetadataService.cpp
│   │   ├── MetadataService.h
│   │   ├── QueryService.cpp
│   │   ├── QueryService.h
│   │   ├── RegisterService.cpp
│   │   ├── RegisterService.h
│   │   ├── Service.cpp
│   │   └── Service.h
│   └── utils/
│       ├── Util.cpp
│       └── Util.h
├── build.sh
├── launch.sh
├── CMakeLists.txt
└── README.md
```

## 目录介绍

### `config/` 文件夹

- `config_example_all.json`：项目的主配置文件，包含了项目运行所需的各种参数，如缓冲区大小、时间间隔、地址、端口、数据库名称等。
- `config.json`：目前使用的配置文件，用于测试配置。
- `database_example_all.json`：数据库相关的标准示例文件，包含数据库的表结构等。
- `database_test.json`：用于测试数据库的文件，方便在开发和测试过程中使用不同的数据库。
- `standby.json`：可能与项目的主备切换机制相关，存储了备用服务的配置信息。

### `message_example/` 文件夹

- 包含所有通信报文的格式。

### `src/` 文件夹

#### `com/` 文件夹

- `UDPConnector.cpp`：实现了 UDP 连接的相关功能，如创建套接字、发送和接收数据等。
- `UDPConnector.h`：定义了 UDP 连接类 `UDPConnector` 的接口和成员变量。

#### `config/` 文件夹

- `Config.cpp`：实现了配置文件的读取、保存和修改功能，确保项目可以根据配置文件进行灵活配置。
- `Config.h`：定义了配置类 `Config` 的接口和成员变量。
- `ConfigUtil.cpp`：包含了配置文件处理的工具函数，如解析命令行参数、获取配置文件路径等。

#### `constant/` 文件夹

- `globals.cpp`：定义了项目中的全局常量，如服务器通道、管理通道、类型映射等。
- `globals.h`：声明了全局常量，方便在项目的其他文件中使用。

#### `controller/` 文件夹

- `RequestController.cpp`：实现了请求处理的逻辑，根据不同的请求类型调用相应的服务进行处理。
- `RequestController.h`：定义了请求控制器类 `RequestController` 的接口和成员变量。

#### `dao/` 文件夹

- `LocalDatabaseDao.cpp`：实现了与本地数据库的交互功能，如添加服务、实例、元数据等。
- `LocalDatabaseDao.h`：定义了本地数据库访问对象类 `LocalDatabaseDao` 的接口和成员变量。

#### `database/` 文件夹

- `LocalDatabase.cpp`：实现了本地数据库的管理功能，如初始化数据库、连接数据库等。
- `LocalDatabase.h`：定义了本地数据库类 `LocalDatabase` 的接口和成员变量。

#### `model/` 文件夹

- `entity/` 文件夹：包含了项目中的实体类，如 `Header` 类，用于表示数据的结构。

#### `service/` 文件夹

- `HeartbeatService.cpp`：实现了心跳服务的逻辑，用于检测服务的存活状态。
- `HeartbeatService.h`：定义了心跳服务类 `HeartbeatService` 的接口和成员变量。
- `MetadataService.cpp`：实现了元数据服务的逻辑，如验证和处理元数据。
- `MetadataService.h`：定义了元数据服务类 `MetadataService` 的接口和成员变量。
- `QueryService.cpp`：实现了查询服务的逻辑，用于查询服务的信息。
- `QueryService.h`：定义了查询服务类 `QueryService` 的接口和成员变量。
- `RegisterService.cpp`：实现了注册服务的逻辑，用于注册新的服务。
- `RegisterService.h`：定义了注册服务类 `RegisterService` 的接口和成员变量。
- `Service.cpp`：实现了服务的基本功能，如启动、停止等。
- `Service.h`：定义了服务类 `Service` 的接口和成员变量。

#### `utils/` 文件夹

- `Util.cpp`：包含了项目中常用的工具函数，如清除文件内容、计算最大公约数、获取当前时间戳等。
- `Util.h`：声明了工具类 `Util` 的接口和成员函数。

#### `main.cpp`

项目的入口文件，负责加载配置文件、获取设备锁、连接数据库、启动线程等操作，是项目启动的核心代码。



## 运行逻辑

1. **启动阶段**：`main.cpp` 作为项目的入口文件，负责加载配置文件、获取设备锁、连接数据库、启动线程等操作。
2. **通信阶段**：`com/UDPConnector.cpp` 中的 `connector` 函数创建 UDP 套接字，绑定地址，然后进入循环等待接收广播消息。
3. **请求处理阶段**：接收到消息后，解析报头和内容，根据报头的类型调用 `controller/RequestController.cpp` 中的相应处理函数。
4. **业务逻辑处理阶段**：`controller` 层调用 `service` 层的具体服务进行业务逻辑处理，如注册、查询等。
5. **数据访问阶段**：`service` 层调用 `dao` 层的函数与数据库进行交互，完成数据的增删改查操作。
6. **响应阶段**：处理完请求后，生成响应消息，调用 `formatResponse` 函数封装报头和内容，然后通过 UDP 套接字发送给客户端。



> + 定义同步主备消息，主备间监测目前通过设备锁监测是否正常运行。未来计划改为抢占式。
> + 本地数据库采用单例模式设计，通过定时器检测数据是否过期，更新数据状态，进行数据持久化。



# 快捷启动

> 请确保系统中已经安装了 `nlohmann/json` 库，否则编译时会出错。

1. 运行 `sh build.sh` ，构建项目文件。
2. 运行 `sh launch.sh` ，启动注册中心。

### 测试方法

1. 运行 `sh launch.sh` ，启动注册中心。

2. 进入example目录。

3. 运行 `sh build_example.sh` ，构建测试文件。

4. 运行 `sh launch_example.sh` ，启动测试文件。相关测试参数配置可在 `example.json` 中修改。

   

# 配置详情

## 通信类型

+ 远程服务发送的消息都是广播。
+ 用户向注册中心查询消息是单播。
+ 管理端管理消息是单播。
+ 10450端口：注册、元数据、查询、心跳。
+ 10451端口：管理相关服务。

## 报头信息

这些属性在目前的使用中，除了send_time和type之外，只用来占位，不包含实际意义。

1. 协议标识符，identifier，int32_t。
2. 信息发送时间，sendTime，long long。毫秒级时间戳
3. 报文长度，messageLength，int32_t
4. 报文流水号,serialNumber，int32_t
5. 校验位，checkBit，int32_t
6. 消息类型，type，int32_t
   1. register, 注册报文, type = 1。
   2. register_response, 注册回复报文, type = 2。
   3. matadata, 元数据注册报文, type = 3。
   4. matadata_response, 元数据注册回复报文, type = 4。
   5. query, 查询报文, type = 5。
   6. query_response, 查询回复报文, type = 6。
   7. heartbeat, 心跳报文, type = 7。
   8. manage, 管理消息报文, type = 8。
   9. manage_response, 管理消息回复报文, type = 9。

## 报文信息


1. status,服务状态分为UP和SHUTUP，即运行状态和关闭状态。后续可能加入例如 "UP"（服务正常），"DOWN"（服务不可用），"STARTING"（服务启动中），"OUT_OF_SERVICE"（服务下线），"UNKNOWN"（未知状态）等。
2. role，服务的优先级，即主备份，0为主份，1为备份，数字越大，优先级越低。另一种方法是使用字符串，primary为主份，backup为备份。
3. card，板卡的id。
4. tag，标签，用户自定义
5. 内存占用，network_usage，单位为KB
6. 服务构建时间，build_time
7. 实例创建时间，create_time
8. build_time,create_time等时间都是字符串，也可以改为时间戳。
9. 注册完后，回复status，1代表成功，2代表失败。
10. ip_num，返回的地址的数量，0代表全部返回，0以上的数字代表返回的数目。



# 注意事项

1. 注册中心接收消息全部为广播接收。
2. 注册中心回复消息全部为单播发送。



# 贡献

如果你对本项目有任何建议或发现了 bug，请在 GitHub 上提交 issue 或 pull request。