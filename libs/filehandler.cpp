#include <QDir>

#include "filehandler.h"

FileHandler::FileHandler(QObject *parent)
    : QObject{parent}
{}

bool FileHandler::folderExists(const QString &folder)
{
    return QDir(folder).exists();
}

bool FileHandler::fileExists(const QString &file)
{
    return QFile::exists(file);
}
