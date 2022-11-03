#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickWindow>

#include <QLocale>
#include <QObject>
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
#include "windowModel.h"

#ifndef WINDOWSELECTORITEM_H
#define WINDOWSELECTORITEM_H

class windowSelectorItem : public QObject {
  Q_OBJECT
public:
  windowSelectorItem();
  int create(QString path, panel *p);

  QObject *root;
  QQuickItem *sampleItem;
  QQuickItem *windowSelectorItemInstance;
  QQmlComponent *component;
  QObject *repeater;

  windowModel *windowmodel;
};

#endif // WINDOWSELECTORITEM_H
