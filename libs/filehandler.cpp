#include <QDir>
#include <QFileInfo>

#include "filehandler.hpp"

using namespace SignCert;

FileHandler::FileHandler(QObject *parent)
    : QObject{parent}
{}

bool FileHandler::dirCheck(const QString &dir_name)
{
    QFileInfo dir(dir_name);
    return dir.isDir() && dir.isWritable();
}

bool FileHandler::fileCheck(const QString &file_name)
{
    QFileInfo file(file_name);
    return file.isFile() && file.isReadable();
}
