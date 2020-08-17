import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import PageManager 1.0

import "../components"

Pane {
    id: root
    anchors.fill: parent

    PagesToolbar {
        id: toolbar
    }

    ScrollView {
        width: parent.width
        height: parent.height - toolbar.height
        anchors.top: toolbar.bottom
        id: scroll
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ListView {
            anchors.fill: scroll

            model: PageManager.currentPage.hotkeys
            anchors.topMargin: 5
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            spacing: 5

            delegate: HotkeyPane {}
        }
    }
}
