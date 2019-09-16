#ifndef REDIS_MANAGER_H
#define REDIS_MANAGER_H
#include <string>
#include <hiredis/hiredis.h>

namespace yukiina {
    class RedisManager {
    public:
        static RedisManager* GetInstance();
        ~RedisManager();
        void SetKV(std::string& key,  std::string& value);
        std::string GetValue(std::string& key);
    private:
        RedisManager(const std::string& ip = "127.0.0.1", const int port = 6379);
        redisContext* rcon;
        static RedisManager* instance;
    };

}
#endif