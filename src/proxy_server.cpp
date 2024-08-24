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

  while (true) {
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLength);
    if (clientSocket < 0) {
      std::cerr << "Ошибка при принятии соединения" << std::endl;
      continue;
    }

    // Запускаем новый поток для обработки клиента
    pthread_t thread;
    int* pClientSocket = new int(clientSocket);  // Передаём указатель на сокет
    if (pthread_create(&thread, NULL, handleClient, pClientSocket) != 0) {
      std::cerr << "Ошибка при создании потока для клиента" << std::endl;
      close(clientSocket);
      delete pClientSocket;
    } else {
      pthread_detach(thread);  // Поток отделяется, чтобы автоматически освободить ресурсы после завершения
    }
  }
}

void* ProxyServer::handleClient(void* arg) {
  int clientSock = *(int*)arg;
  char buffer[4096]; // Увеличен размер буфера
  int bytesReceived;

  // Создание сокета для подключения к серверу базы данных
  int dbSock = socket(AF_INET, SOCK_STREAM, 0);
  if (dbSock < 0) {
    std::cerr << "Ошибка при создании сокета для базы данных: " << strerror(errno) << std::endl;
    close(clientSock);
    return nullptr;
  }

  struct sockaddr_in dbAddress;
  dbAddress.sin_family = AF_INET;
  dbAddress.sin_port = htons(5432);  // Порт сервера базы данных
  dbAddress.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP-адрес сервера базы данных

  if (connect(dbSock, (struct sockaddr*)&dbAddress, sizeof(dbAddress)) < 0) {
    std::cerr << "Ошибка при подключении к серверу базы данных: " << strerror(errno) << std::endl;
    close(clientSock);
    close(dbSock);
    return nullptr;
  }

  while (true) {
    // Получение данных от клиента
    bytesReceived = recv(clientSock, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0) {
      std::cerr << "Ошибка при получении данных от клиента: " << strerror(errno) << std::endl;
      break;
    }

    if (bytesReceived == 0) {
      std::cout << "Клиент закрыл соединение." << std::endl;
      break; // Клиент закрыл соединение
    }

    // Вывод полученных данных для отладки
    std::cout << "Полученные данные от клиента: " << std::string(buffer, bytesReceived) << std::endl;

    // Отправка данных на сервер базы данных
    if (send(dbSock, buffer, bytesReceived, 0) < 0) {
      std::cerr << "Ошибка при отправке данных на сервер базы данных: " << strerror(errno) << std::endl;
      break;
    }

    // Получение ответа от сервера базы данных
    int bytesSent = recv(dbSock, buffer, sizeof(buffer), 0);
    if (bytesSent < 0) {
      std::cerr << "Ошибка при получении данных от сервера базы данных: " << strerror(errno) << std::endl;
      break;
    }

    // Вывод полученных данных от сервера базы данных для отладки
    std::cout << "Полученные данные от сервера базы данных: " << std::string(buffer, bytesSent) << std::endl;

    // Отправка ответа обратно клиенту
    if (send(clientSock, buffer, bytesSent, 0) < 0) {
      std::cerr << "Ошибка при отправке данных обратно клиенту: " << strerror(errno) << std::endl;
      break;
    }
  }

  close(clientSock);
  close(dbSock);
  return nullptr;
}
