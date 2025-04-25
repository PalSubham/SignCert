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

    Component.onCompleted: {
        if (settings.lastDirectory && filehandler.fileCheck(settings.lastDirectory.replace("file://", ""))) {
            currentFolder = settings.lastDirectory;
        } else {
            currentFolder = StandardPaths.standardLocations(StandardPaths.HomeLocation)[0];
        }
    }

    onAccepted: settings.lastDirectory = currentFolder
}
