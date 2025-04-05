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
        color: Qt.rgba(0, 0, 0, 0.6)
    }

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

                Label {
                    text: qsTr("Password:")
                    Layout.preferredWidth: 100
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: passwordField
                    Layout.preferredWidth: 400
                    height: parent.height
                    placeholderText: qsTr("Enter Password...")
                }
            }

            Button {
                Layout.preferredWidth: 80
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
