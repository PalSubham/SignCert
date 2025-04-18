import QtCore
import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls

import "qmls"


ApplicationWindow {
    id: mainWindow
    width: column.implicitWidth + 40
    height: column.implicitHeight + 40
    visible: true
    title: qsTr("Sign Your CSR")

    property bool signing: false

    PersistentFileDialog {
        id: csrSelector
        settingsKey: "csrDialog"
        title: qsTr("Please Choose the CSR")
        fileMode: FileDialog.OpenFile
        nameFilters: [qsTr("CSR file (*.csr)"), qsTr("DER file (*.der)")]
        onAccepted: {
            csrField.text = selectedFile.toString().replace("file://", "")
            statusHelper.appendStatus(statusList, qsTr(`CSR: ${csrField.text}`), statusHelper.info);
        }
    }

    PersistentFileDialog {
        id: caCertSelector
        settingsKey: "caCertDialog"
        title: qsTr("Please Choose the CA certificate")
        fileMode: FileDialog.OpenFile
        nameFilters: [qsTr("CRT file (*.csr)"), qsTr("PEM file (*.pem)"), qsTr("DER file (*.der)")]
        onAccepted: {
            caCertField.text = selectedFile.toString().replace("file://", "")
            statusHelper.appendStatus(statusList, qsTr(`CA: ${caCertField.text}`), statusHelper.info);
        }
    }

    PersistentFileDialog {
        id: caKeySelector
        settingsKey: "caKeyDialog"
        title: qsTr("Please Choose the CA key")
        fileMode: FileDialog.OpenFile
        nameFilters: [qsTr("KEY file (*.key)"), qsTr("PEM file (*.pem)"), qsTr("DER file (*.der)")]
        onAccepted: {
            caKeyField.text = selectedFile.toString().replace("file://", "")
            statusHelper.appendStatus(statusList, qsTr(`CA Key: ${caKeyField.text}`), statusHelper.info);
        }
    }

    PersistentFolderDialog {
        id: outFolderSelector
        settingsKey: "outFolderDialog"
        title: qsTr("Please choose the output directory")
        onAccepted: {
            outFolderField.text = selectedFolder.toString().replace("file://", "")
            statusHelper.appendStatus(statusList, qsTr(`Out Dir: ${outFolderField.text}`), statusHelper.info);
        }
    }

    Status {
        id: statusHelper
    }

    ListModel {
        id: statusList
    }

    PasswordModal {
        id: passwordModal
        anchors.centerIn: parent
        width: mainWindow.width
        margins: 0
    }

    Column {
        id: column
        anchors.margins: 20
        spacing: 15
        anchors.centerIn: parent

        Divider {
            title: qsTr("Inputs")
        }

        Row {
            spacing: 10

            Label {
                text: qsTr("CSR File:")
                width: 100
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }
            TextField {
                id: csrField
                width: 350
                height: parent.height
                placeholderText: qsTr("No CSR selected...")
                readOnly: true
            }
            Button {
                width: 140
                text: qsTr("Browse CSR...")
                onClicked: csrSelector.open()
                contentItem: Text {
                    height: parent.height
                    width: parent.width
                    text: parent.text
                    color: mainWindow.palette.text
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                enabled: !signing
            }
        }

        Row {
            spacing: 10

            Label {
                text: qsTr("CA Cert File:")
                width: 100
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }
            TextField {
                id: caCertField
                width: 350
                height: parent.height
                placeholderText: qsTr("No CA certificate selected...")
                readOnly: true
            }
            Button {
                width: 140
                text: qsTr("Browse CA Cert...")
                onClicked: caCertSelector.open()
                contentItem: Text {
                    height: parent.height
                    width: parent.width
                    text: parent.text
                    color: mainWindow.palette.text
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                enabled: !signing
            }
        }

        Row {
            spacing: 10

            Label {
                text: qsTr("CA Key File:")
                width: 100
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }
            TextField {
                id: caKeyField
                width: 350
                height: parent.height
                placeholderText: qsTr("No CA Key selected...")
                readOnly: true
            }
            Button {
                width: 140
                text: qsTr("Browse CA Key...")
                onClicked: caKeySelector.open()
                contentItem: Text {
                    height: parent.height
                    width: parent.width
                    text: parent.text
                    color: mainWindow.palette.text
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                enabled: !signing
            }
        }

        Row {
            spacing: 10

            Label {
                text: qsTr("Days:")
                width: 100
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }
            SpinBox {
                id: daysField
                from: 1
                to: 3650
                stepSize: 1
                value: 365
                editable: true
                width: 350
                enabled: !signing
            }
            Button {
                width: 140
                text: qsTr("Max")
                onClicked: daysField.value = daysField.to
                contentItem: Text {
                    height: parent.height
                    width: parent.width
                    text: parent.text
                    color: mainWindow.palette.text
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                enabled: !signing
            }
        }

        Divider {
            title: qsTr("Output")
        }

        Row {
            spacing: 10

            Label {
                text: qsTr("Out Directory:")
                width: 100
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }
            TextField {
                id: outFolderField
                width: 350
                height: parent.height
                placeholderText: qsTr("No output directory chosen...")
                readOnly: true
            }
            Button {
                width: 140
                text: qsTr("Choose Directory...")
                onClicked: outFolderSelector.open()
                contentItem: Text {
                    height: parent.height
                    width: parent.width
                    text: parent.text
                    color: mainWindow.palette.text
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                enabled: !signing
            }
        }

        Row {
            spacing: 10

            Label {
                text: qsTr("Out File:")
                width: 100
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }
            TextField {
                id: outFileNameField
                width: 350
                height: parent.height
                placeholderText: qsTr("Enter out file name...")
                enabled: !signing
            }
            ComboBox {
                width: 140
                model: [qsTr(".pem"), qsTr(".der")]
                currentIndex: 0
                enabled: !signing
            }
        }

        Button {
            width: 80
            text: qsTr("Sign")
            anchors.right: parent.right
            contentItem: Text {
                height: parent.height
                width: parent.width
                text: parent.text
                color: mainWindow.palette.text
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            enabled: !signing
            onClicked: {
                let valid = true;

                if (!csrField.text) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("No CSR Selected"), statusHelper.error);
                }
                else if (!filehandler.fileExists(csrField.text)) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("CSR Doesn't Exist"), statusHelper.error);
                }

                if (!caCertField.text) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("No CA Cert Selected"), statusHelper.error);
                }
                else if (!filehandler.fileExists(caCertField.text)) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("CA Cert Doesn't Exist"), statusHelper.error);
                }

                if (!caKeyField.text) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("No CA Key Selected"), statusHelper.error);
                }
                else if (!filehandler.fileExists(caKeyField.text)) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("CA Key Doesn't Exist"), statusHelper.error);
                }

                if (daysField.value < 1 || daysField.value > 3650)
                {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("Invalid Number of Days"), statusHelper.error);
                }

                if (!outFolderField.text) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("No Out Directory Selected"), statusHelper.error);
                }
                else if (!filehandler.folderExists(outFolderField.text)) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("Out Directory Doesn't Exist"), statusHelper.error);
                }

                if (!outFileNameField.text) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("No Out File Mentioned"), statusHelper.error);
                }

                if (valid) {

                }
            }
        }

        Divider {
            title: qsTr("Status")
        }

        Rectangle {
            width: parent.width
            height: 100
            border.color: "gray"
            border.width: 1
            radius: 5
            color: "transparent"

            ListView {
                id: listView
                anchors.fill: parent
                anchors.margins: 5
                clip: true
                spacing: 5
                bottomMargin: 30
                model: statusList
                delegate: Row {
                    width: listView.width
                    spacing: 10

                    Image {
                        source: model.icon
                        width: 16
                        height: 16
                        smooth: true
                    }
                    Text {
                        text: model.msg
                        color: model.color
                        font.pixelSize: 16

                    }
                }
                onCountChanged: positionViewAtEnd()
            }
        }
    }

    Component.onCompleted: {
        statusHelper.appendStatus(statusList, qsTr("Welcome..."), statusHelper.info);
    }
}
