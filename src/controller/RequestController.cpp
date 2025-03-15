#include "RequestController.h"

json RequestController::handleRegister(Header &header, json &content)
{
    /**
     * @brief 初始化服务
     */
    RegisterService service = RegisterService();
    /**
     * @brief 处理服务
     * 
    */
    return RequestController::handleService(service, header, content);

}

json RequestController::handleService(Service& service, Header &header, json &content){
    /**
     * @brief 检查数据正确性
     */
    if (!service.isValidData(content))
    {
        return service.formatResponse(json::object(), 400, "ERROR: params error");
    }
    /**
     * @brief 处理数据，修改数据库
     */
    json res = service.processData(header, content);
    /**
     * @brief 生成返回值json，返回json
     */

    return service.formatResponse(res, 200, "SUCCESS: params success");
}