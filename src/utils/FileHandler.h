#pragma once
#include <QString>
#include <stdexcept>
namespace Utils {
class FileIOException : public std::runtime_error { public: using std::runtime_error::runtime_error; };
class FileHandler {
public:
    static QString defaultPath();
    static void write(const QString& path, const QByteArray& json);
    static QByteArray read(const QString& path);
};
}
