#include "../include/ResourceLogger.h"
#include <stdexcept>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>

ResourceLogger::ResourceLogger(const std::string& filename)
    : m_filename(filename) {
    if (m_filename.empty()) {
        throw std::invalid_argument("log filename must not be empty");
    }
}

void ResourceLogger::log(const std::string& message) const {
    std::lock_guard<std::mutex> lock(m_mutex);

    std::ofstream out(m_filename, std::ios::app);
    if (!out.is_open()) throw std::runtime_error("failed to open log file");

    std::time_t now = std::time(nullptr);
    std::tm tm_snapshot{};

    if (localtime_r(&now, &tm_snapshot) == nullptr) {
        throw std::runtime_error("localtime_r failed");
    }

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
