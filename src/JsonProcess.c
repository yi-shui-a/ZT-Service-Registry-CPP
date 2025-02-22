#include <stdio.h>
#include <time.h>
#include "cJSON.h"

#include "DataStruct.h"

char *processResponse(cJSON *response, uint8_t type);
int getDigit(int number);


time_t getReceiveSendTime(cJSON *data)
{
    cJSON *header = cJSON_GetObjectItemCaseSensitive(data, "header");
    cJSON *send_time = cJSON_GetObjectItemCaseSensitive(header, "send_time");
    time_t time_res = send_time->valueint;
    return time_res;
};

uint8_t getReceiveType(cJSON *data)
{
    cJSON *header = cJSON_GetObjectItemCaseSensitive(data, "header");
    cJSON *type = cJSON_GetObjectItemCaseSensitive(header, "type");
    uint8_t type_res = type->valueint;
    return type_res;
};

// 通过注册服务实例
char *addServiceInstanceRegister(cJSON *database, cJSON *data)
{
    // 读取关键信息
    cJSON *send_time = cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(data, "header"), "send_time");
    cJSON *server_name = cJSON_GetObjectItemCaseSensitive(data, "server_name");
    cJSON *address = cJSON_GetObjectItemCaseSensitive(data, "address");
    cJSON *port = cJSON_GetObjectItemCaseSensitive(data, "port");

    cJSON *services_list = cJSON_GetObjectItemCaseSensitive(data, "services_list");

    cJSON *temp_register_service = NULL;
    // 遍历data中的每个元素，对注册json中的每一条数据进行操作
    cJSON_ArrayForEach(temp_register_service, services_list)
    {
        // 判断该服务是否注册
        cJSON *temp_service = NULL;
        // 使用 cJSON_ArrayForEach 宏遍历database中的每个元素,判断有没有当前服务
        cJSON_ArrayForEach(temp_service, cJSON_GetObjectItemCaseSensitive(database, "services"))
        {
            // 检查元素类类型是否符合,如果存在该服务，直接注册该实例
            if (strcmp(cJSON_GetObjectItemCaseSensitive(temp_service, "service_name")->valuestring, cJSON_GetObjectItemCaseSensitive(temp_register_service, "service_name")->valuestring) == 0)
            {
                // 如果没有实例列表，创建实例列表
                if (cJSON_GetObjectItemCaseSensitive(temp_service, "instances") == NULL)
                {
                    cJSON_AddItemToObject(temp_service, "instances", cJSON_CreateArray());
                }

                // 有实例列表，开始检索
                cJSON *temp_instance = NULL;
                cJSON_ArrayForEach(temp_instance, cJSON_GetObjectItemCaseSensitive(temp_service, "instances"))
                {
                    // 通过服务器名、地址、端口判断是否是同一个实例
                    // 如果存在该实例，修改状态、心跳、主备
                    // 如果存在该实例，更新所有状态
                    if (strcmp(cJSON_GetObjectItemCaseSensitive(temp_instance, "server_name")->valuestring, server_name->valuestring) == 0 && strcmp(cJSON_GetObjectItemCaseSensitive(temp_instance, "address")->valuestring, address->valuestring) == 0 && cJSON_GetObjectItemCaseSensitive(temp_instance, "port")->valueint == port->valueint)
                    {
                        cJSON_SetValuestring(cJSON_GetObjectItemCaseSensitive(temp_instance, "status"), "UP");
                        cJSON_SetNumberValue(cJSON_GetObjectItemCaseSensitive(temp_instance, "heartbeat_time"), send_time->valueint);
                        cJSON_SetNumberValue(cJSON_GetObjectItemCaseSensitive(temp_instance, "role"), cJSON_GetObjectItemCaseSensitive(temp_register_service, "role")->valueint);
                        // 修改完，可以退出循环
                        break;
                    }
                }

                // 如果不存在该实例，创建该实例（状态：有服务，有实例列表，无当前实例）
                if (temp_instance == NULL)
                {
                    cJSON *new_instance;
                    // 添加id
                    char temp_id[50] = {0};
                    strcat(temp_id, cJSON_GetObjectItemCaseSensitive(temp_service, "service_name")->valuestring);
                    strcat(temp_id, "-");
                    int temp_int = cJSON_GetArraySize(cJSON_GetObjectItemCaseSensitive(temp_service, "instances")) + 1;
                    char temp_str[10];
                    snprintf(temp_str, sizeof(temp_str), "%d", temp_int);
                    strcat(temp_id, temp_str);
                    cJSON_AddStringToObject(new_instance, "instance_id", temp_id);
                    // 添加服务器名等
                    cJSON_AddStringToObject(new_instance, "server_name", server_name->valuestring);
                    cJSON_AddStringToObject(new_instance, "address", address->valuestring);
                    cJSON_AddNumberToObject(new_instance, "port", port->valueint);
                    cJSON_AddNumberToObject(new_instance, "heartbeat_time", send_time->valueint);
                    cJSON_AddStringToObject(new_instance, "status", "UP");
                    cJSON_AddNumberToObject(new_instance, "role", cJSON_GetObjectItemCaseSensitive(temp_register_service, "role")->valueint);
                    // 添加元数据
                    // cJSON_AddItemToObject(new_instance, "metadata", cJSON_CreateObject());
                    // cJSON_AddNumberToObject(cJSON_GetObjectItemCaseSensitive(new_instance, "metadata"), "create_time", send_time->valueint);
                    // 将新实例添加到instances数组里
                    cJSON_AddItemToArray(cJSON_GetObjectItemCaseSensitive(temp_service, "instances"), new_instance);

                    // 添加实例完毕，开始添加下一个实例
                    break;
                }
                else
                {
                    break;
                }
            }
        }
        // 如果没找到该服务，temp_service为空；如果找到了，temp_service指向当前服务。
        // 创建服务，开始写入数据
        if (temp_service == NULL)
        {
            // 创建服务和instances
            temp_service = cJSON_CreateObject();
            cJSON_AddStringToObject(temp_service, "service_name", cJSON_GetObjectItemCaseSensitive(temp_register_service, "service_name")->valuestring);
            // 创建实例列表
            cJSON_AddItemToObject(temp_service, "instances", cJSON_CreateArray());
            // 创建instance
            cJSON *new_instance = cJSON_CreateObject();
            ;
            // 添加id
            char temp_id[50] = {0};
            strcat(temp_id, cJSON_GetObjectItemCaseSensitive(temp_service, "service_name")->valuestring);
            strcat(temp_id, "-");
            int temp_int = cJSON_GetArraySize(cJSON_GetObjectItemCaseSensitive(temp_service, "instances")) + 1;
            char temp_str[10];
            snprintf(temp_str, sizeof(temp_str), "%d", temp_int);
            strcat(temp_id, temp_str);
            cJSON_AddStringToObject(new_instance, "instance_id", temp_id);
            // 添加服务器名等
            cJSON_AddStringToObject(new_instance, "server_name", server_name->valuestring);
            cJSON_AddStringToObject(new_instance, "address", address->valuestring);
            cJSON_AddNumberToObject(new_instance, "port", port->valueint);
            cJSON_AddNumberToObject(new_instance, "heartbeat_time", send_time->valueint);
            cJSON_AddStringToObject(new_instance, "status", "UP");
            cJSON_AddNumberToObject(new_instance, "role", cJSON_GetObjectItemCaseSensitive(temp_register_service, "role")->valueint);
            // 添加元数据
            // cJSON_AddItemToObject(new_instance, "metadata", cJSON_CreateObject());
            // cJSON_AddNumberToObject(cJSON_GetObjectItemCaseSensitive(new_instance, "metadata"), "create_time", send_time->valueint);
            // 将新实例添加到instances数组里
            cJSON_AddItemToArray(cJSON_GetObjectItemCaseSensitive(temp_service, "instances"), new_instance);

            // 将新的service添加到database
            cJSON_AddItemToArray(cJSON_GetObjectItemCaseSensitive(database, "services"), temp_service);
        }
    }

    // 处理回复数据
    cJSON *response = cJSON_CreateObject();
    cJSON_AddNumberToObject(response, "status", 1);
    cJSON_AddStringToObject(response, "server_name", server_name->valuestring);
    char *response_str = processResponse(response, 2);
    return response_str;
}

