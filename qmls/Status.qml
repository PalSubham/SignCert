import QtQuick

QtObject {
    id: statusHelper

    readonly property var info: {"color": "green", "icon": "qrc:/icons/info_icon"}
    readonly property var warn: {"color": "#f39c12", "icon": "qrc:/icons/warn_icon"}
    readonly property var error: {"color": "red", "icon": "qrc:/icons/error_icon"}

    function appendStatus(statusList, msg, type) {
        statusList.append({
            "msg": qsTr(msg),
            "icon": type.icon,
            "color": type.color
        });
    }
}
