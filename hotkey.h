#ifndef HOTKEY_H
#define HOTKEY_H

#include <QString>
#include <QJsonArray>

#include "ssbstd.hpp"

class Hotkey : public QObject {
  Q_OBJECT
 public:
  void write(QJsonArray &json);

  int combo = 0;

  QString name;
  QString activity;
  QString uuid;

  QString text() { return getKeyStr(combo); }
};

#endif  // HOTKEY_H
