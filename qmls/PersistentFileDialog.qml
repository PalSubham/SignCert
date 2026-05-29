// SPDX-License-Identifier: GPL-3.0-or-later

import QtCore
import QtQuick
import QtQuick.Dialogs

import SignCertQml

FileDialog {
    id: fileDialog
    property string settingsKey: qsTr("")

    Settings {
        id: settings
        category: fileDialog.settingsKey
        property string lastDirectory: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
    }

    Component.onCompleted: {
        if (FileHandler.dirCheck(settings.lastDirectory.replace("file://", ""))) {
            currentFolder = settings.lastDirectory;
        } else {
            currentFolder = StandardPaths.standardLocations(StandardPaths.HomeLocation)[0];
        }
    }

    onAccepted: settings.lastDirectory = currentFolder
}
