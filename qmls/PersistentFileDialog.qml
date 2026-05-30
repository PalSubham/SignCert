// SPDX-License-Identifier: GPL-3.0-or-later

import QtCore
import QtQuick
import QtQuick.Dialogs

FileDialog {
    id: fileDialog
    property string settingsKey: qsTr("")

    Settings {
        id: settings
        category: fileDialog.settingsKey
        property string lastDirectory: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
    }

    Component.onCompleted: currentFolder = settings.lastDirectory

    onAccepted: settings.lastDirectory = currentFolder
}
