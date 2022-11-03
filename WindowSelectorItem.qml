import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.13 as Kirigami

Item {
    property int itemwidth: 60
    property bool fill: false
    property bool group: true
    required property var windowModel
    signal windowActivateRequested(int appnumber, int windownumber)
    signal windowMinimizeRequested(int appnumber, int windownumber)
    id: windowSelectorItem
    Layout.fillWidth: fill
    width: 600
    height: 30
    clip: true

    Flickable {
        width: windowSelectorItem.width
        height: windowSelectorItem.height
        contentWidth: row.width
        contentHeight: row.height
        Row {
            id: row
            spacing: Kirigami.Units.smallSpacing
            onWidthChanged: {
                print(windowSelectorItem.width)
            }
            Repeater {
                id: repeater
                objectName: "repeater"
                model: windowModel
                Row {
                    property bool active: isActive
                    property int activeIndex: currentActive
                    property int appIndex: index
                    onActiveChanged: {
                        toggleitem.selected = active
                    }
                    ToggleItem {
                        id: toggleitem
                        width: itemwidth
                        height: windowSelectorItem.height
                        iconname: "image://windowicon/" + appId
                        direction: 4
                        onSelectedChanged: {
                            if (iconName.length !== 1) {
                                if (selected == true) {
                                    if (activeIndex + 1 !== iconName.length) {
                                        windowActivateRequested(appIndex,
                                                                activeIndex + 1)
                                    } else {
                                        windowActivateRequested(appIndex, 0)
                                    }
                                } else {
                                    windowActivateRequested(appIndex,
                                                            activeIndex)
                                }
                            } else {
                                if (selected == true) {
                                    windowActivateRequested(appIndex, 0)
                                } else {
                                    windowMinimizeRequested(appIndex, 0)
                                }
                            }
                        }
                        Component.onCompleted: {
                            if (active === true) {
                                selected = true
                            }
                        }
                    }
                    id: row2
                    Repeater {
                        model: iconName
                        Rectangle {
                            id: rect
                            width: itemwidth + 100
                            height: windowSelectorItem.height
                            color: "#00000000"
                            visible: group
                            Controls.Button {
                                id: text
                                Layout.preferredWidth: rect.width
                                height: windowSelectorItem.height
                                width: rect.width
                                text: modelData
                                background: Column {
                                    Rectangle {
                                        id: topRect
                                        implicitWidth: text.width
                                        implicitHeight: text.height - 2
                                        opacity: text.down ? 1 : text.hovered ? 0.5 : 0
                                        color: Qt.rgba(
                                                   Kirigami.Theme.highlightColor.r,
                                                   Kirigami.Theme.highlightColor.g,
                                                   Kirigami.Theme.highlightColor.b,
                                                   0.2)
                                    }
                                    Rectangle {
                                        id: bottomRect
                                        implicitWidth: text.width
                                        implicitHeight: 2
                                        opacity: 1
                                        color: active
                                               && activeIndex == index ? Kirigami.Theme.highlightColor : "#00000000"
                                    }
                                }
                                onClicked: {
                                    if (active && activeIndex == index) {
                                        windowMinimizeRequested(appIndex, index)
                                    } else {
                                        windowActivateRequested(appIndex, index)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
