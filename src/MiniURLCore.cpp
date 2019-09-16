#include "MiniURLCore.h"
#include <limits>
#include <iostream>
#include <vector>

using namespace yukiina;

static std::vector<char> alphas{'q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k','l','z','x','c','v','b','n','m','0','1','2','3','4','5','6','7','8','9','Q','W','E','R','T','Y','U','I','O','P','A','S','D','F','G','H','J','K','L','Z','X','C','V','B','N','M'};

std::string MiniUrl::GeneratorMiniUrl(long id){
    // 生成了非法的id, 程序结束
    if (id < 0 || id > std::numeric_limits<long>::max()){
        std::cerr << "id error, exit..." << std::endl;
        exit(1);
    }
    //std::cout << id << ": ";
    // 得到id 转为62进制的结果
    std::string result = "";
    while (id != 0) {
        result += alphas[(id % 62)];
        id /= 62;
    }

    // 为了美观, 不足6位的短链接补足 6位
    while (result.size() < 6) {
            result += "0";
    }
    //std::cout << result << std::endl;
    return result;
}