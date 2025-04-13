#include "Util.h"

void Util::clearFile(const std::string &filepath)
{
    std::ofstream file(filepath, std::ios::trunc);
    if (!file.is_open())
    {
        throw std::runtime_error("can not open file.");
    }

    file.close();
    std::cout << filepath << " The file content has been cleared." << std::endl;
}


int Util::gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int Util::gcdMultiple(const int nums[], int size) {
    if (size == 0) return 0;
    int result = nums[0];
    for (int i = 1; i < size; i++) {
        result = gcd(result, nums[i]);
    }
    std::cout << "gcdMultiple: " << result << std::endl;
    return result;
}

int Util::getDigit(int number)
{
    int count = 0;
    while (number != 0)
    {
        number /= 10;
        count++;
    }
    return count;
}


// 获取当前毫秒级时间戳
long long Util::getCurrentTimeMillis() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}


json Util::parseArgs(int argc, char* argv[]){
    json args;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.find("--") == 0) {
            std::string key = arg.substr(2);
            if(argv[i+1] != nullptr && argv[i+1][0] != '-'){
                std::string value = argv[i+1];
                args[key] = value;
            }
        }
    }
    return args;
}