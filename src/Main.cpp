#include <iostream>
#include <thread>
#include <regex>
#include <limits>
#include <chrono>
#include "MiniURLCore.h"
#include "IdCreator.h"
#include "RedisManager.h"
#include "HttpServ.h"

using namespace std;
using namespace yukiina;

const string prefix = "http://192.168.79.140:8080/"; // 短链接服务器地址 (虚拟机测试 φ(>ω<*) )

int main() {
    /* 后台运行服务器 */
    HttpServ serv;
    std::thread t([&serv](){
        serv.Start();
    });
    RedisManager* rm = RedisManager::GetInstance(); // 数据库操作对象 
    SnowflakeIdWorker worker; // ID发号器生成对象
    string input;
    regex r("^(https?|ftp|file)://.+$");
    /* 终端进行url转换操作 */
    while(1) {
        cout << "输入要转换的长链接(输入exit结束程序)>>";
        cin >> input;
        if (input == "exit") {
            cout << "Bye D:)" << endl;
            exit(1);
        }

        if (regex_match(input,r)) {
            string record = rm->GetValue(input);
            /* 数据库中没有对应的短链接, 生成并插入 */
            if (record ==  "") {
                string str = MiniUrl::GeneratorMiniUrl(worker.nextId());
                string shortUrl = prefix + str;
                cout << "生成的短链接, 请保存: " << shortUrl << endl;
                rm->SetKV(str, input); // 短-长, 用来重定向
                rm->SetKV(input, str);         //  长-短, 用来避免重复生成
            } else {
                /* 说明之前生成过, 直接返回 */
                cout << "链接之前已经生成过: " << prefix + record << endl;
            }  
        }
        else {
            cout << "请检查输入的Url, 重新输入"  << endl;
            continue;
        }
    }
    t.join();
    return 0;
}