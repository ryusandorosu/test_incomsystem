#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <thread>

class ProxyServer {
public:
  void start(int port);

private:
  int createServerSocket(int port);
  static void handleConnections(int serverSocket);
  static void* handleClient(void*);
  static void createLogFile();
  static void logSQLQuery(const std::string& query);
  static std::ofstream& getLogFile();

  static std::ofstream logFile;
};

#endif  // PROXY_SERVER_H
