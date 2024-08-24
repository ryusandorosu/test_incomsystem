#include "proxy_server.h"

void ProxyServer::handleConnections(int serverSocket) {
  struct sockaddr_in clientAddress;
  socklen_t clientLength = sizeof(clientAddress);

  while (true) {
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLength);
    if (clientSocket < 0) {
      std::cerr << "Ошибка при принятии соединения" << std::endl;
      continue;
    }

    // Запускаем новый поток для обработки клиента
    pthread_t thread;
    int* pClientSocket = new int(clientSocket);  // Передаем указатель на сокет
    if (pthread_create(&thread, NULL, ProxyServer::handleClient, pClientSocket) != 0) {
      std::cerr << "Ошибка при создании потока для клиента" << std::endl;
      close(clientSocket);
      delete pClientSocket;
    } else {
      pthread_detach(thread);  // Поток отделяется, чтобы автоматически освободить ресурсы после завершения
    }
  }
}
