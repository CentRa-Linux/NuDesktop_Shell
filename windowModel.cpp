#include "windowModel.h"

windowModel::windowModel(QObject *parent)
    : QAbstractListModel{parent}, kwindowsystem(KWindowSystem::self()) {
  for (int i = 0; i < kwindowsystem->windows().count(); i++) {
    onWindowAdded(kwindowsystem->windows()[i]);
  }
  connect(kwindowsystem,
          QOverload<WId, NET::Properties, NET::Properties2>::of(
              &KWindowSystem::windowChanged),
          this, &windowModel::onWindowChanged);
  connect(kwindowsystem, &KWindowSystem::windowAdded, this,
          &windowModel::onWindowAdded);
  connect(kwindowsystem, &KWindowSystem::windowRemoved, this,
          &windowModel::onWindowRemoved);
  connect(kwindowsystem, &KWindowSystem::activeWindowChanged, this,
          &windowModel::onActiveChanged);
}

int windowModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)

  return entries.count();
}

QVariant windowModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  windowentry item = entries.at(index.row());

  switch (role) {
  case AppIdRole:
    return item.id;
  case IconNameRole:
    return item.iconName;
  case VisibleNameRole:
    return item.visibleName;
  case ActiveRole:
    return item.isActive;
  case CurrentActiveRole:
    return item.currentActive;
  case WindowCountRole:
    return item.wids.count();
  case IsPinnedRole:
    return item.isPinned;
  case DesktopIconRole:
    return item.desktopIcon;
  case DesktopFileRole:
    return item.desktopPath;
  case FixedItemRole:
    return item.fixed;
  default:
    return QVariant();
  }

  return QVariant();
}

QHash<int, QByteArray> windowModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[AppIdRole] = "appId";
  roles[IconNameRole] = "iconName";
  roles[VisibleNameRole] = "visibleName";
  roles[ActiveRole] = "isActive";
  roles[CurrentActiveRole] = "currentActive";
  roles[WindowCountRole] = "windowCount";
  roles[IsPinnedRole] = "isPinned";
  roles[DesktopFileRole] = "desktopFile";
  roles[DesktopIconRole] = "desktopIcon";
  roles[FixedItemRole] = "fixed";
  return roles;
}

QString parse(QString path) {
  QString output;
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << "cannot open file. " << path;
  }

  QTextStream in(&file);

  while (!in.atEnd()) {
    QString temp = in.readLine();
    if (temp.split("=")[0] == "Icon") {
      output = temp.split("=")[1];
    }
  }

  return output;
}

void windowModel::onWindowAdded(WId wid) {
  if (wid == 0) {
    return;
  }
  KWindowInfo info(wid,
                   NET::WMFrameExtents | NET::WMWindowType | NET::WMIconName |
                       NET::WMGeometry | NET::WMDesktop | NET::WMState |
                       NET::WMName | NET::WMVisibleName | NET::WMWindowType,
                   NET::WM2WindowClass | NET::WM2DesktopFileName |
                       NET::WM2Activities | NET::WM2AllowedActions |
                       NET::WM2TransientFor);

  QFlags<NET::WindowTypeMask> ignoreList;
  ignoreList |= NET::DesktopMask;
  ignoreList |= NET::DockMask;
  ignoreList |= NET::SplashMask;
  ignoreList |= NET::ToolbarMask;
  ignoreList |= NET::MenuMask;
  ignoreList |= NET::PopupMenuMask;
  ignoreList |= NET::NotificationMask;
  ignoreList |= NET::UtilityMask;
  if (info.windowClassClass() == "NuDesktop_Shell" ||
      NET::typeMatchesMask(info.windowType(NET::AllTypesMask), ignoreList) ||
      info.hasState(NET::SkipTaskbar) || info.hasState(NET::SkipPager)) {
    return;
  }

  qDebug() << info.visibleName() << ":" << info.windowType(NET::AllTypesMask);
  for (int i = 0; i < entries.count(); i++) {
    if (entries[i].id == info.windowClassClass()) {
      entries[i].wids.push_back(wid);
      entries[i].iconName.push_back(info.iconName());
      entries[i].visibleName.push_back(info.visibleName());
      return;
    }
  }
  emit beginInsertRows(QModelIndex(), entries.count(), entries.count());
  windowentry result;
  result.id = info.windowClassClass();
  result.wids.push_back(wid);
  result.iconName.push_back(info.iconName());
  result.visibleName.push_back(info.visibleName());
  QFileInfo desktopfile(info.desktopFileName());
  if (desktopfile.exists()) {
    result.desktopPath = info.desktopFileName() + ".desktop";
  } else {
    if (info.desktopFileName() != "") {
      result.desktopPath =
          "/usr/share/applications/" + info.desktopFileName() + ".desktop";
    }
  }
  result.desktopIcon = parse(result.desktopPath);
  if (kwindowsystem->activeWindow() == wid) {
    onActiveChanged(wid);
  }
  entries.push_back(result);
  QModelIndex topLeft = createIndex(0, 0);
  QModelIndex bottomRight = createIndex(entries.count(), 0);
  emit endInsertRows();
  emit dataChanged(topLeft, bottomRight);
  emit itemAdded();
  emit countChanged();
  return;
};

