#include "../include/ResourceLogger.h"
#include <stdexcept>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

ResourceLogger::ResourceLogger(const std::string& filename, size_t maxSize) : m_filename(filename), m_maxSize(maxSize) {
    if (m_filename.empty()) {
        throw std::invalid_argument("log filename must not be empty");
    }
    if (m_maxSize == 0) {
        throw std::invalid_argument("logfile max size must be greater than 0");
    }
}

void ResourceLogger::rollLog() const {
    namespace fs = std::filesystem;

    if (!fs::exists(m_filename)) return;
    auto fileSize = fs::file_size(m_filename);
    if (fileSize <= m_maxSize) return;

    int index = 1;
    std::string newName;
    do {
        newName = std::to_string(index++) + m_filename;
    } while (fs::exists(newName));

    fs::rename(m_filename, newName);
}

void ResourceLogger::log(const std::string& message) const {
    std::lock_guard<std::mutex> lock(m_mutex);

    rollLog();

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
    if (!out.good()) {
        throw std::runtime_error("failed to write to log file");
    }
}
