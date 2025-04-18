import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Popup {
    id: passwordModal
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    height: parent.height
    width: parent.width
    padding: 0
    background: Rectangle {
        color: Qt.rgba(0, 0, 0, 0.7)
    }

    property bool passwordVisible: false

    Rectangle {
        width: passwordColumn.implicitWidth + 40
        height: passwordColumn.implicitHeight + 40
        anchors.centerIn: parent
        color: mainWindow.palette.base
        anchors.margins: 0
        border.color: "gray"
        radius: 8

        ColumnLayout {
            id: passwordColumn
            spacing: 15
            anchors.margins: 20
            anchors.fill: parent

            RowLayout {
                spacing: 10
                Layout.fillWidth: true

                Text {
                    text: qsTr("Password for CA Key")
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.bold: true
                    font.italic: true
                }
            }

            RowLayout {
                spacing: 10
                Layout.fillWidth: true

                Label {
                    text: qsTr("Password:")
                    Layout.preferredWidth: 70
                    Layout.fillHeight: true
                    verticalAlignment: Text.AlignVCenter
                }

                RowLayout {
                    spacing: 5

                    TextField {
                        id: passwordField
                        Layout.preferredWidth: 400
                        Layout.fillHeight: true
                        placeholderText: qsTr("Enter Password...")
                        echoMode: passwordVisible ? TextInput.Normal : TextInput.Password
                    }

                    Button {
                        Layout.fillHeight: true
                        Layout.preferredWidth: height
                        icon.source: passwordVisible ? "qrc:/icons/visibility_off" : "qrc:/icons/visibility"
                        onClicked: passwordVisible = !passwordVisible
                    }
                }
            }

            Button {
                Layout.preferredWidth: 80
                Layout.fillHeight: true
                text: qsTr("Ok")
                contentItem: Text {
                    height: parent.height
                    width: parent.width
                    text: parent.text
                    color: mainWindow.palette.text
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: passwordModal.close()
                Layout.alignment: Qt.AlignRight
            }
        }
    }
}