void windowModel::onWindowChanged(WId wid, NET::Properties properties,
                                  NET::Properties2 properties2) {
  KWindowInfo info(wid,
                   NET::WMFrameExtents | NET::WMWindowType | NET::WMIconName |
                       NET::WMGeometry | NET::WMDesktop | NET::WMState |
                       NET::WMName | NET::WMVisibleName | NET::WMWindowType,
                   NET::WM2WindowClass | NET::WM2DesktopFileName |
                       NET::WM2Activities | NET::WM2AllowedActions |
                       NET::WM2TransientFor);
  for (int i = 0; i < entries.count(); i++) {
    for (int j = 0; j < entries[i].wids.count(); j++) {
      if (wid == entries[i].wids[j]) {
        if (properties == NET::WMName) {
          entries[i].iconName[j] = info.iconName();
          entries[i].visibleName[j] = info.visibleName();
        }
      }
    }
  }
  QModelIndex topLeft = createIndex(0, 0);
  QModelIndex bottomRight = createIndex(entries.count(), 0);
  emit dataChanged(topLeft, bottomRight);
}

void windowModel::onWindowRemoved(WId wid) {
  for (int i = 0; i < entries.count(); i++) {
    for (int j = 0; j < entries[i].wids.count(); j++) {
      if (wid == entries[i].wids[j]) {
        entries[i].iconName.removeAt(j);
        entries[i].visibleName.removeAt(j);
        entries[i].wids.removeAt(j);
      }
    }
    if (entries[i].wids.count() == 0) {
      beginRemoveRows(QModelIndex(), i, i);
      entries.removeAt(i);
      endRemoveRows();
    }
  }
  QModelIndex topLeft = createIndex(0, 0);
  QModelIndex bottomRight = createIndex(entries.count(), 0);
  emit dataChanged(topLeft, bottomRight);
  emit itemRemoved();
  emit countChanged();
  return;
};

void windowModel::onActiveChanged(WId wid) {
  int index = -1;
  for (int i = 0; i < entries.count(); i++) {
    for (int j = 0; j < entries[i].wids.count(); j++) {
      if (wid == entries[i].wids[j]) {
        index = i;
        entries[i].currentActive = j;
      }
    }
  }
  if (index != -1) {
    for (int i = 0; i < entries.count(); i++) {
      entries[i].isActive = false;
    }
    entries[index].isActive = true;
  }
  QModelIndex topLeft = createIndex(0, 0);
  QModelIndex bottomRight = createIndex(entries.count(), 0);
  emit dataChanged(topLeft, bottomRight);
};

void windowModel::onWindowActivateRequested(int index, int number) {
  kwindowsystem->activateWindow(entries[index].wids[number]);
}

void windowModel::onWindowCloseRequested(int index, int number) {
  NETRootInfo(QX11Info::connection(), NET::CloseWindow)
      .closeWindowRequest(entries[index].wids[number]);
}

void windowModel::onWindowMinimizeRequested(int index, int number) {
  kwindowsystem->minimizeWindow(entries[index].wids[number]);
}
