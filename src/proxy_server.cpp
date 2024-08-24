#include "proxy_server.h"

ProxyServer::ProxyServer(const char* listen_ip, uint16_t listen_port, const char* db_server_ip, uint16_t db_server_port)
    : listen_ip_(listen_ip), listen_port_(listen_port), db_server_ip_(db_server_ip), db_server_port_(db_server_port), listen_sock_(-1) {}

int ProxyServer::create_socket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(listen_port_);
    addr.sin_addr.s_addr = inet_addr(listen_ip_);

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return -1;
    }

    if (listen(sock, SOMAXCONN) < 0) {
        perror("listen");
        close(sock);
        return -1;
    }

    return sock;
}

bool ProxyServer::start() {
    listen_sock_ = create_socket();
    if (listen_sock_ < 0) {
        return false;
    }

    fd_set read_fds;
    int max_fd = listen_sock_;

    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(listen_sock_, &read_fds);

        int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
        if (activity < 0 && errno != EINTR) {
            perror("select");
            break;
        }

        if (FD_ISSET(listen_sock_, &read_fds)) {
            int client_sock = accept(listen_sock_, nullptr, nullptr);
            if (client_sock < 0) {
                perror("accept");
                continue;
            }

            int db_sock = socket(AF_INET, SOCK_STREAM, 0);
            if (db_sock < 0) {
                perror("socket");
                close(client_sock);
                continue;
            }

            sockaddr_in db_addr;
            db_addr.sin_family = AF_INET;
            db_addr.sin_port = htons(db_server_port_);
            db_addr.sin_addr.s_addr = inet_addr(db_server_ip_);

            if (connect(db_sock, (struct sockaddr*)&db_addr, sizeof(db_addr)) < 0) {
                perror("connect");
                close(client_sock);
                close(db_sock);
                continue;
            }

            max_fd = std::max(client_sock, db_sock);

            handle_client(client_sock, db_sock);

            close(client_sock);
            close(db_sock);
        }
    }

    close(listen_sock_);
    return true;
}

void ProxyServer::handle_client(int client_sock, int db_sock) {
    char buffer[4096];
    int bytes_read;

    while ((bytes_read = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
        send(db_sock, buffer, bytes_read, 0);
        
        std::string sql_query(buffer, bytes_read);
        Logger::log_query(sql_query);
    }

    if (bytes_read < 0) {
        perror("recv");
    }
}
