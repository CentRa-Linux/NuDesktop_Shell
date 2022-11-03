import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.13 as Kirigami

Controls.AbstractButton {
    property var textlist
    property int itemheight
    property int activeIndex
    property bool appActive
    signal closeClicked(int number)
    signal activateClicked(int number)
    signal minimizeClicked(int number)

    id: root
    contentItem: RowLayout {
        Kirigami.Icon {
            source: root.icon.source
            x: Kirigami.Units.largeSpacing
            implicitWidth: root.icon.width
            implicitHeight: root.icon.height
            Layout.alignment: Qt.AlignCenter
        }
        ColumnLayout {
            Flickable {
                id: flickable
                anchors.fill: parent
                contentWidth: listView.width
                contentHeight: listView.count * itemheight + Kirigami.Units.largeSpacing * 2
                flickableDirection: Flickable.VerticalFlick
                ListView {
                    id: listView
                    Layout.alignment: Qt.AlignCenter
                    model: root.textlist
                    delegate: Rectangle {
                        id: rect
                        width: root.width - root.icon.width - Kirigami.Units.largeSpacing
                        Controls.Button {
                            id: text
                            y: index * itemheight + Kirigami.Units.largeSpacing
                            Layout.preferredWidth: rect.width
                            height: itemheight
                            width: rect.width - closebutton.width - Kirigami.Units.largeSpacing
                            text: modelData
                            background: Rectangle {
                                implicitWidth: text.width
                                implicitHeight: text.height
                                opacity: text.down ? 1 : text.hovered ? 0.5 : 0
                                color: Qt.rgba(Kirigami.Theme.highlightColor.r,
                                               Kirigami.Theme.highlightColor.g,
                                               Kirigami.Theme.highlightColor.b,
                                               0.2)
                            }
                            onClicked: {
                                activateClicked(index)
                            }
                        }
                        Controls.Button {
                            Kirigami.Theme.inherit: true
                            id: closebutton
                            icon.name: "window-close-symbolic"
                            x: rect.width - this.width - Kirigami.Units.largeSpacing
                            y: index * itemheight + Kirigami.Units.largeSpacing
                            height: text.height
                            width: height * 1.5
                            background: Rectangle {
                                implicitWidth: closebutton.width
                                implicitHeight: closebutton.height
                                opacity: closebutton.down ? 1 : closebutton.hovered ? 0.5 : 0
                                color: Qt.rgba(Kirigami.Theme.highlightColor.r,
                                               Kirigami.Theme.highlightColor.g,
                                               Kirigami.Theme.highlightColor.b,
                                               0.2)
                            }
                            onClicked: {
                                closeClicked(index)
                            }
                        }
                    }
                }
            }
        }
    }
    background: Rectangle {
        implicitWidth: root.width
        implicitHeight: root.height
        opacity: root.appActive ? 1 : hovered ? pressed ? 0.66 : 0.33 : 0
        color: Qt.rgba(Kirigami.Theme.highlightColor.r,
                       Kirigami.Theme.highlightColor.g,
                       Kirigami.Theme.highlightColor.b, 0.2)
        radius: 5
        border.color: Qt.rgba(Kirigami.Theme.highlightColor.r,
                              Kirigami.Theme.highlightColor.g,
                              Kirigami.Theme.highlightColor.b, 1)
        border.width: 1
    }
}
