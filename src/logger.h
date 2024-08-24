#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <mutex>

class Logger {
public:
    static void log_query(const std::string& query);

private:
    static std::mutex log_mutex;
    static std::ofstream log_file;
};

#endif // LOGGER_H
