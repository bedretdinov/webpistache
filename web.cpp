
#include "/usr/local/include/pistache/endpoint.h"
#include "/usr/local/include/pistache/router.h"
#include "/usr/local/include/pistache/http.h"
#include "/usr/local/include/pistache/net.h"
#include <iostream>
#include <fstream>      // std::ifstream

using namespace std;


using namespace Net;





std::string getFileContent (char *file_name) {


  std::ifstream is(file_name);     // open file

  char c;
  std::string content;
  while (is.get(c))
    content.push_back(c);

  is.close();


  return content;
}


namespace Generic {
    void handleReady(const Rest::Request&, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, "1");
    }
}

class StatsEndpoint {
public:
    StatsEndpoint(Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
            .threads(thr);
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

private:
    void setupRoutes() {
        using namespace Rest;

        Routes::Get(router, "/reconmendation/deal/:id", Routes::bind(&StatsEndpoint::doAuth, this));

    }

    void doAuth(const Rest::Request& request, Http::ResponseWriter response) {
        auto id = request.param(":id").as<std::string>();

        std::string file = "/home/pistache/cache/"+id;

        int n = file.length();
        char char_array[n + 1];
        strcpy(char_array, file.c_str());

        std::string content = getFileContent(char_array);

        response.cookies()
            .add(Http::Cookie("lang", "en-US"));
        response.send(Http::Code::Ok, content);
    }


    typedef std::mutex Lock;
    typedef std::lock_guard<Lock> Guard;

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

int main(int argc, char *argv[]) {
    Port port(80);

    int thr = 2;

    if (argc >= 2) {
        port = std::stol(argv[1]);

        if (argc == 3)
            thr = std::stol(argv[2]);
    }

    Address addr(Ipv4::any(), port);

    StatsEndpoint stats(addr);

    stats.init(thr);
    stats.start();
}
