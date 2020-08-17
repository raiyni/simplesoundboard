import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Window 2.12
import QtQuick.Layouts 1.15
import QtQuick.Controls.Universal 2.15

import "settings" as Settings
import "sounds" as Sounds

Window {
    id: root
    visible: true
    width: 500
    height: 480
    title: "Simple Soundboard"
    minimumWidth: 450

    TabBar {
        id: tabBar
        width: parent.width
        TabButton {
            text: "Sounds"
        }
        TabButton {
            text: "Settings"
        }
    }

    SwipeView {
        id: view
        width: parent.width
        height: parent.height - tabBar.height
        y: tabBar.height
        currentIndex: tabBar.currentIndex
        interactive: false

        Item {
            Sounds.Container {}
        }

        Item {
            Settings.Container {}
        }
    }
}
