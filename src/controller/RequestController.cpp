#include "RequestController.h"

json RequestController::handleRegister(Header &header, json &content)
{
    RegisterService service = RegisterService();
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
    json res = service.processData(content);
    /**
     * @brief 生成返回值json，返回json
     */

    return service.formatResponse(res, 200, "SUCCESS: params success");
}