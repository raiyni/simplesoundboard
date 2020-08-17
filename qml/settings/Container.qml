import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import KeyManager 1.0
import SoundManager 1.0

import "../components"

Pane {
    id: root
    anchors.fill: parent

    Item {
        id: wrapper
        width: root.width - 20

        DeviceChooser {
            id: device1
            y: 10
            device: "device1"
            defaultIndex: SoundManager.indexOf(SoundManager.device1)
            index: 1
        }

        DeviceChooser {
            id: device2
            anchors.top: device1.bottom
            anchors.topMargin: 10
            device: "device2"
            defaultIndex: SoundManager.indexOf(SoundManager.device2)
            index: 2
        }

        SettingsLabel {
            id: lastPage
            text: "Previous Page"
            anchors.top: device2.bottom
            anchors.topMargin: 10
        }

        HotkeyButton {
            id: lastPageHotkey
            anchors.top: lastPage.bottom
            width: wrapper.width
            text: KeyManager.previousPage
            uuid: "previousPage"
        }

        SettingsLabel {
            id: nextPage
            anchors.top: lastPageHotkey.bottom
            anchors.topMargin: 10
            text: "Next Page"
        }

        HotkeyButton {
            id: nextPageHotkey
            anchors.top: nextPage.bottom
            width: wrapper.width
            text: KeyManager.nextPage
            uuid: "nextPage"
        }
    }
}
