#ifndef WINDOWMODEL_H
#define WINDOWMODEL_H

#include <QAbstractListModel>
#include <QFileInfo>
#include <QStandardPaths>
#include <QtX11Extras/QX11Info>

#include <KF5/KWindowSystem/KWindowEffects>
#include <KF5/KWindowSystem/KWindowInfo>
#include <KF5/KWindowSystem/KWindowShadow>
#include <KF5/KWindowSystem/KWindowSystem>
#include <KF5/Kirigami2/Kirigami/PlatformTheme>

#include <KF5/KWindowSystem/netwm.h>

#include <xcb/xcb.h>

typedef struct windowentry {
  // window class
  QString id;
  // icon name
  QStringList iconName;
  // visible name
  QStringList visibleName;
  QString desktopPath;
  QString desktopIcon;
  QString exec;

  QList<quint64> wids;

  int currentActive = 0;
  bool isActive = false;
  bool isPinned = false;
  bool fixed = false;
} windowentry;
Q_DECLARE_METATYPE(windowentry)

class windowModel : public QAbstractListModel {
  Q_OBJECT
public:
  enum Roles {
    AppIdRole = Qt::UserRole + 1,
    IconNameRole,
    VisibleNameRole,
    ActiveRole,
    CurrentActiveRole,
    WindowCountRole,
    IsPinnedRole,
    DesktopFileRole,
    DesktopIconRole,
    FixedItemRole
  };

  explicit windowModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QHash<int, QByteArray> roleNames() const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

signals:
  void countChanged();

  void itemAdded();
  void itemRemoved();

public slots:
  void onWindowActivateRequested(int index, int number);
  void onWindowCloseRequested(int index, int number);
  void onWindowMinimizeRequested(int index, int number);

private:
  void onWindowAdded(WId wid);
  void onWindowChanged(WId wid, NET::Properties properties,
                       NET::Properties2 properties2);
  void onWindowRemoved(WId wid);
  void onActiveChanged(WId wid);

  KWindowSystem *kwindowsystem;
  QList<windowentry> entries;
};

#endif // WINDOWMODEL_H
