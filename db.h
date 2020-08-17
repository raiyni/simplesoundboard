#ifndef DB_H
#define DB_H

#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include "hotkeymodel.h"
#include "page.hpp"
#include "uuid.hpp"

class Db : public QObject {
  Q_OBJECT

public:
  static Db* get();
  static QObject* singletonTypeProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

  void write();
  void read(const QJsonObject &obj);
  bool loadFile();

 public slots:
  Q_INVOKABLE void save() {
    write();
  }
};

#endif // DB_H