// 通过元数据注册服务实例
char *addServiceInstanceMetadata(cJSON *database, cJSON *data)
{
    // 读取关键信息
    cJSON *send_time = cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(data, "header"), "send_time");
    cJSON *server_name = cJSON_GetObjectItemCaseSensitive(data, "server_name");
    cJSON *address = cJSON_GetObjectItemCaseSensitive(data, "address");
    cJSON *port = cJSON_GetObjectItemCaseSensitive(data, "port");
    cJSON *services_list = cJSON_GetObjectItemCaseSensitive(data, "services_list");

    cJSON *temp_register_service = NULL;
    // 遍历data中的每个元素，对注册json中的每一条数据进行操作
    cJSON_ArrayForEach(temp_register_service, services_list)
    {
        // 判断该服务是否注册
        cJSON *temp_service = NULL;
        // 使用 cJSON_ArrayForEach 宏遍历database中的每个元素,判断有没有当前服务
        cJSON_ArrayForEach(temp_service, cJSON_GetObjectItemCaseSensitive(database, "services"))
        {
            // 检查元素类类型是否符合,如果存在该服务，直接注册该实例
            if (strcmp(cJSON_GetObjectItemCaseSensitive(temp_service, "service_name")->valuestring, cJSON_GetObjectItemCaseSensitive(temp_register_service, "service_name")->valuestring) == 0)
            {
                // 检查服务元数据是否存在
                // 存在则将以前的删除了，再把新的添加上
                if (cJSON_GetObjectItemCaseSensitive(temp_service, "metadata") != NULL)
                {
                    cJSON_DeleteItemFromObject(temp_service, "metadata");
                }
                // 不存在就直接把新的添加上
                cJSON *temp_metadata_json = cJSON_Duplicate(cJSON_GetObjectItemCaseSensitive(temp_register_service, "metadata"), 1); // 第二个参数为1时，会复制字符串内容
                cJSON_AddItemToObject(temp_service, "metadata", temp_metadata_json);
                // 如果没有实例列表，创建实例列表
                if (cJSON_GetObjectItemCaseSensitive(temp_service, "instances") == NULL)
                {
                    cJSON_AddItemToObject(temp_service, "instances", cJSON_CreateArray());
                }

                // 有实例列表，开始检索
                cJSON *temp_instance = NULL;
                cJSON_ArrayForEach(temp_instance, cJSON_GetObjectItemCaseSensitive(temp_service, "instances"))
                {
                    // 通过服务器名、地址、端口判断是否是同一个实例
                    // 如果存在该实例，先修改状态、心跳、主备，在检查是否存在元数据
                    // 如果存在该实例，更新所有状态
                    if (strcmp(cJSON_GetObjectItemCaseSensitive(temp_instance, "server_name")->valuestring, server_name->valuestring) == 0 && strcmp(cJSON_GetObjectItemCaseSensitive(temp_instance, "address")->valuestring, address->valuestring) == 0 && cJSON_GetObjectItemCaseSensitive(temp_instance, "port")->valueint == port->valueint)
                    {
                        // cJSON_SetValuestring(cJSON_GetObjectItemCaseSensitive(temp_instance, "status"), "UP");
                        cJSON_SetNumberValue(cJSON_GetObjectItemCaseSensitive(temp_instance, "heartbeat_time"), send_time->valueint);
                        // cJSON_SetNumberValue(cJSON_GetObjectItemCaseSensitive(temp_instance, "role"), cJSON_GetObjectItemCaseSensitive(temp_register_service, "role")->valueint);

                        // 检查是否存在元数据
                        // 如果不存在元数据，添加元数据
                        // printf("\n\n%s\n\n", cJSON_Print(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata")));
                        // printf("\n\n%d\n\n", cJSON_IsNull(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata")));
                        if (cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata") == NULL)
                        {
                            cJSON_AddItemToObject(temp_instance, "metadata", cJSON_CreateObject());
                            if (cJSON_IsString(cJSON_GetObjectItemCaseSensitive(temp_register_service, "card")))
                            {
                                // printf("")
                                cJSON_AddStringToObject(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata"), "card", cJSON_GetObjectItemCaseSensitive(temp_register_service, "card")->valuestring);
                            }
                            if (cJSON_IsString(cJSON_GetObjectItemCaseSensitive(temp_register_service, "os")))
                            {
                                cJSON_AddStringToObject(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata"), "os", cJSON_GetObjectItemCaseSensitive(temp_register_service, "os")->valuestring);
                            }
                            cJSON_AddNumberToObject(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata"), "create_time", send_time->valueint);
                        }
                        // 如果存在元数据，检查是否存在该项，存在就修改，不存在就添加
                        else
                        {
                            if (cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata"), "card") != NULL)
                            {
                                cJSON_SetValuestring(cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata"), "card"), cJSON_GetObjectItemCaseSensitive(temp_register_service, "card")->valuestring);
                            }
                            else
                            {
                                cJSON_AddStringToObject(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata"), "card", cJSON_GetObjectItemCaseSensitive(temp_register_service, "card")->valuestring);
                            }
                            if (cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata"), "os") != NULL)
                            {
                                cJSON_SetValuestring(cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata"), "os"), cJSON_GetObjectItemCaseSensitive(temp_register_service, "os")->valuestring);
                            }
                            else
                            {
                                cJSON_AddStringToObject(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata"), "os", cJSON_GetObjectItemCaseSensitive(temp_register_service, "os")->valuestring);
                            }
                            if (cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata"), "create_time") != NULL)
                            {
                                cJSON_SetIntValue(cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata"), "create_time"), send_time->valueint);
                            }
                            else
                            {
                                cJSON_AddNumberToObject(cJSON_GetObjectItemCaseSensitive(temp_instance, "metadata"), "create_time", send_time->valueint);
                            }
                        }

                        // 修改完，可以退出循环
                        break;
                    }
                }

                // 如果不存在该实例，创建该实例（状态：有服务，有实例列表，无当前实例）
                if (temp_instance == NULL)
                {
                    cJSON *new_instance;
                    // 添加id
                    char temp_id[50] = {0};
                    strcat(temp_id, cJSON_GetObjectItemCaseSensitive(temp_service, "service_name")->valuestring);
                    strcat(temp_id, "-");
                    int temp_int = cJSON_GetArraySize(cJSON_GetObjectItemCaseSensitive(temp_service, "instances")) + 1;
                    char temp_str[10];
                    snprintf(temp_str, sizeof(temp_str), "%d", temp_int);
                    strcat(temp_id, temp_str);
                    cJSON_AddStringToObject(new_instance, "instance_id", temp_id);
                    // 添加服务器名等
                    cJSON_AddStringToObject(new_instance, "server_name", server_name->valuestring);
                    cJSON_AddStringToObject(new_instance, "address", address->valuestring);
                    cJSON_AddNumberToObject(new_instance, "port", port->valueint);
                    cJSON_AddNumberToObject(new_instance, "heartbeat_time", send_time->valueint);
                    cJSON_AddStringToObject(new_instance, "status", "STARTING");
                    cJSON_AddNumberToObject(new_instance, "role", 0);
                    // 添加元数据
                    cJSON_AddItemToObject(new_instance, "metadata", cJSON_CreateObject());
                    cJSON_AddNumberToObject(cJSON_GetObjectItemCaseSensitive(new_instance, "metadata"), "create_time", send_time->valueint);
                    cJSON_AddStringToObject(cJSON_GetObjectItemCaseSensitive(new_instance, "metadata"), "card", cJSON_GetObjectItemCaseSensitive(temp_register_service, "card")->valuestring);
                    cJSON_AddStringToObject(cJSON_GetObjectItemCaseSensitive(new_instance, "metadata"), "os", cJSON_GetObjectItemCaseSensitive(temp_register_service, "os")->valuestring);

                    // 将新实例添加到instances数组里
                    cJSON_AddItemToArray(cJSON_GetObjectItemCaseSensitive(temp_service, "instances"), new_instance);

                    // 添加实例完毕，开始添加下一个实例
                    break;
                }
                else
                {
                    break;
                }
            }
        }
        // 如果不存在服务，创建服务，创建实例
        if (temp_service == NULL)
        {
            temp_service = cJSON_CreateObject();
            cJSON_AddStringToObject(temp_service, "service_name", cJSON_GetObjectItemCaseSensitive(temp_register_service, "service_name")->valuestring);
            cJSON_AddItemToObject(temp_service, "metadata", cJSON_Duplicate(cJSON_GetObjectItemCaseSensitive(temp_register_service, "metadata"), 1));
            // 创建实例列表
            cJSON_AddItemToObject(temp_service, "instances", cJSON_CreateArray());
            // 创建实例
            cJSON *new_instance = cJSON_CreateObject();
            // 添加id
            char temp_id[50] = {0};
            strcat(temp_id, cJSON_GetObjectItemCaseSensitive(temp_service, "service_name")->valuestring);
            strcat(temp_id, "-");
            int temp_int = cJSON_GetArraySize(cJSON_GetObjectItemCaseSensitive(temp_service, "instances")) + 1;
            char temp_str[10];
            snprintf(temp_str, sizeof(temp_str), "%d", temp_int);
            strcat(temp_id, temp_str);

            // printf("\n%ld\n%s\n\n",strlen(temp_id),temp_id);
            // temp_id[strlen(temp_id)]=0;
            cJSON_AddStringToObject(new_instance, "instance_id", temp_id);
            // 添加服务器名等
            cJSON_AddStringToObject(new_instance, "server_name", server_name->valuestring);
            cJSON_AddStringToObject(new_instance, "address", address->valuestring);
            cJSON_AddNumberToObject(new_instance, "port", port->valueint);
            cJSON_AddNumberToObject(new_instance, "heartbeat_time", send_time->valueint);
            cJSON_AddStringToObject(new_instance, "status", "STARTING");
            cJSON_AddNumberToObject(new_instance, "role", 0);
            // 添加元数据
            cJSON_AddItemToObject(new_instance, "metadata", cJSON_CreateObject());
            cJSON_AddNumberToObject(cJSON_GetObjectItemCaseSensitive(new_instance, "metadata"), "create_time", send_time->valueint);
            cJSON_AddStringToObject(cJSON_GetObjectItemCaseSensitive(new_instance, "metadata"), "card", cJSON_GetObjectItemCaseSensitive(temp_register_service, "card")->valuestring);
            cJSON_AddStringToObject(cJSON_GetObjectItemCaseSensitive(new_instance, "metadata"), "os", cJSON_GetObjectItemCaseSensitive(temp_register_service, "os")->valuestring);

            // 将新实例添加到instances数组里
            cJSON_AddItemToArray(cJSON_GetObjectItemCaseSensitive(temp_service, "instances"), new_instance);
            // char * bbb= cJSON_Print(database);
            // printf("\n\nbbbbbbbbbbb:\n%s\n\n",bbb);
            // free(bbb);
            cJSON_AddItemToArray(cJSON_GetObjectItemCaseSensitive(database, "services"), temp_service);

            // char * aaa= cJSON_Print(temp_service);
            // printf("\n\naaaaaaaaaaaaa:\n%s\n\n",aaa);
            // free(aaa);
        }
    }

    // 处理回复数据
    cJSON *response = cJSON_CreateObject();
    cJSON_AddNumberToObject(response, "status", 1);
    cJSON_AddStringToObject(response, "server_name", server_name->valuestring);
    char *response_str = processResponse(response, 4);
    return response_str;
}

char *query(cJSON *database, cJSON *req_data)
{
    cJSON *service_name = cJSON_GetObjectItemCaseSensitive(req_data, "service_name");
    int service_num = cJSON_GetObjectItemCaseSensitive(req_data, "service_num")->valueint;

    // 创建一个回复变量
    cJSON *result_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(result_json, "service_num", 0);
    cJSON_AddItemToObject(result_json, "instance_list", cJSON_CreateArray());
    // int instance_count = 0;//统计服务数量

    // 创建一个暂存服务变量
    cJSON *temp_service = NULL;
    cJSON_ArrayForEach(temp_service, cJSON_GetObjectItemCaseSensitive(database, "services"))
    {
        // 找到服务
        if (strcmp(cJSON_GetObjectItemCaseSensitive(temp_service, "service_name")->valuestring, service_name->valuestring) == 0)
        {
            // 遍历服务中所有的instance
            cJSON *temp_instance = NULL;
            int temp_instance_num = 0;
            cJSON_ArrayForEach(temp_instance, cJSON_GetObjectItemCaseSensitive(temp_service, "instances"))
            {
                // printf("Instance");
                cJSON *temp_res_instance = cJSON_CreateObject();
                cJSON_AddStringToObject(temp_res_instance, "address", cJSON_GetObjectItemCaseSensitive(temp_instance, "address")->valuestring);
                cJSON_AddNumberToObject(temp_res_instance, "port", cJSON_GetObjectItemCaseSensitive(temp_instance, "port")->valueint);
                cJSON_AddStringToObject(temp_res_instance, "server_name", cJSON_GetObjectItemCaseSensitive(temp_instance, "server_name")->valuestring);
                cJSON_AddNumberToObject(temp_res_instance, "role", cJSON_GetObjectItemCaseSensitive(temp_instance, "role")->valueint);
                // 将temp_res_instance添加到result_json中
                cJSON_AddItemToArray(cJSON_GetObjectItemCaseSensitive(result_json, "instance_list"), temp_res_instance);
                temp_instance_num++;
                if (temp_instance_num == service_num)
                {
                    break;
                }
            }
            // 修改service_num
            // cJSON_SetNumberValue(cJSON_GetObjectItemCaseSensitive(result_json, "service_num"), cJSON_GetArraySize(cJSON_GetObjectItemCaseSensitive(result_json, "instance_list")));
            cJSON_SetNumberValue(cJSON_GetObjectItemCaseSensitive(result_json, "service_num"), temp_instance_num);
        }
    }

    // 没找到服务，直接返回

    // 给result_json添加报头，并转为字符串返回
    return processResponse(result_json, 6);
}

void resetHeartbeatTime(cJSON *database, cJSON *data)
{
    // 读取关键信息
    cJSON *send_time = cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(data, "header"), "send_time");
    cJSON *server_name = cJSON_GetObjectItemCaseSensitive(data, "server_name");
    cJSON *address = cJSON_GetObjectItemCaseSensitive(data, "address");
    cJSON *port = cJSON_GetObjectItemCaseSensitive(data, "port");
    cJSON *services_list = cJSON_GetObjectItemCaseSensitive(data, "services_list");

    // 遍历services_list，查找服务
    cJSON *temp_heartbeat_services = NULL;
    cJSON_ArrayForEach(temp_heartbeat_services, services_list)
    {
        // 遍历database中的service，查找对应service
        cJSON *temp_service = NULL;
        cJSON_ArrayForEach(temp_service, cJSON_GetObjectItemCaseSensitive(database, "services"))
        {
            // service相同，遍历instances，比较address，port，server_name
            if (strcmp(cJSON_GetObjectItemCaseSensitive(temp_service, "service_name")->valuestring, cJSON_GetObjectItemCaseSensitive(temp_heartbeat_services, "service_name")->valuestring) == 0)
            {
                cJSON *temp_instance;
                cJSON_ArrayForEach(temp_instance, cJSON_GetObjectItemCaseSensitive(temp_service, "instances"))
                {
                    // 如果address，port，server_name都相同，更新心跳
                    if (strcmp(cJSON_GetObjectItemCaseSensitive(temp_instance, "server_name")->valuestring, server_name->valuestring) == 0 && strcmp(cJSON_GetObjectItemCaseSensitive(temp_instance, "address")->valuestring, address->valuestring) == 0 && cJSON_GetObjectItemCaseSensitive(temp_instance, "port")->valueint == port->valueint)
                    {
                        cJSON_SetNumberValue(cJSON_GetObjectItemCaseSensitive(temp_instance, "heartbeat_time"), send_time->valueint);
                    }
                }

                // 更新完后return，继续遍历services_list
                break;
            }
        }
    }
    // 遍历结束，无需返回消息，函数结束
}

char *processResponse(cJSON *response, uint8_t type)
{
    // 获取当前时间戳
    time_t send_time;
    time(&send_time);
    // 编写报头
    cJSON_AddItemToObject(response, "header", cJSON_CreateObject());
    cJSON_AddNumberToObject(cJSON_GetObjectItemCaseSensitive(response, "header"), "protocol_identifier", 22);
    cJSON_AddNumberToObject(cJSON_GetObjectItemCaseSensitive(response, "header"), "send_time", send_time);
    cJSON_AddNumberToObject(cJSON_GetObjectItemCaseSensitive(response, "header"), "message_length", 0);
    cJSON_AddNumberToObject(cJSON_GetObjectItemCaseSensitive(response, "header"), "message_serial_number", 1);
    cJSON_AddNumberToObject(cJSON_GetObjectItemCaseSensitive(response, "header"), "check_bit", 2);
    cJSON_AddNumberToObject(cJSON_GetObjectItemCaseSensitive(response, "header"), "type", type);
    char *reponse_str = cJSON_Print(response);
    // 更新报文长度,不包括字符串末尾的\0
    int i = strlen(reponse_str);
    i = i + getDigit(i);
    i -= 1;
    // cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(response, "header"), "message_length")->valuedouble = i;
    cJSON_SetNumberValue(cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(response, "header"), "message_length"), i);
    free(reponse_str);
    reponse_str = cJSON_Print(response);
    printf("%s", reponse_str);

    return reponse_str;
}

// int getDigit(int number)
// {
//     int count = 0;
//     while (number != 0)
//     {
//         number /= 10;
//         count++;
//     }
//     return count;
// }

void updateServiceInstanceStatus(cJSON *database, time_t current_time, time_t interval_time)
{

    cJSON *temp_service = NULL;
    cJSON_ArrayForEach(temp_service, cJSON_GetObjectItemCaseSensitive(database, "services"))
    {
        cJSON *temp_instance = NULL;
        cJSON_ArrayForEach(temp_instance, cJSON_GetObjectItemCaseSensitive(temp_service, "instances"))
        {
            if (current_time - cJSON_GetObjectItemCaseSensitive(temp_instance, "heartbeat_time")->valueint > interval_time)
            {
                cJSON_SetValuestring(cJSON_GetObjectItemCaseSensitive(temp_instance, "status"), "DOWN");
            }
        }
    }
}

cJSON *parseFromStr(char *buffer)
{
    return cJSON_Parse(buffer);
}

void test()
{
    cJSON *database = cJSON_CreateObject();
    cJSON_AddItemToObject(database, "services", cJSON_CreateArray());
    printf("%s", cJSON_Print(database));
}

// 测试函数
// int main()
// {
//     // test();
//     cJSON *database = cJSON_CreateObject();
//     cJSON_AddItemToObject(database, "services", cJSON_CreateArray());
//     processResponse(database, 5);
//     // struct connect_struct conn = config_load(1);
//     // printf("READ_BUFFER_SIZE: %zu\n", conn.READ_BUFFER_SIZE);

//     // printf("CORE_BUFFER_SIZE: %zu\n", conn.CORE_BUFFER_SIZE);

//     // printf("READ_TIME_INTERTAL: %zu\n", conn.READ_TIME_INTERTAL);

//     // printf("HEARTBEAT_TIME_INTERTAL: %zu\n", conn.HEARTBEAT_TIME_INTERTAL);

//     // printf("ADDRESS: %s\n", conn.ADDRESS);

//     // printf("PORT: %hu\n", conn.PORT); // 注意：%hu是uint16_t的正确格式说明符
//     return 0;
// }