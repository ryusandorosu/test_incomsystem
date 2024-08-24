#include "proxy_server.h"

int ProxyServer::createServerSocket(int port) {
  struct sockaddr_in serverAddress;
  int serverSocket;

  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket < 0) {
    std::cerr << "Ошибка при создании сокета" << std::endl;
    exit(1);
  }

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    std::cerr << "Ошибка при привязке к порту" << std::endl;
    exit(1);
  }

  if (listen(serverSocket, 3) < 0) {
    std::cerr << "Ошибка при установке прослушивания" << std::endl;
    exit(1);
  }

  return serverSocket;
}
