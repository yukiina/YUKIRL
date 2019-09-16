#include "RedisManager.h"

using namespace yukiina;

RedisManager* RedisManager::instance = new RedisManager();

RedisManager* RedisManager::GetInstance() {
    return instance;
}
/* 构造函数, 连接redis */
RedisManager::RedisManager(const std::string& ip, const int port) {
    rcon = redisConnect(ip.c_str(), port);
    if (rcon == NULL || rcon->err) {
        if (rcon) {
            printf("%s\n", rcon->errstr);
        } else {
            printf("redis 连接失败\n");
        }
        exit(1);
    }
}

/* 析构函数 */
RedisManager::~RedisManager() {
    redisFree(rcon);
}

/* K-V插入数据库 */
void RedisManager::SetKV(std::string& key, std::string& value) {
    /*SET key value */
    redisCommand(rcon, "SET %s %s", key.c_str(), value.c_str());
}

/* 根据 Key 返回 Value */
std::string RedisManager::GetValue(std::string& key) {
     std::string str = "";
     redisReply *reply = (redisReply* )redisCommand(rcon, "GET %s", key.c_str());
     //return std::string(reply->str);
     if (reply->str == nullptr) {
         return str;
         
     }

     str = reply->str;
     freeReplyObject(reply);
     return str;
}