// SPDX-License-Identifier: GPL-3.0-or-later

import QtCore
import QtQuick
import QtQuick.Dialogs


FolderDialog {
    id: folderDialog
    property string settingsKey: qsTr("")

    Settings {
        id: settings
        category: folderDialog.settingsKey
        property string lastDirectory: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
    }

    Component.onCompleted: {
        if (filehandler.dirCheck(settings.lastDirectory.replace("file://", ""))) {
            currentFolder = settings.lastDirectory;
        } else {
            currentFolder = StandardPaths.standardLocations(StandardPaths.HomeLocation)[0];
        }
    }

    onAccepted: settings.lastDirectory = currentFolder
}
