#include "config.h"

config::config() {}

int config::load(bool fallback, QString path) {
  QScreen *screen = QGuiApplication::primaryScreen();

  panelGeometry = screen->availableGeometry();
  QFile *file;
  if (fallback == true) {
    file = new QFile(":/fallback.json");
  } else {
    file = new QFile(path);
  }
  if (!file->open(QIODevice::ReadOnly)) {
    qDebug() << "Fallback JSON Read Failed!!";
  } else {
    qDebug() << "file opened";
    QJsonDocument configJson = QJsonDocument::fromJson(file->readAll());
    QJsonObject configObj = configJson.object();
    QJsonArray panelsArray = configObj.value("panels").toArray();
    for (int i = 0; i < panelsArray.count(); i++) {
      QJsonObject panelObj = panelsArray[i].toObject();
      if (panelObj.value("type") == "panel") {
        panels.emplace_back(new panel(panelObj.value("width").toInt(),
                                      panelObj.value("direction").toInt(),
                                      panelObj.value("name").toString(),
                                      panelObj.value("reserve").toBool()));
        panels.back()->create();
        if (panelObj.value("direction").toInt() == 1) {
          panelGeometry.setRect(
              panelGeometry.x() - panelObj.value("width").toInt(),
              panelGeometry.y(),
              panelGeometry.width() - panelObj.value("width").toInt(),
              panelGeometry.height());
        }
        if (panelObj.value("direction").toInt() == 2) {
          panelGeometry.setRect(panelGeometry.x(), panelGeometry.y(),
                                panelGeometry.width() -
                                    panelObj.value("width").toInt(),
                                panelGeometry.height());
        }
        if (panelObj.value("direction").toInt() == 3) {
          panelGeometry.setRect(
              panelGeometry.x(),
              panelGeometry.y() - panelObj.value("width").toInt(),
              panelGeometry.width(),
              panelGeometry.height() - panelObj.value("width").toInt());
        }
        if (panelObj.value("direction").toInt() == 4) {
          panelGeometry.setRect(
              panelGeometry.x(), panelGeometry.y(), panelGeometry.width(),
              panelGeometry.height() - panelObj.value("width").toInt());
        }
        panelROffsets.push_back(0);
        QJsonArray widgetsArray = panelObj.value("widgets").toArray();
        for (int j = 0; j < widgetsArray.count(); j++) {
          QJsonObject widgetObj = widgetsArray[j].toObject();

          if (widgetObj.value("type") == "clickItem") {
            if (widgetObj.value("special") == "clock") {
              items.emplace_back(new clockItem());
            } else {
              items.emplace_back(new clickItem());
            }
            items.back()->create("qrc:/ToggleItem.qml", panels.back().get());
            items.back()->clickItemInstance->setParentItem(
                items.back()->sampleItem);
            /*items.back()->clickItemInstance->setProperty("x",
                                                         panelROffsets.back());*/
            items.back()->clickItemInstance->setProperty(
                "animation", widgetObj.value("animation").toInt());
            items.back()->clickItemInstance->setProperty(
                "direction", widgetObj.value("direction").toInt());
            if (panelObj.value("direction").toInt() == 3 ||
                panelObj.value("direction").toInt() == 4) {
              items.back()->clickItemInstance->setProperty(
                  "width", widgetObj.value("width").toInt());
              items.back()->clickItemInstance->setProperty(
                  "height", panelObj.value("width").toInt());
            }
            if (widgetObj.value("fill").toBool() == true) {
              qDebug() << "fill";
              items.back()->clickItemInstance->setProperty("fill", true);
            }
            if (widgetObj.value("iconVisible").toBool() == false) {
              items.back()->clickItemInstance->setProperty("iconVisible",
                                                           false);
            } else {
              items.back()->clickItemInstance->setProperty("iconVisible", true);
            }
            if (widgetObj.value("textVisible").toBool() == true) {
              items.back()->clickItemInstance->setProperty("textVisible", true);
            } else {
              items.back()->clickItemInstance->setProperty("textVisible",
                                                           false);
            }
            items.back()->clickItemInstance->setProperty(
                "iconname", widgetObj.value("iconname").toString());
            panelROffsets.back() += widgetObj.value("width").toInt();
            QJsonObject actionObj = widgetObj.value("action").toObject();
            if (actionObj.value("type") == "window") {
              windows.emplace_back(
                  new window(actionObj.value("width").toInt(),
                             actionObj.value("height").toInt(),
                             actionObj.value("direction").toInt(),
                             actionObj.value("offset").toInt()));
              windows.back()->create();
              QObject::connect(
                  windows.back()->edgeWindow, SIGNAL(windowhidden(QString)),
                  items.back().get(), SLOT(clickItemDisable(QString)));
              QObject::connect(items.back()->clickItemInstance,
                               SIGNAL(itemEnabled(QString)),
                               windows.back().get(), SLOT(showSlot()));
              QObject::connect(items.back()->clickItemInstance,
                               SIGNAL(itemDisabled(QString)),
                               windows.back().get(), SLOT(hideSlot()));
              QJsonObject itemObj = actionObj.value("item").toObject();
              if (itemObj.value("type") == "windowSelector") {
                windowSelectors.emplace_back(new windowSelector());
                windowSelectors.back()->create("qrc:/WindowSelector.qml",
                                               windows.back().get());
                windowSelectors.back()->windowSelectorInstance->setParentItem(
                    windowSelectors.back()->sampleItem);
                windowSelectors.back()->windowSelectorInstance->setWidth(
                    windows.back()->edgeWindow->width());
                windowSelectors.back()->windowSelectorInstance->setHeight(
                    windows.back()->edgeWindow->height());
              }
              if (itemObj.value("type") == "launcher") {
                launchers.emplace_back(new launcher());
                if (itemObj.value("mode") == "mobile") {
                  launchers.back()->create("qrc:/LauncherMobile.qml",
                                           windows.back().get());
                }
                if (itemObj.value("mode") == "desktop") {
                  launchers.back()->create("qrc:/LauncherDesktop.qml",
                                           windows.back().get());
                }
                launchers.back()->launcherInstance->setParentItem(
                    launchers.back()->sampleItem);
                qDebug() << "atari";
                launchers.back()->launcherInstance->setWidth(
                    windows.back()->edgeWindow->width());
                launchers.back()->launcherInstance->setHeight(
                    windows.back()->edgeWindow->height());
              }
            }
          }

          if (widgetObj.value("type") == "windowSelectorItem") {
            windowSelectorItems.emplace_back(new windowSelectorItem());
            windowSelectorItems.back()->create("qrc:/WindowSelectorItem.qml",
                                               panels.back().get());
            windowSelectorItems.back()
                ->windowSelectorItemInstance->setParentItem(
                    windowSelectorItems.back()->sampleItem);
            windowSelectorItems.back()->windowSelectorItemInstance->setX(
                panelROffsets.back());
            if (widgetObj.value("fill").toBool() == true) {
              qDebug() << "fill";
              windowSelectorItems.back()
                  ->windowSelectorItemInstance->setProperty("fill", true);
            }
            if (widgetObj.value("group").toBool() == true) {
              windowSelectorItems.back()
                  ->windowSelectorItemInstance->setProperty("group", true);
            } else {
              windowSelectorItems.back()
                  ->windowSelectorItemInstance->setProperty("group", false);
            }
            // TODO:縦対応
            if (panelObj.value("direction").toInt() == 3 ||
                panelObj.value("direction").toInt() == 4) {
              windowSelectorItems.back()
                  ->windowSelectorItemInstance->setProperty(
                      "width", widgetObj.value("width").toInt());
              windowSelectorItems.back()
                  ->windowSelectorItemInstance->setProperty(
                      "height", panelObj.value("width").toInt());
            }
            panelROffsets.back() += widgetObj.value("width").toInt();
          }
        }
      }
    }
  }
  for (int i = 0; i < windows.size(); i++) {
    if (windows[i]->edgeWindow->property("direction").toInt() == 3) {
      windows[i]->edgeWindow->setX(
          screen->availableGeometry().left() +
          windows[i]->edgeWindow->property("offset").toInt());
      windows[i]->edgeWindow->setY(panelGeometry.y());
    }
    if (windows[i]->edgeWindow->property("direction").toInt() == 4) {
      windows[i]->edgeWindow->setX(
          screen->availableGeometry().left() +
          windows[i]->edgeWindow->property("offset").toInt());
      windows[i]->edgeWindow->setY(
          panelGeometry.y() + panelGeometry.height() -
          windows[i]->edgeWindow->property("height").toInt());
    }
  }
  return 0;
}
