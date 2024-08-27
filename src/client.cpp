#include "proxy_server.h"

void ProxyServer::handleClient(int clientSocket) {
  unsigned long bufferSize = 32 * 1024;
  std::vector<char> buffer(bufferSize);
  int bytesReceived;

  int dbSock = createAndConfigureSocket(5432, false);

  while (true) {
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
  close(dbSock);
}
