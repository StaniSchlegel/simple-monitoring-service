#pragma once
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <chrono>
#include <ctime>

// Sensors
#include "../sensors/memory_sensor.hpp"
#include "../sensors/cpu_sensor.hpp"

class ApiServer{

public:
    ApiServer(std::string host, int port);
    void startListening();
    void stopListening();
    void handleRequest(int newSocket);
    std::string buildHTTPResponse(int statusCode, std::string contentType, std::string body);

private:
    std::string host;
    int port = 8080;
    int server_socket;
    bool isListening;

    struct sockaddr_in server_address;
};