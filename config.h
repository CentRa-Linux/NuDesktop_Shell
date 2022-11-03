#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickWindow>

#include <QLocale>
#include <QScreen>
#include <QString>
#include <QTranslator>
#include <QtX11Extras/QX11Info>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <KF5/KWindowSystem/KWindowEffects>
#include <KF5/KWindowSystem/KWindowInfo>
#include <KF5/KWindowSystem/KWindowShadow>
#include <KF5/KWindowSystem/KWindowSystem>
#include <KF5/Kirigami2/Kirigami/PlatformTheme>

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include "clickItem.h"
#include "clockItem.h"
#include "launcher.h"
#include "panel.h"
#include "window.h"
#include "windowModel.h"
#include "windowSelector.h"
#include "windowSelectorItem.h"

#ifndef CONFIG_H
#define CONFIG_H

class config {
public:
  config();
  int load(bool fallback = true, QString path = "");

  std::vector<std::unique_ptr<panel>> panels;
  std::vector<int> panelROffsets;
  std::vector<std::unique_ptr<clickItem>> items;
  std::vector<std::unique_ptr<windowSelectorItem>> windowSelectorItems;
  std::vector<std::unique_ptr<window>> windows;
  std::vector<std::unique_ptr<launcher>> launchers;
  std::vector<std::unique_ptr<windowSelector>> windowSelectors;
  QRect panelGeometry = QRect(0, 0, 0, 0);
};

#endif // CONFIG_H
