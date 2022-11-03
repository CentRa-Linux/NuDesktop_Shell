#include "panel.h"

panel::panel(int width, int direction, QString name, bool reserve, bool show) {
  panel::width = width;
  panel::direction = direction;
  panel::name = name;
  panel::reserve = reserve;
  panel::show = show;
}

int panel::create() {
  // QScreen *screen = QGuiApplication::primaryScreen();
  const QUrl url(QStringLiteral("qrc:/panel.qml"));
  panel::applicationEngine.load(url);
  panel::panelWindow = dynamic_cast<QQuickWindow *>(
      panel::applicationEngine.rootObjects().first());

  panel::panelWindow->setProperty("settingwidth", panel::width);
  panel::panelWindow->setProperty("settingopacity", 0.8);
  panel::panelWindow->setProperty("direction", panel::direction);
  panel::panelWindow->setFlags(Qt::CustomizeWindowHint |
                               Qt::WindowDoesNotAcceptFocus |
                               Qt::WindowStaysOnTopHint);
  panel::panelWindow->raise();

  if (reserve == true) {
    if (direction == 1) {
      KWindowSystem::setStrut(panel::panelWindow->winId(),
                              panel::panelWindow->width(), 0, 0, 0);
    }
    if (direction == 2) {
      KWindowSystem::setStrut(panel::panelWindow->winId(), 0,
                              panel::panelWindow->width(), 0, 0);
    }
    if (direction == 3) {
      KWindowSystem::setStrut(panel::panelWindow->winId(), 0, 0,
                              panel::panelWindow->height(), 0);
    }
    if (direction == 4) {
      KWindowSystem::setStrut(panel::panelWindow->winId(), 0, 0, 0,
                              panel::panelWindow->height());
    }
  }
  KWindowSystem::setOnAllDesktops(panel::panelWindow->winId(), true);
  KWindowSystem::setType(panel::panelWindow->winId(), NET::Dock);
  KWindowSystem::setState(panel::panelWindow->winId(), NET::SkipTaskbar |
                                                           NET::SkipPager |
                                                           NET::SkipSwitcher);
  KWindowEffects::enableBlurBehind(panel::panelWindow);

  return 0;
}
