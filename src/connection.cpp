#include "proxy_server.h"

void ProxyServer::handleConnections(int serverSocket) {
  fd_set masterSet, readSet;
  FD_ZERO(&masterSet);
  FD_ZERO(&readSet);

  FD_SET(serverSocket, &masterSet);
  int maxFd = serverSocket;

  struct sockaddr_in clientAddress;
  socklen_t clientLength = sizeof(clientAddress);

  while (true) {
    readSet = masterSet;

    int activity = select(maxFd + 1, &readSet, nullptr, nullptr, nullptr);
    if (activity < 0 && errno != EINTR) {
      std::cerr << "Ошибка при использовании select" << std::endl;
      break;
    }

    for (int fd = 0; fd <= maxFd; ++fd) {
      if (FD_ISSET(fd, &readSet)) {
        if (fd == serverSocket) {
          // Обработка нового подключения
          int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLength);
          if (clientSocket < 0) {
            std::cerr << "Ошибка при принятии соединения" << std::endl;
            continue;
          }
          fcntl(clientSocket, F_SETFL, O_NONBLOCK);
          FD_SET(clientSocket, &masterSet);
          if (clientSocket > maxFd) {
            maxFd = clientSocket;
          }
        } else {
          // Обработка данных от клиента
          handleClient((void*)&fd);

          // Закрываем соединение и удаляем из набора дескрипторов
          close(fd);
          FD_CLR(fd, &masterSet);
        }
      }
    }
  }
}
