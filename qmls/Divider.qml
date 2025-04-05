import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


Rectangle {
    id: divider
    width: parent.width
    height: 20
    color: "transparent"

    property string title: qsTr("")

    RowLayout {
        anchors.fill: parent
        spacing: 5

        Rectangle {
            Layout.fillWidth: true
            height: 2
            color: "gray"
        }

        Label {
            text: divider.title
            font.bold: true
        }

        Rectangle {
            Layout.fillWidth: true
            height: 2
            color: "gray"
        }
    }
}
