import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.13 as Kirigami

Window {
    id: window
    property real settingopacity: 0.7
    property int direction: 4 //0->disabled 1->left 2->right 3->up 4->down
    property int offset: 0
    property bool show: false
    property bool horizontalexpand
    property bool verticalexpand
    signal windowhidden(string mes)

    visible: false
    visibility: "Hidden"
    color: Qt.rgba(Kirigami.Theme.backgroundColor.r * settingopacity,
                   Kirigami.Theme.backgroundColor.g * settingopacity,
                   Kirigami.Theme.backgroundColor.b * settingopacity,
                   settingopacity)

    width: horizontalexpand ? Screen.desktopAvailableWidth : width
    height: verticalexpand ? Screen.desktopAvailableHeight : height

    onActiveChanged: {
        if (!window.active) {
            window.show = false
            window.windowhidden("hidden")
        }
    }

    onShowChanged: {
        if (show == true) {
            visible = true
        }
        if (show == false) {
            visible = false
        }
    }

    Item {
        id: sampleItem
        objectName: "sampleItem"
    }
}
