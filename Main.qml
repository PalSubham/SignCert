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
    property bool dataPresent: false

    PersistentFileDialog {
        id: csrSelector
        settingsKey: "csrDialog"
        title: qsTr("Please Choose the CSR")
        fileMode: FileDialog.OpenFile
        nameFilters: [qsTr("CSR file (*.csr)"), qsTr("DER file (*.der)")]
        onAccepted: {
            csrField.text = selectedFile.toString().replace("file://", "");
            statusHelper.appendStatus(statusList, qsTr(`CSR: ${csrField.text}`), statusHelper.info);
        }
    }

    PersistentFileDialog {
        id: caCertSelector
        settingsKey: "caCertDialog"
        title: qsTr("Please Choose the CA certificate")
        fileMode: FileDialog.OpenFile
        nameFilters: [qsTr("CRT file (*.crt)"), qsTr("PEM file (*.pem)"), qsTr("DER file (*.der)")]
        onAccepted: {
            caCertField.text = selectedFile.toString().replace("file://", "");
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
            caKeyField.text = selectedFile.toString().replace("file://", "");
            statusHelper.appendStatus(statusList, qsTr(`CA Key: ${caKeyField.text}`), statusHelper.info);
        }
    }

    PersistentFolderDialog {
        id: outFolderSelector
        settingsKey: "outFolderDialog"
        title: qsTr("Please choose the output directory")
        onAccepted: {
            outFolderField.text = selectedFolder.toString().replace("file://", "");
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

    Connections {
        target: controller

        function onNeedPassword() {
            passwordModal.open();
        }

        function onFinished() {
            signing = false;
        }

        function onInfo(msg) {
            statusHelper.appendStatus(statusList, qsTr(msg), statusHelper.info);
        }

        function onWarn(msg) {
            statusHelper.appendStatus(statusList, qsTr(msg), statusHelper.warn);
        }

        function onError(msg) {
            statusHelper.appendStatus(statusList, qsTr(msg), statusHelper.error);
        }

        function onDebug(msg) {
            console.log(msg);
        }
    }

    Timer {
        id: debounceTimer
        interval: 500 // ms
        repeat: false
        onTriggered: {
            dataPresent = csrField.text.length > 0 &&
                          caCertField.text.length > 0 &&
                          caKeyField.text.length > 0 &&
                          outFolderField.text.length > 0 &&
                          outFileNameField.text.length > 0;
        }
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
                onTextChanged: debounceTimer.restart()
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
                onTextChanged: debounceTimer.restart()
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
                onTextChanged: debounceTimer.restart()
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
                validator: IntValidator {
                    locale: daysField.locale.name
                    bottom: daysField.from
                    top: daysField.to
                }
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
                onTextChanged: debounceTimer.restart()
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
                onTextChanged: debounceTimer.restart()
            }
            ComboBox {
                id: outExtnField
                width: 140
                textRole: "text"
                valueRole: "value"
                model: [
                    {value: true, text: qsTr("PEM (.crt)")},
                    {value: false, text: qsTr("DER (.der)")}
                ]
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
            enabled: !signing && dataPresent
            onClicked: {
                let valid = true;

                if (!filehandler.fileCheck(csrField.text)) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("Invalid CSR"), statusHelper.error);
                }

                if (!filehandler.fileCheck(caCertField.text)) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("Invalid CA Cert"), statusHelper.error);
                }

                if (!filehandler.fileCheck(caKeyField.text)) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("Invalid CA Key"), statusHelper.error);
                }

                if (!filehandler.dirCheck(outFolderField.text)) {
                    valid = false;
                    statusHelper.appendStatus(statusList, qsTr("Invalid Out Directory"), statusHelper.error);
                }

                if (valid) {
                    signing = true;
                    controller.startSigning(
                        csrField.text,
                        caCertField.text,
                        caKeyField.text,
                        daysField.value,
                        outFolderField.text,
                        outFileNameField.text,
                        outExtnField.currentValue
                    );
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
