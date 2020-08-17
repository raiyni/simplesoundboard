import QtQuick 2.15
import QtQuick.Controls 2.15

import SoundManager 1.0
import Db 1.0

Item {
    id: root
    property string device
    property int defaultIndex
    property int index

    width: parent.width
    height: label.height + box.height

    SettingsLabel {
        text: "Output " + index
        id: label
    }

    ComboBox {
        property bool firstChange: false
        id: box
        anchors.top: label.bottom

        textRole: 'description'
        valueRole: 'id'
        model: SoundManager.deviceList
        width: root.width
        currentIndex: defaultIndex
        onCurrentValueChanged: {
            SoundManager.loadEngine(device, currentValue)
            if (!firstChange)
                firstChange = true
            else
                Db.save()
        }
    }
}
