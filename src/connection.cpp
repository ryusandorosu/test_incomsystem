#include "proxy_server.h"

void ProxyServer::handleConnections(int serverSocket) {
<<<<<<< HEAD
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
=======
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    std::cerr << "Ошибка при создании epoll: " << strerror(errno) << std::endl;
    exit(1);
  }

  struct epoll_event event;
  struct epoll_event events[MAX_EVENTS];

  event.events = EPOLLIN;
  event.data.fd = serverSocket;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverSocket, &event) == -1) {
    std::cerr << "Ошибка при добавлении серверного сокета в epoll: " << strerror(errno) << std::endl;
    exit(1);
  }

  while (true) {
    int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (n == -1) {
      std::cerr << "Ошибка при ожидании epoll: " << strerror(errno) << std::endl;
      exit(1);
    }

    for (int i = 0; i < n; i++) {
      if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP) {
        std::cerr << "Ошибка или завершение работы на сокете " << events[i].data.fd << std::endl;
        close(events[i].data.fd);
        continue;
      }

      if (events[i].data.fd == serverSocket) {
        struct sockaddr_in clientAddress;
        socklen_t clientLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLength);
        if (clientSocket < 0) {
          std::cerr << "Ошибка при принятии соединения: " << strerror(errno) << std::endl;
          continue;
        }

        std::cout << "Новое подключение: " << clientSocket << std::endl;

        fcntl(clientSocket, F_SETFL, fcntl(clientSocket, F_GETFL, 0) | O_NONBLOCK);

        event.events = EPOLLIN | EPOLLET;
        event.data.fd = clientSocket;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
          std::cerr << "Ошибка при добавлении клиентского сокета в epoll: " << strerror(errno) << std::endl;
          close(clientSocket);
          continue;
        }
      } else {
        int clientSocket = events[i].data.fd;
        if (events[i].events & EPOLLIN) {
          std::cout << "Событие EPOLLIN на сокете: " << clientSocket << std::endl;

          // Создание потока для обработки клиента
          std::thread clientThread(&ProxyServer::handleClient, clientSocket);
          clientThread.detach();  // Отделение потока для обработки
>>>>>>> 8c1b7f5 (rewriting using epoll, works but problems with sysbench tests)
        }
      }
    }
  }

  close(epoll_fd);
}
