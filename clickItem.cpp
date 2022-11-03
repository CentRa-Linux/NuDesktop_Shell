#include "clickItem.h"

clickItem::clickItem() {}

int clickItem::create(QString path, panel *p) {
  const QUrl url(path);

  clickItem::root = p->applicationEngine.rootObjects().first();
  clickItem::sampleItem = root->findChild<QQuickItem *>("sampleItem");
  clickItem::component = new QQmlComponent(&p->applicationEngine, QUrl(path));
  clickItem::clickItemInstance =
      qobject_cast<QQuickItem *>(component->create());
  p->applicationEngine.setObjectOwnership(clickItemInstance,
                                          QQmlEngine::JavaScriptOwnership);
  QObject::connect(clickItemInstance, SIGNAL(itemEnabled(QString)), this,
                   SLOT(clickItemEnabled(QString)));
  QObject::connect(clickItemInstance, SIGNAL(itemDisabled(QString)), this,
                   SLOT(clickItemDisabled(QString)));
  created = true;

  return 0;
}

void clickItem::clickItemDisable(QString mes) {
  clickItem::clickItemInstance->setProperty("selected", false);
}
void clickItem::clickItemEnabled(QString mes) {}
void clickItem::clickItemDisabled(QString mes) {}
