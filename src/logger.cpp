#include "logger.h"

std::mutex Logger::log_mutex;
std::ofstream Logger::log_file("logs/sql_queries.log", std::ios::app);

void Logger::log_query(const std::string& query) {
    std::lock_guard<std::mutex> lock(log_mutex);
    if (log_file.is_open()) {
        log_file << query << std::endl;
    } else {
        std::cerr << "Failed to open log file." << std::endl;
    }
}
