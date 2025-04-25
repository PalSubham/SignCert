#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>

namespace SignCert {

class FileHandler : public QObject
{
    Q_OBJECT
public:
    explicit FileHandler(QObject *parent = nullptr);

    Q_INVOKABLE bool dirCheck(const QString &dir_name);
    Q_INVOKABLE bool fileCheck(const QString &file_name);
};

}

#endif // FILEHANDLER_H
