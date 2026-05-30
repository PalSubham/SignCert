// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QFileInfo>

namespace SignCert
{
    inline bool dir_check(const QString &dir_name)
    {
        QFileInfo dir(dir_name);
        return dir.exists() && dir.isDir() && dir.isWritable();
    }

    inline bool file_check(const QString &file_name)
    {
        QFileInfo file(file_name);
        return file.exists() && file.isFile() && file.isReadable();
    }
} // namespace SignCert
