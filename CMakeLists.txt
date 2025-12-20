# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025 Subham Pal
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

cmake_minimum_required(VERSION 3.16)

project(SignCert VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.10)

set(CMAKE_AUTORCC ON)
set(CMAKE_SKIP_RPATH TRUE)
set(CMAKE_SKIP_INSTALL_RPATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

find_library(BOTAN_LIBRARIES NAMES botan-3)

qt_add_executable(SignCert
    main.cpp
    resources/Icons.qrc
)

qt_add_qml_module(SignCert
    URI SignCertQml
    VERSION 1.0

    QML_FILES
        Main.qml
        qmls/Divider.qml
        qmls/Status.qml
        qmls/PersistentFileDialog.qml
        qmls/PersistentFolderDialog.qml
        qmls/PasswordModal.qml

    SOURCES
        libs/filehandler.hpp
        libs/filehandler.cpp
        libs/csrsigner.hpp
        libs/csrsigner.cpp
        libs/controller.hpp
        libs/controller.cpp
)

target_link_libraries(SignCert
    PRIVATE ${BOTAN_LIBRARIES} Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS SignCert
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
