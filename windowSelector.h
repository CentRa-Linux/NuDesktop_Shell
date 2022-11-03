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

#include "window.h"
#include "windowModel.h"

#ifndef WINDOWSELECTOR_H
#define WINDOWSELECTOR_H

class windowSelector : public QObject {
  Q_OBJECT
public:
  windowSelector();
  int create(QString path, window *w);

  QObject *root;
  QQuickItem *sampleItem;
  QQuickItem *windowSelectorInstance;
  QQmlComponent *component;
  QObject *repeater;

  windowModel *windowmodel;
};

#endif // WINDOWSELECTOR_H
