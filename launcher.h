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

#include "appList.h"
#include "window.h"

#ifndef LAUNCHER_H
#define LAUNCHER_H

class launcher : public QObject {
  Q_OBJECT
public:
  launcher();
  int create(QString path, window *w);

  QObject *root;
  QQuickItem *sampleItem;
  QQuickItem *launcherInstance;
  QQmlComponent *component;
  QObject *repeater;
  QObject *catrepeater;
  window *launcherWindow;

  appList *applist;

public slots:
  void reloadModel();
  void hideWindow();
};

#endif // LAUNCHER_H
