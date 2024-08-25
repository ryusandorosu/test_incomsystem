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
  if (logFile.is_open()) {
    logFile << query << std::endl;
  } else {
    std::cerr << "Ошибка: Файл лога не открыт" << std::endl;
  }
}

std::string ProxyServer::trim(const std::string& str) {
  const auto start = str.find_first_not_of(" \t\n\r\f\v");
  if (start == std::string::npos) {
    return "";  // Строка состоит только из пробелов
  }
  const auto end = str.find_last_not_of(" \t\n\r\f\v");
  return str.substr(start, end - start + 1);
}
