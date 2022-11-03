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

#ifndef WINDOW_H
#define WINDOW_H

class window : public QObject {
  Q_OBJECT
public:
  window(int width, int height, int direction, int offset, bool show = false);
  int create();

  QQmlApplicationEngine applicationEngine;
  QQmlEngine engine;
  QQuickWindow *edgeWindow;

  bool show;
  int width;
  int height;
  int direction;
  int offset;

public slots:
  void showSlot();
  void hideSlot();
};

#endif // WINDOW_H
