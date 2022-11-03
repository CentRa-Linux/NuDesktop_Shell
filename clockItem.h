#ifndef CLOCKITEM_H
#define CLOCKITEM_H

#include <QTime>
#include <QTimer>

#include "clickItem.h"

class clockItem : public clickItem {
public:
  clockItem();

public slots:
  void updateTime();
};

#endif // CLOCKITEM_H
