#include "proxy_server.h"

std::ofstream ProxyServer::logFile;

void ProxyServer::createLogFile() {
  logFile.open("sql_queries.log", std::ios_base::out | std::ios_base::app);
  if (!logFile.is_open()) {
    std::cerr << "Ошибка при открытии файла для логирования" << std::endl;
    exit(1);
  }
}

void ProxyServer::logSQLQuery(const std::string& query) {
  logFile << query << std::endl;
}
