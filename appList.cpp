#include "appList.h"

QLocale locale;

appModel::appModel(const QVector<desktopentry> &inputlist, QObject *parent)
    : QAbstractListModel(parent) {
  list = inputlist;
}

appModel::appModel(QObject *parent) : QAbstractListModel(parent) {}

int appModel::rowCount(const QModelIndex &parent) const { return list.count(); }

int appModel::columnCount(const QModelIndex &parent) const { return 1; }

QHash<int, QByteArray> appModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[ActionsRole] = "actions";
  roles[NameRole] = "name";
  roles[GenericnameRole] = "genericname";
  roles[TypeRole] = "type";
  roles[IconRole] = "iconname";
  roles[ExecRole] = "exec";
  roles[PathRole] = "path";
  roles[CategoriesRole] = "categories";
  roles[CommentRole] = "comment";
  roles[HiddenRole] = "hidden";
  return roles;
}

QVariant appModel::data(const QModelIndex &index, int role) const {
  QVariant ret = QVariant();
  ret.setValue(list[index.row()]);
  if (role == ActionsRole) {
    return list[index.row()].vactions;
  }
  if (role == NameRole) {
    return list[index.row()].name;
  }
  if (role == IconRole) {
    return list[index.row()].icon;
  }
  if (role == ExecRole) {
    return list[index.row()].exec;
  }
  if (role == CategoriesRole) {
    return list[index.row()].categories;
  }
  if (role == HiddenRole) {
    return list[index.row()].hidden;
  }
  return ret;
}

void appModel::setmodel(const QVector<desktopentry> &inputlist) {
  list = inputlist;
}

void appModel::onAppLaunchRequested(int i) {
  QString exec = list[i].exec;
  QStringList args = exec.split(" ");
  QString name = args[0];
  qDebug() << args.count();
  if (args.count() > 1) {
    QString last = args.back();
    if (last.contains("%")) {
      args.pop_back();
    }
  }
  args.pop_front();
  QProcess process;
  process.startDetached(name, args);
}

categoryModel::categoryModel(const QVector<categoryentry> &inputlist,
                             QObject *parent)
    : QAbstractListModel(parent) {
  list = inputlist;
}

categoryModel::categoryModel(QObject *parent) : QAbstractListModel(parent) {}

int categoryModel::rowCount(const QModelIndex &parent) const {
  return list.count();
}

int categoryModel::columnCount(const QModelIndex &parent) const { return 1; }

QHash<int, QByteArray> categoryModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[NameRole] = "name";
  roles[VisibleNameRole] = "visibleName";
  roles[IconRole] = "iconname";
  roles[IncludesRole] = "includes";
  roles[NotIncludesRole] = "notIncludes";
  return roles;
}

QVariant categoryModel::data(const QModelIndex &index, int role) const {
  QVariant ret = QVariant();
  ret.setValue(list[index.row()]);
  if (role == NameRole) {
    return list[index.row()].name;
  }
  if (role == VisibleNameRole) {
    return list[index.row()].visibleName;
  }
  if (role == IconRole) {
    return list[index.row()].icon;
  }
  if (role == IncludesRole) {
    return list[index.row()].includes;
  }
  if (role == NotIncludesRole) {
    return list[index.row()].notIncludes;
  }
  return ret;
}

void categoryModel::setmodel(const QVector<categoryentry> &inputlist) {
  list = inputlist;
}

QVariantMap categoryModel::get(int row) {
  QHash<int, QByteArray> names = roleNames();
  QHashIterator<int, QByteArray> i(names);
  QVariantMap res;
  while (i.hasNext()) {
    i.next();
    QModelIndex idx = index(row, 0);
    QVariant data = idx.data(i.key());
    res[i.value()] = data;
  }
  return res;
}

appList::appList() {}

void appList::load() {
  QDir directory("/usr/share/applications");
  QDir localdirectory("/usr/local/share/applications");
  QDir homedirectory(QDir::homePath() + "/.local/share/applications");
  QDir desktopdirectories("/usr/share/desktop-directories");
  QDir userdesktopdirectories(QDir::homePath() +
                              "/.local/share/desktop-directories");
  QStringList files =
      directory.entryList(QStringList() << "*.desktop", QDir::Files);
  files = files +
          localdirectory.entryList(QStringList() << "*.desktop", QDir::Files);
  files = files +
          homedirectory.entryList(QStringList() << "*.desktop", QDir::Files);
  QStringList directoryfiles =
      desktopdirectories.entryList(QStringList() << "*.directory");
  directoryfiles =
      directoryfiles + userdesktopdirectories.entryList(
                           QStringList() << "*.directory", QDir::Files);
  for (int i = 0;
       i <
       directory.entryList(QStringList() << "*.desktop", QDir::Files).count();
       i++) {
    allapps.push_back(parse("/usr/share/applications/" + files[i]));
  }
  for (int i = 0;
       i < localdirectory.entryList(QStringList() << "*.desktop", QDir::Files)
               .count();
       i++) {
    allapps.push_back(parse(
        "/usr/local/share/applications/" +
        files[directory.entryList(QStringList() << "*.desktop", QDir::Files)
                  .count() +
              i]));
  }
  for (int i = 0;
       i < homedirectory.entryList(QStringList() << "*.desktop", QDir::Files)
               .count();
       i++) {
    allapps.push_back(parse(
        QDir::homePath() + "/.local/share/applications/" +
        files[directory.entryList(QStringList() << "*.desktop", QDir::Files)
                  .count() +
              localdirectory
                  .entryList(QStringList() << "*.desktop", QDir::Files)
                  .count() +
              i]));
  }
  std::sort(allapps.begin(), allapps.end());
  model.setmodel(allapps);

  categoryentry all;
  all.name = "All Applications";
  all.visibleName = "All Applications";
  all.icon = "applications-all";
  all.includes.push_back("All");
  allcategories.push_back(all);
  for (int i = 0;
       i < desktopdirectories.entryList(QStringList() << "*.directory").count();
       i++) {
    if (parseDirectory("/usr/share/desktop-directories/" + directoryfiles[i])
            .includes.count() != 0) {
      allcategories.push_back(parseDirectory("/usr/share/desktop-directories/" +
                                             directoryfiles[i]));
    }
  }
  categorymodel.setmodel(allcategories);
}

