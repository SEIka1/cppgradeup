#include "../include/ResourceLogger.h"
#include <stdexcept>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>

ResourceLogger::ResourceLogger(const std::string& filename): m_filename(filename) {}

void ResourceLogger::log(const std::string& message) const {

    std::ofstream out(m_filename, std::ios::app);
    std::time_t t = std::time(nullptr);
    std::tm tm_snapshot{};
    #if defined(_WIN32)
        localtime_s(&tm_snapshot, &t);
    #else
        localtime_r(&t, &tm_snapshot);
    #endif

    std::ostringstream oss;
    oss << '[' << (tm_snapshot.tm_year + 1900) << '-'
        << std::setw(2) << std::setfill('0') << (tm_snapshot.tm_mon + 1) << '-'
        << std::setw(2) << std::setfill('0') << tm_snapshot.tm_mday << ' '
        << std::setw(2) << std::setfill('0') << tm_snapshot.tm_hour << ':'
        << std::setw(2) << std::setfill('0') << tm_snapshot.tm_min << ':'
        << std::setw(2) << std::setfill('0') << tm_snapshot.tm_sec << "] "
        << message << '\n';

    out << oss.str();
}
