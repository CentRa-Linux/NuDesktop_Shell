#include "clockItem.h"

clockItem::clockItem() {
  QTimer *timer = new QTimer(this);
  QObject::connect(timer, &QTimer::timeout, this, &clockItem::updateTime);
  timer->start(1000);
}

void clockItem::updateTime() {
  QTime time = QTime::currentTime();
  QString text = time.toString("hh:mm:ss");
  if (clickItem::created != false) {
    clickItemInstance->setProperty("textcontent", text);
  }
}
