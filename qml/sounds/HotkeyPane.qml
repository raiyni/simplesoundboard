import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Universal 2.15
import "../components"

import SoundManager 1.0

ColumnLayout {
    id: root
    width: parent.width

    FileDialog {
        id: file
        title: "Choose a file"
        onAccepted: {
            var z = file.fileUrl.toString()
            model.action = z.replace("file:///", "")
        }
    }

    Item {
        Layout.fillWidth: true
        Layout.minimumHeight: hk.height

        RowLayout {
            id: topRow
            width: parent.width
            height: hk.height
            TextField {
                Layout.minimumWidth: 150
                text: name
                onEditingFinished: {
                    model.name = text
                }
            }

            HotkeyButton {
                id: hk
                Layout.fillWidth: true
                uuid: model.uuid
                text: combo
                onDoubleClick: SoundManager.play(model.action)
            }

            Button {
                id: toggleBtn
                text: 'V'
                onClicked: ParallelAnimation {
                    PropertyAnimation {
                        target: toggleBtn
                        property: 'rotation'
                        to: (toggleBtn.rotation - 180) * -1
                        duration: 0
                    }
                    PropertyAnimation {
                        target: bottomRow
                        property: 'implicitHeight'
                        to: (bottomRow.implicitHeight - fileField.height) * -1
                        duration: 500
                    }
                }

                rotation: 0
            }
        }
    }

    ScrollView {
        id: bottomRow
        implicitWidth: root.width
        implicitHeight: 0
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
        RowLayout {
            width: root.width

            TextField {
                id: fileField
                enabled: false
                text: model.action
                Layout.fillWidth: true
                color: Universal.color(Universal.Steel)
                background: Rectangle {
                    color: fileField.Universal.background
                    border.color: Universal.chromeDisabledLowColor
                    border.width: 2
                }
            }

            Button {
                text: 'Browse'
                onClicked: file.open()
            }
        }
    }
}
