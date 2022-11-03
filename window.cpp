#include "window.h"

window::window(int width, int height, int direction, int offset, bool show) {
  window::width = width;
  window::height = height;
  window::direction = direction;
  window::offset = offset;
  window::show = show;
}

int window::create() {
  const QUrl url(QStringLiteral("qrc:/window.qml"));
  window::applicationEngine.load(url);
  window::edgeWindow = dynamic_cast<QQuickWindow *>(
      window::applicationEngine.rootObjects().first());

  QScreen *screen = QGuiApplication::primaryScreen();

  window::edgeWindow->setProperty("offset", offset);
  window::edgeWindow->setProperty("direction", window::direction);

  if (window::width != 0) {
    window::edgeWindow->setWidth(window::width);
  } else {
    window::edgeWindow->setProperty("horizontalexpand", true);
  }
  if (window::height != 0) {
    window::edgeWindow->setHeight(window::height);
  } else {
    window::edgeWindow->setProperty("verticalexpand", true);
  }

  /*
  window::edgeWindow->setProperty("topoffset", screen->availableGeometry().y());
  window::edgeWindow->setProperty("leftoffset",
                                  screen->availableGeometry().x());*/
  window::edgeWindow->setFlags(Qt::CustomizeWindowHint |
                               Qt::WindowStaysOnTopHint);
  /*window::edgeWindow->setFlags(Qt::FramelessWindowHint |
                               Qt::WindowStaysOnTopHint | Qt::SplashScreen |
                               Qt::BypassWindowManagerHint);*/
  // KWindowSystem::setOnAllDesktops(window::edgeWindow->winId(), true);
  // KWindowSystem::setType(window::edgeWindow->winId(), NET::Dock);
  KWindowSystem::setState(window::edgeWindow->winId(), NET::SkipTaskbar |
                                                           NET::SkipPager |
                                                           NET::SkipSwitcher);
  window::edgeWindow->raise();
  window::edgeWindow->requestActivate();
  KWindowEffects::enableBlurBehind(window::edgeWindow);
  return 0;
}

void window::showSlot() {
  KWindowSystem::setState(window::edgeWindow->winId(), NET::SkipTaskbar |
                                                           NET::SkipPager |
                                                           NET::SkipSwitcher);
  window::edgeWindow->setProperty("visible", true);
  window::edgeWindow->setProperty("show", true);
}

void window::hideSlot() { window::edgeWindow->setProperty("show", false); }
