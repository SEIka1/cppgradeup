#pragma once
#include <string>
#include <mutex>

class ResourceLogger {
public:
    explicit ResourceLogger(const std::string& filename, size_t maxSize = 10 * 8388608); //10 Mb
    void log(const std::string& message) const;

private:
    std::string m_filename;
    size_t m_maxSize;
    mutable std::mutex m_mutex;

    void rollLog() const;
};
