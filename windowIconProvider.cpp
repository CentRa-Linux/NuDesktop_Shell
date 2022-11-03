#include "windowIconProvider.h"

windowIconProvider::windowIconProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

QImage windowIconProvider::requestImage(const QString &id, QSize *size,
                                        const QSize &requestedSize) {
  reloadimage();
  QImage image;
  image = pixmaps[id].scaled(requestedSize).toImage();
  return image;
};

QPixmap windowIconProvider::requestPixmap(const QString &id, QSize *size,
                                          const QSize &requestedSize) {
  reloadimage();
  QPixmap pixmap;
  pixmap = pixmaps[id].scaled(requestedSize);
  return pixmap;
};

void windowIconProvider::reloadimage() {
  for (int i = 0; i < KWindowSystem::windows().count(); i++) {
    WId wid = KWindowSystem::windows()[i];
    KWindowInfo info(wid, NET::WMVisibleIconName, NET::WM2WindowClass);
    pixmaps[info.windowClassClass()] = KWindowSystem::icon(wid, 512, 512, true);
  }
}
