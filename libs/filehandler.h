#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>

class FileHandler : public QObject
{
    Q_OBJECT
public:
    explicit FileHandler(QObject *parent = nullptr);

    Q_INVOKABLE bool folderExists(const QString &folder);
    Q_INVOKABLE bool fileExists(const QString &file);
};

#endif // FILEHANDLER_H
