#ifndef APPLIST_H
#define APPLIST_H

#undef Unsorted

#include <QAbstractItemModel>
#include <QDir>
#include <QGraphicsEffect>
#include <QGuiApplication>
#include <QMargins>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QX11Info>
#include <QtPlatformHeaders/QXcbWindowFunctions>

#include <KF5/KWindowSystem/KWindowEffects>
#include <KF5/KWindowSystem/KWindowInfo>
#include <KF5/KWindowSystem/KWindowShadow>
#include <KF5/KWindowSystem/KWindowSystem>
#include <KF5/Kirigami2/Kirigami/PlatformTheme>

#include <KF5/KWindowSystem/netwm.h>

#include <X11/Xatom.h>
#include <X11/Xlib.h>

typedef struct {
  QString name;
  QString exec;
  QString icon;
} actionentry;
Q_DECLARE_METATYPE(actionentry)

typedef struct desktopentry {
  QString name;
  QString genericname;
  QString type;
  QString icon;
  QString exec;
  QString path;
  QString comment;
  bool hidden = false;
  QStringList onlyshowin;
  QStringList notshowin;
  QStringList categories;
  QStringList keywords;
  QVector<actionentry> actions;
  QVariantList vactions;

  bool operator<(const desktopentry &other) const {
    return name < other.name; // sort by namefile
  }
} desktopentry;
Q_DECLARE_METATYPE(desktopentry)

class appModel : public QAbstractListModel {
  Q_OBJECT
public:
  enum Roles {
    ActionsRole,
    NameRole,
    GenericnameRole,
    TypeRole,
    IconRole,
    ExecRole,
    PathRole,
    CategoriesRole,
    CommentRole,
    HiddenRole
  };

  appModel(const QVector<desktopentry> &inputlist, QObject *parent = 0);
  appModel(QObject *parent = 0);
  QVector<desktopentry> list;
  QVariant data(const QModelIndex &index, int role) const;
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  void setmodel(const QVector<desktopentry> &inputlist);

public slots:
  void onAppLaunchRequested(int);

protected:
  QHash<int, QByteArray> roleNames() const;
};

typedef struct categoryentry {
  QString name;
  QString visibleName;
  QString icon;
  QStringList includes;
  QStringList notIncludes;
} categoryentry;
Q_DECLARE_METATYPE(categoryentry)

class categoryModel : public QAbstractListModel {
  Q_OBJECT
public:
  enum Roles {
    NameRole,
    VisibleNameRole,
    IconRole,
    IncludesRole,
    NotIncludesRole
  };

  categoryModel(const QVector<categoryentry> &inputlist, QObject *parent = 0);
  categoryModel(QObject *parent = 0);
  QVector<categoryentry> list;
  QVariant data(const QModelIndex &index, int role) const;
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  void setmodel(const QVector<categoryentry> &inputlist);

  Q_INVOKABLE QVariantMap get(int row);

protected:
  QHash<int, QByteArray> roleNames() const;
};

class appList {
public:
  appList();

  appModel model;
  categoryModel categorymodel;
  QVector<desktopentry> allapps;
  QVector<categoryentry> allcategories;
  desktopentry parse(QString path);
  categoryentry parseDirectory(QString path);
  void load();
};

#endif // APPLIST_H
