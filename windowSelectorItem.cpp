#include "windowSelectorItem.h"
#include "windowIconProvider.h"

windowSelectorItem::windowSelectorItem() : windowmodel(new windowModel) {}

int windowSelectorItem::create(QString path, panel *p) {
  const QUrl url(path);

  windowSelectorItem::root = p->applicationEngine.rootObjects().first();
  windowSelectorItem::sampleItem = root->findChild<QQuickItem *>("sampleItem");
  windowSelectorItem::component =
      new QQmlComponent(&p->applicationEngine, QUrl(path));
  windowSelectorItem::windowSelectorItemInstance =
      qobject_cast<QQuickItem *>(component->createWithInitialProperties(
          {{"windowModel", QVariant::fromValue(windowmodel)}}));
  if (component->status() != component->Ready) {
    if (component->status() == component->Error) {
      qDebug() << "Error:" + component->errorString();
    }
  }
  p->applicationEngine.setObjectOwnership(windowSelectorItemInstance,
                                          QQmlEngine::JavaScriptOwnership);
  p->applicationEngine.addImageProvider("windowicon", new windowIconProvider);
  repeater = dynamic_cast<QObject *>(
      windowSelectorItem::windowSelectorItemInstance->findChild<QObject *>(
          "repeater"));
  QObject::connect(windowSelectorItemInstance,
                   SIGNAL(windowActivateRequested(int, int)), windowmodel,
                   SLOT(onWindowActivateRequested(int, int)));
  QObject::connect(windowSelectorItemInstance,
                   SIGNAL(windowMinimizeRequested(int, int)), windowmodel,
                   SLOT(onWindowMinimizeRequested(int, int)));
  return 0;
}
