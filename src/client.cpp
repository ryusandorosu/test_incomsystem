#include "proxy_server.h"

<<<<<<< HEAD
void* ProxyServer::handleClient(void* arg) {
  int clientSock = *(int*)arg;

  std::vector<char> buffer(32 * 1024);
=======
void ProxyServer::handleClient(int clientSocket) {
  unsigned long bufferSize = 32 * 1024;
  std::vector<char> buffer(bufferSize);
>>>>>>> 8c1b7f5 (rewriting using epoll, works but problems with sysbench tests)
  int bytesReceived;

  int dbSock = createAndConfigureSocket(5432, false);

  while (true) {
<<<<<<< HEAD
    fd_set readFds, writeFds;
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);

    FD_SET(clientSock, &readFds);
    FD_SET(dbSock, &readFds);

    int maxFd = std::max(clientSock, dbSock);

    int activity = select(maxFd + 1, &readFds, nullptr, nullptr, nullptr);
    if (activity < 0 && errno != EINTR) {
      std::cerr << "Ошибка при использовании select" << std::endl;
      break;
    }

    if (FD_ISSET(clientSock, &readFds)) {
      // Получение данных от клиента
      bytesReceived = recv(clientSock, buffer.data(), buffer.size(), 0);
      if (bytesReceived <= 0) {
        if (bytesReceived < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
          continue;  // Данных нет, продолжаем ждать
        }
        if (bytesReceived < 0) {
          std::cerr << "Ошибка при получении данных от клиента: " << strerror(errno) << std::endl;
        }
        break;
      }

      // Логируем запрос
      std::string data(buffer.data(), bytesReceived);
      if (data[0] == 'Q') {                  // Команда типа Query
        std::string query = data.substr(1);  // Удаляем символ 'Q'
        query.erase(query.begin(),
                    std::find_if(query.begin(), query.end(), [](unsigned char c) {
                      return std::isalpha(c) ||
                             std::isspace(c);  // Оставляем только буквы и пробелы
                    }));
        query.erase(
            std::remove_if(
                query.begin(), query.end(),
                [](unsigned char c) {
                  return !std::isprint(c) &&
                         !std::isspace(
                             c);  // Удаляет непечатаемые символы, кроме пробелов
                }),
            query.end());
        query = trim(query);

        std::cout << "Клиент отправил запрос: " << query << std::endl;
        logSQLQuery(query);
      }

      // Ожидание готовности сервера для записи
      FD_SET(dbSock, &writeFds);
      activity = select(dbSock + 1, nullptr, &writeFds, nullptr, nullptr);
      if (activity > 0 && FD_ISSET(dbSock, &writeFds)) {
        // Отправка данных на сервер базы данных
        if (send(dbSock, buffer.data(), bytesReceived, 0) < 0) {
          std::cerr << "Ошибка при отправке данных на сервер базы данных: " << strerror(errno) << std::endl;
          break;
        }
      }
    }

    if (FD_ISSET(dbSock, &readFds)) {
      // Получение ответа от сервера базы данных
      int bytesSent = recv(dbSock, buffer.data(), buffer.size(), 0);
      if (bytesSent <= 0) {
        if (bytesSent < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
          continue;  // Данных нет, продолжаем ждать
        }
        if (bytesSent < 0) {
          std::cerr << "Ошибка при получении данных от сервера базы данных: " << strerror(errno) << std::endl;
        }
        break;
      }

      // Ожидание готовности клиента для записи
      FD_SET(clientSock, &writeFds);
      activity = select(clientSock + 1, nullptr, &writeFds, nullptr, nullptr);
      if (activity > 0 && FD_ISSET(clientSock, &writeFds)) {
        // Отправка ответа обратно клиенту
        if (send(clientSock, buffer.data(), bytesSent, 0) < 0) {
          std::cerr << "Ошибка при отправке данных обратно клиенту: " << strerror(errno) << std::endl;
          break;
        }
      }
    }
  }

=======
    // Получение данных от клиента
    bytesReceived = recv(clientSocket, buffer.data(), bufferSize, 0);
    if (bytesReceived < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        continue; // Сокет временно не готов
      } else {
        std::cerr << "Ошибка при получении данных от клиента: " << strerror(errno) << std::endl;
        break;
      }
    }
    if (bytesReceived == 0) {
      std::cout << "Клиент закрыл соединение." << std::endl;
      break;
    }

    // Логируем запрос
    std::string data(buffer.data(), bytesReceived);
    if (data[0] == 'Q') { // Команда типа Query
      std::string query = data.substr(1);
      query.erase(query.begin(), std::find_if(query.begin(), query.end(), [](unsigned char c) {
                    return std::isalpha(c) || std::isspace(c);
                  }));
      query.erase(std::remove_if(query.begin(), query.end(), [](unsigned char c) {
                    return !std::isprint(c) && !std::isspace(c);
                  }), query.end());
      query = trim(query);
      std::cout << "Клиент отправил запрос: " << query << std::endl;
      logSQLQuery(query);
    }

    // Отправка данных на сервер базы данных
    if (send(dbSock, buffer.data(), bytesReceived, 0) < 0) {
      std::cerr << "Ошибка при отправке данных на сервер базы данных: " << strerror(errno) << std::endl;
      break;
    }

    // Получение ответа от сервера базы данных
    int bytesSent = recv(dbSock, buffer.data(), bufferSize, 0);
    if (bytesSent < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        continue; // Сокет временно не готов
      } else {
        std::cerr << "Ошибка при получении данных от сервера базы данных: " << strerror(errno) << std::endl;
        break;
      }
    }

    // Отправка ответа обратно клиенту
    if (send(clientSocket, buffer.data(), bytesSent, 0) < 0) {
      std::cerr << "Ошибка при отправке данных обратно клиенту: " << strerror(errno) << std::endl;
      break;
    }
  }

  close(clientSocket);
>>>>>>> 8c1b7f5 (rewriting using epoll, works but problems with sysbench tests)
  close(dbSock);
}
