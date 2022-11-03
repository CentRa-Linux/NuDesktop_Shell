#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickWindow>

#include <QLocale>
#include <QScreen>
#include <QString>
#include <QTranslator>
#include <QtX11Extras/QX11Info>

#include <KF5/KWindowSystem/KWindowEffects>
#include <KF5/KWindowSystem/KWindowInfo>
#include <KF5/KWindowSystem/KWindowShadow>
#include <KF5/KWindowSystem/KWindowSystem>
#include <KF5/Kirigami2/Kirigami/PlatformTheme>

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include "panel.h"

#ifndef ITEM_H
#define ITEM_H

class clickItem : public QObject {
  Q_OBJECT
public:
  clickItem();
  int create(QString path, panel *p);

  QObject *root;
  QQuickItem *sampleItem;
  QQuickItem *clickItemInstance;
  QQmlComponent *component;
  bool created = false;

public slots:
  void clickItemDisable(QString mes);
  void clickItemEnabled(QString mes);
  void clickItemDisabled(QString mes);
};

#endif // ITEM_H
