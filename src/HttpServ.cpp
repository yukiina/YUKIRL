#include "HttpServ.h"
#include <iostream>
#include "RedisManager.h"
#include <string>

using namespace yukiina;
using namespace httplib;

HttpServ::HttpServ(int port_) : port(port_) {
    
}

void HttpServ::Start() {
    srv.Get("/([a-zA-Z0-9]+)", ShortUrlDerict);
    srv.listen("0.0.0.0", port);
}

void HttpServ::ShortUrlDerict(const Request& req, Response& rsp){
    /* 查询对应的长链接 */
    RedisManager* rm = RedisManager::GetInstance();
    std::string url = req.matches[1];
    std::string record = rm->GetValue(url);
    if (record == "") {
        rsp.status = 404;
    } else {
        rsp.status = 302;
        rsp.set_header("Location", record.c_str());
    }
}