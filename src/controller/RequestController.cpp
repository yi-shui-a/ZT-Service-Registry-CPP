#include "RequestController.h"

json RequestController::handleRegister(Header& header,json& content)
{
    RegisterService service = RegisterService();
    /**
     * @brief 检查数据正确性
     */
    if(!service.isValidData(content)){
        json a = json::object();
        return service.formatResponse(json::object(),400,"ERROR: params error");
    }
    /**
     * @brief 处理数据，修改数据库
     */

    /**
     * @brief 生成返回值json，返回json
     */
    
    
}