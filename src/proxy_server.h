#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

class ProxyServer {
 public:
  void start(int port);

 private:
  int createServerSocket(int port);
  static int createAndConfigureSocket(int port, bool isServer);
  static void handleConnections(int serverSocket);
  static void* handleClient(void* arg);
  static void createLogFile();
  static void logSQLQuery(const std::string& query);
  static std::string trim(const std::string& str);

  static std::ofstream logFile;
};

#endif  // PROXY_SERVER_H
