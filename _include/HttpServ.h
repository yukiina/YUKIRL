#ifndef HTTP_SERV_H
#define HTTP_SERV_H
#include "httplib.h"

namespace yukiina {
    class HttpServ {
    public:
        HttpServ(int port_ = 8080);
        void Start();
    private:
        static void ShortUrlDerict(const httplib::Request& req, httplib::Response& rsp);
    private:
        httplib::Server srv;
        int port;
    };
}

#endif