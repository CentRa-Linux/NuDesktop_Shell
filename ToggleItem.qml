import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.13 as Kirigami

Item {
    property bool selected: false
    property bool changed: false
    property bool click: false
    property bool hovered: false
    property bool iconVisible: true
    property bool textVisible: false
    property bool disableable: true
    property bool fill: false
    property int animation: 0 //0->disabled 1->menu 2->search
    property int direction: 0 //0->disabled 1->left 2->right 3->up 4->down
    property string iconname: "icon"
    property int iconsize: Kirigami.Units.iconSizes.smallMedium
    property string textcontent
    Layout.fillWidth: fill
    signal itemEnabled(string mes)
    signal itemDisabled(string mes)
    width: 80
    height: 30

    id: toggleitem
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            if (hovered == true) {
                if (selected == false) {
                    toggleitem.itemEnabled("enabled!")
                    selected = true
                } else {
                    toggleitem.itemDisabled("disabled!")
                    if (disableable == true) {
                        selected = false
                    }
                }
            } else {
                selected == selected
            }
        }
        onPressed: click = true
        onReleased: click = false
        onEntered: hovered = true
        onExited: hovered = false
    }
    ColumnLayout {
        spacing: 0
        Rectangle {
            id: middleRect
            width: toggleitem.width
            height: toggleitem.height
            anchors.bottom: bottomRect.top
            Layout.alignment: Qt.AlignCenter
            color: Kirigami.Theme.highlightColor
            state: hovered ? click ? "selected" : "hovered" : selected ? "hovered" : "nothovered"
            states: [
                State {
                    name: "nothovered"
                    PropertyChanges {
                        target: middleRect
                        opacity: 0
                    }
                },
                State {
                    name: "hovered"
                    PropertyChanges {
                        target: middleRect
                        opacity: 0.1
                    }
                },
                State {
                    name: "selected"
                    PropertyChanges {
                        target: middleRect
                        opacity: 0.2
                    }
                }
            ]
            transitions: Transition {
                PropertyAnimation {
                    properties: "opacity"
                    easing.type: Easing.OutQuad
                    duration: Kirigami.Units.shortDuration
                }
            }
        }
    }
    Rectangle {
        id: topRect
        width: toggleitem.width
        anchors.top: toggleitem.top
        Layout.alignment: Qt.AlignCenter
        opacity: 1
        color: Kirigami.Theme.highlightColor
        state: selected ? "selected" : "notselected"
        visible: direction == 3
        states: [
            State {
                name: "notselected"
                PropertyChanges {
                    target: topRect
                    height: 0
                }
            },
            State {
                name: "selected"
                PropertyChanges {
                    target: topRect
                    height: 2
                }
            }
        ]
        transitions: Transition {
            PropertyAnimation {
                properties: "height"
                easing.type: Easing.OutQuad
                duration: Kirigami.Units.shortDuration
            }
        }
    }
    Rectangle {
        id: bottomRect
        width: toggleitem.width
        anchors.bottom: toggleitem.bottom
        Layout.alignment: Qt.AlignCenter
        opacity: 1
        color: Kirigami.Theme.highlightColor
        state: selected ? "selected" : "notselected"
        visible: direction == 4
        states: [
            State {
                name: "notselected"
                PropertyChanges {
                    target: bottomRect
                    height: 0
                }
            },
            State {
                name: "selected"
                PropertyChanges {
                    target: bottomRect
                    height: 2
                }
            }
        ]
        transitions: Transition {
            PropertyAnimation {
                properties: "height"
                easing.type: Easing.OutQuad
                duration: Kirigami.Units.shortDuration
            }
        }
    }
    Rectangle {
        id: leftRect
        height: toggleitem.height
        anchors.left: toggleitem.left
        Layout.alignment: Qt.AlignCenter
        opacity: 1
        color: Kirigami.Theme.highlightColor
        state: selected ? "selected" : "notselected"
        visible: direction == 1
        states: [
            State {
                name: "notselected"
                PropertyChanges {
                    target: leftRect
                    width: 0
                }
            },
            State {
                name: "selected"
                PropertyChanges {
                    target: leftRect
                    width: 2
                }
            }
        ]
        transitions: Transition {
            PropertyAnimation {
                properties: "width"
                easing.type: Easing.OutQuad
                duration: Kirigami.Units.shortDuration
            }
        }
    }
    Rectangle {
        id: rightRect
        height: toggleitem.height
        anchors.right: toggleitem.right
        Layout.alignment: Qt.AlignCenter
        opacity: 1
        color: Kirigami.Theme.highlightColor
        state: selected ? "selected" : "notselected"
        visible: direction == 2
        states: [
            State {
                name: "notselected"
                PropertyChanges {
                    target: rightRect
                    width: 0
                }
            },
            State {
                name: "selected"
                PropertyChanges {
                    target: rightRect
                    width: 2
                }
            }
        ]
        transitions: Transition {
            PropertyAnimation {
                properties: "width"
                easing.type: Easing.OutQuad
                duration: Kirigami.Units.shortDuration
            }
        }
    }
    RowLayout {
        anchors.centerIn: parent
        spacing: Kirigami.Units.smallSpacing
        Kirigami.Icon {
            id: menuIcon
            visible: iconVisible
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            implicitHeight: iconsize
            implicitWidth: iconsize
            source: iconname
            state: toggleitem.selected ? "selected" : "notselected"
            states: [
                State {
                    name: "notselected"
                    PropertyChanges {
                        target: menuIcon
                        rotation: 0
                        scale: 1.0
                    }
                },
                State {
                    name: "selected"
                    PropertyChanges {
                        target: menuIcon
                        rotation: animation == 1 ? 180 : 0
                        scale: animation == 2 ? 1.2 : 1.0
                    }
                }
            ]
            transitions: Transition {
                PropertyAnimation {
                    properties: "scale,rotation"
                    easing.type: Easing.OutQuad
                    duration: Kirigami.Units.shortDuration
                }
            }
        }
        Controls.Label {
            id: menuText
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: textcontent
            color: Kirigami.Theme.textColor
            font: Kirigami.Theme.defaultFont
            visible: textVisible
        }
    }
}
