#include <QPixmap>
#include <QQuickImageProvider>

#include <KF5/KWindowSystem/KWindowSystem>

#ifndef WINDOWICONPROVIDER_H
#define WINDOWICONPROVIDER_H

#endif // WINDOWICONPROVIDER_H

class windowIconProvider : public QQuickImageProvider {
public:
  windowIconProvider();

  QImage requestImage(const QString &id, QSize *size,
                      const QSize &requestedSize);
  QPixmap requestPixmap(const QString &id, QSize *size,
                        const QSize &requestedSize);

private:
  QMap<QString, QPixmap> pixmaps;
  void reloadimage();
};
