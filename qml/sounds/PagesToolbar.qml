import QtQuick 2.15
import QtQuick.Controls 2.15

import QtQuick.Layouts 1.15

import PageManager 1.0
import Db 1.0

import "../components"

Pane {
    width: parent.width
    padding: 0
    leftPadding: 10
    rightPadding: 10

    RowLayout {
        anchors.fill: parent
        TextField {
            Layout.minimumWidth: 150
            text: PageManager.currentPage.name
            onEditingFinished: {
                PageManager.currentPage.name = text
                Db.save()
            }
        }

        HotkeyButton {
            Layout.fillWidth: true
            uuid: 'currentPage'
            text: PageManager.currentPage.hotkey
            Layout.minimumWidth: 150
        }

        ToolButton {
            id: pagesButton
            text: 'v'
            onClicked: pagesMenu.open()
            Layout.maximumWidth: 35
        }

        Menu {
            id: pagesMenu
            y: pagesButton.height
            x: pagesButton.x

            Repeater {
                model: PageManager.pages
                MenuItem {
                    // repeater item doesn't know about menu or ids
                    property var parentMenu: pagesMenu
                    text: name
                    onClicked: {
                        PageManager.setCurrentPage(index)
                        parentMenu.close()
                    }
                }
            }
        }

        ToolButton {
            id: actionsButton
            text: 'o'
            onClicked: actionsMenu.open()
            Layout.maximumWidth: 35
        }

        Menu {
            id: actionsMenu
            y: actionsButton.height
            x: actionsButton.x

            MenuItem {
                text: 'New Page'
                onClicked: {
                    PageManager.newPage()
                }
            }

            MenuItem {
                text: 'Delete Page'
                onClicked: {
                    PageManager.deletePage()
                }
            }
        }

        ToolButton {
            text: '+'
            Layout.maximumWidth: 35
            onClicked: PageManager.newHotkey()
        }
    }
}
