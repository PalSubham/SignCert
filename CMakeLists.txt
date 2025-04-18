cmake_minimum_required(VERSION 3.16)

project(SignCert VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

set(CMAKE_AUTORCC ON)

qt_add_executable(appSignCert
    main.cpp
    resources/Icons.qrc
)

qt_add_qml_module(appSignCert
    URI SignCert
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES
        QML_FILES qmls/Divider.qml
        QML_FILES qmls/Status.qml
        QML_FILES qmls/PersistentFileDialog.qml
        QML_FILES qmls/PersistentFolderDialog.qml
        SOURCES libs/filehandler.h libs/filehandler.cpp
        QML_FILES qmls/PasswordModal.qml
        SOURCES libs/csrsigner.h libs/csrsigner.cpp
        SOURCES libs/signcertcontroller.h libs/signcertcontroller.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appSignCert PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appSignCert
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appSignCert
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appSignCert
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
