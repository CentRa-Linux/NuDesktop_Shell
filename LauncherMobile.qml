import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.13 as Kirigami

Item {
    id: root
    property int itemwidth: 250
    property int itemheight: 120
    property int catitemheight: 80
    property int columncount: 6
    property int categorieswidth: 300
    property int selectedcategory: 0
    required property var appModel
    required property var categoryModel
    signal appLaunchRequested(int i)

    function find(model, criteriamodel) {
        if (model[0] === "All") {
            return 0
        }
        for (var j = 0; j < model.length; ++j) {
            return criteriamodel.indexOf(model[j])
        }
        return -1
    }

    Row {
        Flickable {
            id: catflickable
            width: categorieswidth
            height: parent.height
            contentWidth: column.width
            contentHeight: column.height + Kirigami.Units.largeSpacing * catrepeater.count / 2 + 1
            clip: true
            flickableDirection: Flickable.VerticalFlick
            Item {
                id: catwrapper
                width: Math.max(catflickable.width, column.width)
                height: Math.max(catflickable.height, column.height)
                Column {
                    id: column
                    spacing: Kirigami.Units.largeSpacing
                    anchors.centerIn: parent
                    Repeater {
                        id: catrepeater
                        objectName: "catrepeater"
                        model: categoryModel
                        ToggleItem {
                            property int i: selectedcategory
                            id: catbutton
                            direction: 1
                            width: categorieswidth - Kirigami.Units.largeSpacing * 2
                            height: catitemheight
                            textVisible: true
                            iconname: catrepeater.model.get(index).iconname
                            iconsize: Kirigami.Units.iconSizes.medium
                            textcontent: visibleName
                            disableable: false
                            onIChanged: {
                                if (index === selectedcategory) {
                                    selected = true
                                } else {
                                    selected = false
                                }
                            }
                            onItemEnabled: {
                                selectedcategory = index
                            }
                            Component.onCompleted: {
                                if (index === selectedcategory) {
                                    selected = true
                                } else {
                                    selected = false
                                }
                            }
                        }
                    }
                }
            }
        }
        Flickable {
            id: flickable
            width: root.width - categorieswidth
            height: root.height
            contentWidth: grid.width
            contentHeight: grid.height
            clip: true
            flickableDirection: Flickable.HorizontalFlick
            Item {
                id: wrapper
                width: Math.max(flickable.width, grid.width)
                height: Math.max(flickable.height, grid.height)
                Grid {
                    id: grid
                    rows: columncount
                    spacing: Kirigami.Units.largeSpacing
                    flow: Grid.TopToBottom
                    anchors.centerIn: parent
                    Repeater {
                        id: repeater
                        objectName: "repeater"
                        model: appModel
                        LauncherItem {
                            property bool v: true
                            property int i: selectedcategory
                            normalWidth: itemwidth
                            expandedWidth: itemwidth + 500
                            height: itemheight
                            visible: v & hidden !== true & (name !== "" | iconname !== "")
                            iconName: iconname
                            iconWidth: Kirigami.Units.iconSizes.huge
                            iconHeight: Kirigami.Units.iconSizes.huge
                            text: name
                            onIChanged: {
                                v = find(categoryModel.get(
                                             selectedcategory).includes,
                                         categories) !== -1
                            }
                            Component.onCompleted: {
                                v = true
                            }
                            onAppButtonPressed: {
                                print("a")
                                appLaunchRequested(index)
                            }
                        }
                    }
                }
            }
        }
    }
}
