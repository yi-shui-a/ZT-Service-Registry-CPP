#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ConfigUtil
{

public:
    // 解析命令行参数，这个函数不确保参数的正确性
    // 目前只查找查找 --configPath 选项
    static json parsePathFromArgs(int argc, char *argv[])
    {
        json argInfo;

        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];

            if (arg == "--configPath")
            {
                if (i + 1 < argc && argv[i + 1][0] != '-')
                {   
                    argInfo["configPath"] = argv[i + 1];
                }
                else
                {
                    std::cerr << "Error: --configPath option requires a real value." << std::endl;
                }
            }
        }
        return argInfo;
    }
};