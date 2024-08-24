#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int port = 5400; // Default port

  if (argc != 2) {
    std::cout << "Usage: ./test_client <port>" << std::endl;
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

  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    std::cerr << "Ошибка при создании сокета" << std::endl;
    return 1;
  }

  if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    std::cerr << "Ошибка при подключении к серверу" << std::endl;
    close(sock);
    return 1;
  }

  std::string query = "SELECT 1;";
  send(sock, query.c_str(), query.length(), 0);

  char buffer[256];
  recv(sock, buffer, sizeof(buffer), 0);
  std::cout << "Ответ: " << buffer << std::endl;

  close(sock);
  return 0;
}
