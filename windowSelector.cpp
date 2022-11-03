#include "windowSelector.h"
#include "windowIconProvider.h"

windowSelector::windowSelector() : windowmodel(new windowModel) {}

int windowSelector::create(QString path, window *w) {
  const QUrl url(path);

  windowSelector::root = w->applicationEngine.rootObjects().first();
  windowSelector::sampleItem = root->findChild<QQuickItem *>("sampleItem");
  windowSelector::component =
      new QQmlComponent(&w->applicationEngine, QUrl(path));
  windowSelector::windowSelectorInstance =
      qobject_cast<QQuickItem *>(component->create());
  if (component->status() != component->Ready) {
    if (component->status() == component->Error) {
      qDebug() << "Error:" + component->errorString();
    }
  }
  w->applicationEngine.setObjectOwnership(windowSelectorInstance,
                                          QQmlEngine::JavaScriptOwnership);
  w->applicationEngine.addImageProvider("windowicon", new windowIconProvider);
  repeater = dynamic_cast<QObject *>(
      windowSelector::windowSelectorInstance->findChild<QObject *>("repeater"));
  repeater->setProperty("model", QVariant::fromValue(windowmodel));

  QObject::connect(windowSelectorInstance,
                   SIGNAL(windowActivateRequested(int, int)), windowmodel,
                   SLOT(onWindowActivateRequested(int, int)));
  QObject::connect(windowSelectorInstance,
                   SIGNAL(windowCloseRequested(int, int)), windowmodel,
                   SLOT(onWindowCloseRequested(int, int)));
  return 0;
}
