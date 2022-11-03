#include "launcher.h"

launcher::launcher() : applist(new appList) {}

int launcher::create(QString path, window *w) {
  const QUrl url(path);

  launcher::root = w->applicationEngine.rootObjects().first();
  launcher::sampleItem = root->findChild<QQuickItem *>("sampleItem");
  launcher::component = new QQmlComponent(&w->applicationEngine, QUrl(path));
  applist->load();
  launcher::launcherInstance =
      qobject_cast<QQuickItem *>(component->createWithInitialProperties(
          {{"appModel", QVariant::fromValue(&applist->model)},
           {"categoryModel", QVariant::fromValue(&applist->categorymodel)}}));
  if (component->status() != component->Ready) {
    if (component->status() == component->Error) {
      qDebug() << "Error:" + component->errorString();
    }
  }
  w->applicationEngine.setObjectOwnership(launcherInstance,
                                          QQmlEngine::JavaScriptOwnership);
  launcher::launcherWindow = w;
  QObject::connect(launcherInstance, SIGNAL(appLaunchRequested(int)),
                   &applist->model, SLOT(onAppLaunchRequested(int)));
  QObject::connect(launcherInstance, SIGNAL(appLaunchRequested(int)), this,
                   SLOT(hideWindow()));

  return 0;
}

void launcher::hideWindow() {
  launcher::launcherWindow->setProperty("show", false);
}

void launcher::reloadModel() {
  // repeater->setProperty("model", QVariant::fromValue(applist->model));
}
