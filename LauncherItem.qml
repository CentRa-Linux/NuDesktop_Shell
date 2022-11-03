import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.13 as Kirigami

Item {
    id: root
    property int normalWidth
    property int expandedWidth
    property string text
    property string iconName
    property int iconWidth
    property int iconHeight
    property bool expandButtonVisible: false
    property bool expanded: expandButton.selected
    property int expandButtonWidth: 60
    signal appButtonPressed
    width: expanded ? expandedWidth : normalWidth

    Row {
        Controls.AbstractButton {
            width: normalWidth - expandButtonWidth
            height: root.height
            contentItem: Item {
                id: wrapper
                width: root.width - expandButtonWidth
                height: appIcon.height + ctext.height
                anchors.verticalCenter: parent.verticalCenter
                Column {
                    spacing: Kirigami.Units.largeSpacing
                    Kirigami.Icon {
                        id: appIcon
                        x: Kirigami.Units.largeSpacing
                        source: iconName
                        width: iconWidth
                        height: iconHeight
                    }
                    Text {
                        id: ctext
                        x: Kirigami.Units.largeSpacing
                        width: root.width - expandButtonWidth
                        text: root.text
                        elide: Text.ElideRight
                    }
                }
            }
            background: Rectangle {
                width: root.width - expandButtonWidth
                height: root.height
                opacity: parent.hovered ? parent.pressed ? 0.2 : 0.1 : 0
                color: parent.hovered ? Kirigami.Theme.highlightColor : Kirigami.Theme.backgroundColor
            }
            onPressed: {
                appButtonPressed()
            }
        }
        ToggleItem {
            id: expandButton
            width: expandButtonWidth
            height: root.height
            direction: 2
            iconname: "go-next"
            iconsize: Kirigami.Units.iconSizes.medium
        }
    }
    PropertyAnimation {
        target: root
        properties: "width"
        easing.type: Easing.InOutQuad
        duration: 500
    }
}
