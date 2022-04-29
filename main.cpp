#include "api/api_server.hpp"
#include <iostream>
#include <fstream>
#include <thread>

int main(int argc, char* argv[]){

    auto apiServer = new ApiServer("127.0.0.1",8080);

    std::thread t1(&ApiServer::startListening, apiServer);

    t1.join();

    return 0;
}