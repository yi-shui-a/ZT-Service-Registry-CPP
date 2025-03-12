# 快捷启动
1. 运行 `sh build.sh` ，构建项目文件。
2. 运行 `sh launch.sh` ，启动注册中心。


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
2. 信息发送时间，sendTime，long long。
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

1. status,服务状态分为UP和SHUTUP，即运行状态和关闭状态。后续可能加入例如 "UP"（服务正常），"DOWN"（服务不可用），"STARTING"（服务启动中），"OUT_OF_SERVICE"（服务下线），"UNKNOWN"（未知状态）等。
2. role，服务的优先级，即主备份，0为主份，1为备份，数字越大，优先级越低。另一种方法是使用字符串，primary为主份，backup为备份。
3. card，板卡的id。
4. tag，标签，用户自定义
5. 内存占用，network_usage，单位为KB
6. 服务构建时间，build_time
7. 实例创建时间，create_time
8. build_time,create_time等时间都是一个time_t类型的时间戳
9. 注册完后，回复status，1代表成功，2代表失败。
10. ip_num，返回的地址的数量，0代表全部返回，0以上的数字代表返回的数目。




## 流程
1. 加载配置文件，设置全局变量。
2. 运行环境监测，检查宿主机配置、开销占用情况。
3. 初始化数据库。
4. 开启服务、管理线程。
5. 线程根据需求加载全局变量，定义配置UDPsocket。
6. 持续监听消息。
   + 根据服务端、管理端消息对数据库进行增删改查。
   + 定时更新服务实例状态信息。(查询时监控是否可靠)
   + 定期更新持久化数据库。（先覆盖文件）
   + 定义同步主备消息，主备间心跳检测(如果需要通信，将来专门开一个线程进行检测通信)

## 注意
1. 注册中心接收消息全部为广播接收。
2. 注册中心回复消息全部为单播发送。

## 项目结构
1. controller 层每个函数处理一个请求类型。
2. service 层处理业务逻辑。
3. model 层对请求和回复消息进行处理。