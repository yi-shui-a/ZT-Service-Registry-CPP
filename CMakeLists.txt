# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(ZT-Service-Registry-CPP)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 查找 src 目录下的所有 .cpp 和 .h 文件
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# 设置可执行文件输出路径到主目录下的 bin 目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 包含 nlohmann/json 库
find_package(nlohmann_json 3.2.0 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# 如果你需要链接其他库，可以继续在这里添加
# 例如：
# find_library(OTHER_LIBRARY other_library_name /path/to/library)
# target_link_libraries(${PROJECT_NAME} PRIVATE ${OTHER_LIBRARY})