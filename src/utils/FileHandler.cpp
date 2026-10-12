#include "FileHandler.h"
#include <QFile>
#include <QDir>
namespace Utils {
QString FileHandler::defaultPath(){return QDir::current().filePath("scapms-data.json");}
void FileHandler::write(const QString& path,const QByteArray& json){QFile f(path);if(!f.open(QIODevice::WriteOnly|QIODevice::Truncate))throw FileIOException(("Cannot write data file: "+f.errorString()).toStdString());if(f.write(json)!=json.size())throw FileIOException(("Incomplete write: "+f.errorString()).toStdString());}
QByteArray FileHandler::read(const QString& path){QFile f(path);if(!f.exists())return {};if(!f.open(QIODevice::ReadOnly))throw FileIOException(("Cannot read data file: "+f.errorString()).toStdString());return f.readAll();}
}
