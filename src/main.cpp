#include "proxy_server.h"

int main(int argc, char** argv) {
  int port = 8080; // Default port

  if (argc != 2) {
    std::cout << "Usage: ./PostgreSQLProxy <port>" << std::endl;
    return 1;
  }

  try {
    port = std::stoi(argv[1]);
  } catch (const std::invalid_argument& e) {
    std::cerr << "Ошибка: Порт должен быть целым числом." << std::endl;
    return 1;
  } catch (const std::out_of_range& e) {
    std::cerr << "Ошибка: Порт должен быть в диапазоне от 0 до 65535." << std::endl;
    return 1;
  }

  ProxyServer server;
  server.start(port);

  return 0;
}