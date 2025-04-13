#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <nlohmann/json.hpp>

// 使用 nlohmann/json 库
using json = nlohmann::json;

class Util {
public:
    // 解析argv
    static json parseArgs(int argc, char* argv[]);
    // 清除文件内容
    static void clearFile(const std::string& filepath);
    // 计算两个整数 a 和 b 的最大公约数。
    static int gcd(int a, int b);
    // 计算一个整数数组中所有元素的最大公约数
    static int gcdMultiple(const int nums[], int size);
    // 计算该整数的位数。
    static int getDigit(int number);

    // 获取当前毫秒级时间戳
    static long long getCurrentTimeMillis();

};

#endif // UTIL_H