#pragma once
#include <string>

class ResourceLogger {
public:
    explicit ResourceLogger(const std::string& filename);
    void log(const std::string& message) const;

private:
    std::string m_filename;
};