import QtQuick 2.15
import QtQuick.Controls 2.15
import KeyManager 1.0

Button {
    property string uuid
    property string activity

    signal doubleClick

    text: "undefined"
    font.pointSize: 12

    Timer {
        id: timer
        interval: 300
        onTriggered: singleClick()
    }

    onPressed: {
        if (timer.running) {
            doubleClick()
            timer.stop()
        } else {
            timer.restart()
        }
    }

    function singleClick() {
        KeyManager.acceptHotkey(uuid)
    }
}
