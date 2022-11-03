import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.13 as Kirigami

Window {
    property bool isvertical: direction == 1 || direction == 2 ? true : false
    property int direction: 4 //0->disabled 1->left 2->right 3->up 4->down
    property int settingwidth
    property real settingopacity: 0.7

    id: panel
    visible: true
    width: isvertical == false ? Screen.width : settingwidth
    height: isvertical == false ? settingwidth : Screen.height
    x: direction == 2 ? Screen.width - width : 0
    y: direction == 3 ? 0 : Screen.height - height
    color: Qt.rgba(Kirigami.Theme.backgroundColor.r * settingopacity,
                   Kirigami.Theme.backgroundColor.g * settingopacity,
                   Kirigami.Theme.backgroundColor.b * settingopacity,
                   settingopacity)

    RowLayout {
        id: sampleItem
        objectName: "sampleItem"
        anchors.fill: parent
    }
}
