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
    auto now = std::chrono::steady_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}