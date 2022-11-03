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

#ifndef PANEL_H
#define PANEL_H

class panel : public QObject {
  Q_OBJECT
public:
  panel(int width, int direction, QString name, bool reserve = true,
        bool show = true);
  panel() {}
  panel(const panel &obj) {}

  int create();

  QQmlApplicationEngine applicationEngine;
  QQmlEngine engine;
  QQuickWindow *panelWindow;

  bool reserve;
  bool show;
  int width;
  int direction; // 1->left 2->right 3->up 4->down
  QString name;
};

#endif // PANEL_H
