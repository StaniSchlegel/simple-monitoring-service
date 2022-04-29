#include "api_server.hpp"

ApiServer::ApiServer(std::string host, int port){
    this->host = host;
    this->port = port;
 
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(this->server_socket, 
               SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, 
               &opt, sizeof(opt));

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    this->server_address.sin_port = htons(this->port);
 

    bind(this->server_socket, 
         (struct sockaddr*)&this->server_address, 
         sizeof(this->server_address));
}

std::string ApiServer::buildHTTPResponse(int statusCode, std::string contentType, std::string body){

    // Current date time in GMT date format
    time_t curr_time;
	curr_time = time(NULL);
	tm *tm_gmt = gmtime(&curr_time);
    const char* fmt = "%a, %d %b %Y %T GMT";
    char outstr[200];
    strftime(outstr, sizeof(outstr), fmt, tm_gmt);

    std::string datetime(outstr);

    std::string response = "HTTP/1.1 " + std::to_string(statusCode) +
                           "\nserver: c++\ndate: " + 
                           datetime + 
                           "\ncontent-type: " + contentType + 
                           "\n\n" + body;
    
    return response;
}


void ApiServer::handleRequest(int newSocket){

    char buffer[2048] = { 0 };

    auto valread = read(newSocket, buffer, 2048);
    std::string request(buffer);

    auto tmpPos = request.find_first_of(' ');
    std::string httpMethod = request.substr(0,tmpPos);

    auto tmpPos2 = request.find_first_of(' ', tmpPos+1);
    std::string requestedRoute = request.substr(tmpPos + 2, tmpPos2 - tmpPos - 2);

    std::string response = "";

    if(httpMethod != "GET")
    {
        std::string body = "Error: the server only supports GET requests"; 
        response = this->buildHTTPResponse(405,"application/text", body);

        send(newSocket, response.c_str(), response.length(), 0);

        close(newSocket);
        return;
    }

    if(requestedRoute == "memory"){
        std::string body = MemorySensor::generateJSONOutput();
        response = this->buildHTTPResponse(200,"application/json", body);
    }
    else if(requestedRoute == "cpu"){
        std::string body = CpuSensor::generateJSONOutput();
        response = this->buildHTTPResponse(200,"application/json", body);
    }
    else if(requestedRoute == "disk"){

    }
    else if(requestedRoute == "processes"){

    }
    else {
        response = this->buildHTTPResponse(404,"application/text", "Error: route not found.");
    }

    send(newSocket, response.c_str(), response.length(), 0);

    close(newSocket);
}

void ApiServer::startListening(){

    this->isListening = true;

    int addrlen = sizeof(this->server_address);

    listen(this->server_socket, 10);

    while(this->isListening){
        auto new_socket = accept(this->server_socket, 
                                 (struct sockaddr*)&this->server_address,
                                 (socklen_t*)&addrlen);

        auto th = std::thread(&ApiServer::handleRequest,this, new_socket);
        th.detach();
    }
    
}

void ApiServer::stopListening(){
    
    close(this->server_socket);
    this->isListening = false;
}
