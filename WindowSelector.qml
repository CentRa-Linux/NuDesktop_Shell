import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.13 as Kirigami

Item {
    signal windowActivateRequested(int appnumber, int windownumber)
    signal windowCloseRequested(int appnumber, int windownumber)
    id: windowSelector
    Flickable {
        width: parent.width
        height: parent.height
        contentWidth: row.width
        contentHeight: row.height
        Row {
            id: row
            spacing: Kirigami.Units.largeSpacing
            Repeater {
                id: repeater
                objectName: "repeater"
                AppItem {
                    width: 400
                    height: windowSelector.height
                    itemheight: 40
                    textlist: iconName
                    activeIndex: currentActive
                    appActive: isActive
                    icon.source: "image://windowicon/" + appId
                    icon.width: Kirigami.Units.iconSizes.huge
                    icon.height: Kirigami.Units.iconSizes.huge
                    onActivateClicked: {
                        windowActivateRequested(index, number)
                    }
                    onCloseClicked: {
                        windowCloseRequested(index, number)
                    }
                }
            }
        }
    }
}
