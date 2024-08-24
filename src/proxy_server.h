#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/select.h>
#include "logger.h"

class ProxyServer {
public:
    ProxyServer(const char* listen_ip, uint16_t listen_port, const char* db_server_ip, uint16_t db_server_port);
    bool start();

private:
    int create_socket();
    void handle_client(int client_sock, int db_sock);

    const char* listen_ip_;
    uint16_t listen_port_;
    const char* db_server_ip_;
    uint16_t db_server_port_;
    int listen_sock_;
};

#endif // PROXY_SERVER_H