categoryentry appList::parseDirectory(QString path) {
  categoryentry output;
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << "cannot open file. " << path;
  }

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString temp = in.readLine();
    if (temp.split("=")[0] == "Name" || temp.split("=")[0] == "_Name") {
      output.name = temp.split("=")[1];
    }
    if (temp.split("=")[0] == "Name[" + locale.bcp47Name() + "]") {
      output.visibleName = temp.split("=")[1];
    }
    if (temp.split("=")[0] == "Icon") {
      output.icon = temp.split("=")[1];
    }
    if (temp.split("=")[0] == "_Nu_Includes") {
      output.includes = temp.split("=")[1].split(";");
    }
    if (temp.split("=")[0] == "_Nu_NotIncludes") {
      output.notIncludes = temp.split("=")[1].split(";");
    }
  }
  if (output.visibleName == "") {
    output.visibleName = output.name;
  }
  return output;
}

desktopentry appList::parse(QString path) {
  desktopentry output;
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << "cannot open file. " << path;
  }

  QTextStream in(&file);
  output.hidden = false;

  while (!in.atEnd()) {
    QString temp = in.readLine();
    if (temp.split("=")[0] == "Name" ||
        temp.split("=")[0] == "Name[" + locale.bcp47Name() + "]") {
      if (output.actions.count() == 0) {
        output.name = temp.split("=")[1];
      } else {
        output.actions.back().name = temp.split("=")[1];
      }
    }
    if (temp.split("=")[0] == "GenericName" ||
        temp.split("=")[0] == "GenericName[" + locale.bcp47Name() + "]") {
      output.genericname = temp.split("=")[1];
    }
    if (temp.split("=")[0] == "Type") {
      output.type = temp.split("=")[1];
    }
    if (temp.split("=")[0] == "Icon") {
      if (output.actions.count() == 0) {
        output.icon = temp.split("=")[1];
      } else {
        output.actions.back().icon = temp.split("=")[1];
      }
    }
    if (temp.split("=")[0] == "Exec") {
      int index = temp.indexOf("=");
      if (output.actions.count() == 0) {
        output.exec = temp.right(temp.count() - index - 1);
      } else {
        output.actions.back().exec = temp.right(temp.count() - index - 1);
      }
    }
    if (temp.split("=")[0] == "Path") {
      output.path = temp.split("=")[1];
    }
    if (temp.split("=")[0] == "Comment" ||
        temp.split("=")[0] == "Comment[" + locale.bcp47Name() + "]") {
      output.comment = temp.split("=")[1];
    }
    if (temp.split("=")[0] == "OnlyShowIn") {
      output.onlyshowin = temp.split("=")[1].split(";");
    }
    if (temp.split("=")[0] == "NotShowIn") {
      output.notshowin = temp.split("=")[1].split(";");
    }
    if (temp.split("=")[0] == "Categories") {
      output.categories = temp.split("=")[1].split(";");
    }
    if (temp.split("=")[0] == "Keywords" ||
        temp.split("=")[0] == "Keywords[" + locale.bcp47Name() + "]") {
      output.keywords = temp.split("=")[1].split(";");
    }
    if (temp.split("=")[0] == "Hidden") {
      if (temp.split("=")[1] == "true") {
        output.hidden = true;
      } else {
        output.hidden = false;
      }
    }
    if (temp.split("=")[0] == "NoDisplay") {
      if (temp.split("=")[1] == "true") {
        output.hidden = true;
      } else {
        output.hidden = false;
      }
    }
    if (temp.split(" ").count() != 1) {
      if (temp.split(" ")[1] == "Action") {
        actionentry emptyaction;
        output.actions.push_back(emptyaction);
      }
    }
  }
  if (output.onlyshowin.count() != 0 &&
      output.onlyshowin.indexOf("NuDesktop") == -1) {
    output.hidden = true;
  }
  for (int i = 0; i < output.actions.count(); i++) {
    QVariant var;
    var.setValue(output.actions[i]);
    output.vactions.append(var);
  }

  return output;
}
