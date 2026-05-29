// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QtQml/QQmlComponent>

namespace SignCert
{
    class FileHandler : public QObject
    {
        Q_OBJECT
        QML_ELEMENT
        QML_SINGLETON

    public:
        explicit FileHandler(QObject *parent = nullptr);

        Q_INVOKABLE bool dirCheck(const QString &dir_name);
        Q_INVOKABLE bool fileCheck(const QString &file_name);
    };
} // namespace SignCert
