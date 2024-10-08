#include "proxy_server.h"

int ProxyServer::createServerSocket(int port) {
  return createAndConfigureSocket(port, true);
}

int ProxyServer::createAndConfigureSocket(int port, bool isServer) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    std::cerr << "Ошибка при создании сокета" << std::endl;
    exit(1);
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);

  if (isServer) {
    address.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
      std::cerr << "Ошибка при привязке к порту" << std::endl;
      exit(1);
    }
    if (listen(sock, 3) < 0) {
      std::cerr << "Ошибка при установке прослушивания" << std::endl;
      exit(1);
    }
  } else {
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
      std::cerr << "Ошибка при подключении к серверу базы данных" << std::endl;
      close(sock);
      exit(1);
    }
  }

  return sock;
}
