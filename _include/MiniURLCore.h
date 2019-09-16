#ifndef MINI_URL_CORE_H
#define MINI_URL_CORE_H
#include <string>
#include <cmath>
namespace yukiina{
    class MiniUrl{
    public:
        static std::string GeneratorMiniUrl(long id);
    private: 
        // 生成 62 进制字母表
        //static const std::string alphas;
        // 字母表长度
        //static const int N;
        
    };
}
#endif