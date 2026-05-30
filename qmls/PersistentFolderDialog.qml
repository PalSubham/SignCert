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

    Component.onCompleted: currentFolder = settings.lastDirectory

    onAccepted: settings.lastDirectory = currentFolder
}
