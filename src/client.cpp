#include "proxy_server.h"

void* ProxyServer::handleClient(void* arg) {
  int clientSock = *(int*)arg;
  delete (int*)arg;  // Освобождаем память

  const int initialBufferSize = 2; // Начальный размер буфера
  int bufferSize = pow(initialBufferSize, 22);
  std::cout << "bufferSize: " << bufferSize << std::endl;
  char buffer[bufferSize];
  int bytesReceived;

  // Создание сокета для подключения к серверу базы данных
  int dbSock = createAndConfigureSocket(5432, false);

  while (true) {
    // Получение данных от клиента
    bytesReceived = recv(clientSock, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0) {
      std::cerr << "Ошибка при получении данных от клиента: " << strerror(errno)
                << std::endl;
      break;
    }
    if (bytesReceived == 0) {
      std::cout << "Клиент закрыл соединение." << std::endl;
      break;
    }

    // Логируем запрос
    std::string data(buffer, bytesReceived);
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

    // Отправка данных на сервер базы данных
    if (send(dbSock, buffer, bytesReceived, 0) < 0) {
      std::cerr << "Ошибка при отправке данных на сервер базы данных: "
                << strerror(errno) << std::endl;
      break;
    }

    // Получение ответа от сервера базы данных
    int bytesSent = recv(dbSock, buffer, sizeof(buffer), 0);
    if (bytesSent < 0) {
      std::cerr << "Ошибка при получении данных от сервера базы данных: "
                << strerror(errno) << std::endl;
      break;
    }

    // Отправка ответа обратно клиенту
    if (send(clientSock, buffer, bytesSent, 0) < 0) {
      std::cerr << "Ошибка при отправке данных обратно клиенту: "
                << strerror(errno) << std::endl;
      break;
    }
  }

  close(clientSock);
  close(dbSock);
  return nullptr;
}
