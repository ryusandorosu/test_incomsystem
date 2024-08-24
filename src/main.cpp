#include "proxy_server.h"

int main() {
    const char* listen_ip = "0.0.0.0"; // Слушаем все интерфейсы
    uint16_t listen_port = 5432; // Стандартный порт PostgreSQL
    const char* db_server_ip = "127.0.0.1";
    uint16_t db_server_port = 5433; // Сервер может быть настроен на другом порту

    ProxyServer server(listen_ip, listen_port, db_server_ip, db_server_port);
    
    if (!server.start()) {
        std::cerr << "Failed to start proxy server." << std::endl;
        return 1;
    }
    
    return 0;
}
