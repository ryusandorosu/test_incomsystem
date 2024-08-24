#include "proxy_server.h"

void ProxyServer::start(int port) {
  int serverSocket = createServerSocket(port);

  createLogFile();

  while (true) {
    handleConnections(serverSocket);
  }
}

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

void ProxyServer::handleConnections(int serverSocket) {
  struct sockaddr_in clientAddress;
  socklen_t clientLength = sizeof(clientAddress);
  int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLength);

  if (clientSocket < 0) {
    std::cerr << "Ошибка при принятии соединения" << std::endl;
    return;
  }

  pthread_t thread;
  pthread_create(&thread, NULL, handleClient, (void*)&clientSocket);

  close(clientSocket);
}

void* ProxyServer::handleClient(void* arg) {
  int sock = *(int*)arg;
  char buffer[256];
  int bytesReceived;

  while (true) {
    bytesReceived = recv(sock, buffer, 256, 0);
    if (bytesReceived < 0) {
      std::cerr << "Ошибка при получении данных от клиента" << std::endl;
      break;
    }

    if (bytesReceived == 0) {
      break; // Закончилось соединение
    }

    std::string sqlQuery(buffer, bytesReceived);
    logSQLQuery(sqlQuery);

    send(sock, buffer, bytesReceived, 0);
  }

  close(sock);
  return NULL;
}
