#include <QCommandLineParser>
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
#include "config.h"
#include "panel.h"
#include "window.h"
#include "windowSelector.h"

int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QGuiApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("NuDesktop Commandline Interface");
  parser.addHelpOption();
  parser.addPositionalArgument("path", "Path of config file.");
  parser.process(QCoreApplication::arguments());
  for (QString arg : parser.positionalArguments()) {
    qDebug() << arg;
  }

  config c;
  c.load(false, parser.positionalArguments()[0]);

  /*

  window w(700, 500, 4, 0);
  w.create();

  clickItem i;
  i.create("qrc:/ToggleItem.qml", &p);
  i.clickItemInstance->setParentItem(i.sampleItem);
  i.clickItemInstance->setProperty("animation", 1);
  i.clickItemInstance->setProperty("direction", 4);
  i.clickItemInstance->setProperty("width", 120);
  i.clickItemInstance->setProperty("iconname", "go-up-symbolic");
  QObject::connect(w.edgeWindow, SIGNAL(windowhidden(QString)), &i,
                   SLOT(clickItemDisable(QString)));
  QObject::connect(i.clickItemInstance, SIGNAL(itemEnabled(QString)), &w,
                   SLOT(showSlot()));
  QObject::connect(i.clickItemInstance, SIGNAL(itemDisabled(QString)), &w,
                   SLOT(hideSlot()));*/

  return app.exec();
}
