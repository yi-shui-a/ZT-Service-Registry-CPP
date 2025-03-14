#include "RegisterService.h"


// 实现 isValidData 函数
bool RegisterService::isValidData(const json& content) {
    // 这里可以添加具体的数据验证逻辑
    std::cout << "RegisterService: Validating data..." << std::endl;
    return true;
}

// 实现 processData 函数
json RegisterService::processData(const json& content) {
    // 这里可以添加具体的数据处理逻辑
    std::cout << "RegisterService: Processing data..." << std::endl;
    json j1 = {
    {"name", "John"},
    {"age", 30},
    {"is_student", false}
};
    return j1;
}
