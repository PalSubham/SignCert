// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Subham Pal
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    app.setOrganizationName("SignCert");
    app.setApplicationName("SignCert");
    app.setApplicationDisplayName("SignCert");

    QQmlApplicationEngine engine;
    engine.loadFromModule("SignCertQml", "Main");

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
