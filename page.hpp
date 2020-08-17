#ifndef PAGEMODEL_H
#define PAGEMODEL_H

#include <QAbstractListModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include "hotkeymodel.h"
#include "ssbstd.hpp"
#include "uuid.hpp"

class Page : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged);
  Q_PROPERTY(QString hotkey READ hotkeyStr NOTIFY hotkeyChanged);
  Q_PROPERTY(HotkeyModel* hotkeys MEMBER hotkeys CONSTANT);

 public:
  QString name;
  QString uuid;
  int combo = 0;

  HotkeyModel* hotkeys;

  Page() {
    hotkeys = new HotkeyModel();
    QObject::connect(hotkeys, &HotkeyModel::save, this, &Page::save);
  }

  ~Page() { delete hotkeys; }

  QString hotkeyStr() { return getKeyStr(combo); }

  void newHotkey() {
    QString guid = QString::fromStdString(uuid::generate());
    QString name;
    QString action;

    auto hk = new Hotkey;
    hk->name = name;
    hk->activity = action;
    hk->uuid = guid;

    hotkeys->insert(hk);
    emit save();
  }

  void write(QJsonArray &array) {
    QJsonObject obj;
    obj["name"] = name;
    obj["uuid"] = uuid;

    QJsonArray hks;
    for (auto hotkey : hotkeys->hotkeys) {
      hotkey->write(hks);
    }

    obj["hotkeys"] = hks;
    obj["combo"] = combo;
    array.append(obj);
  }

  void read(QJsonObject& obj) {
    auto ref = obj["hotkeys"];
    if (!ref.isArray()) return;

    auto array = ref.toArray();
    for (auto vf : array) {
      auto o = vf.toObject();
      auto hk = new Hotkey;
      hk->name = o["name"].toString();
      hk->uuid = o["uuid"].toString();
      hk->activity = o["activity"].toString();
      hk->combo = o["combo"].toInt();

      hotkeys->insert(hk);
    }
  }
 signals:
  void nameChanged();
  void hotkeyChanged();
  void save();
};

#endif // PAGEMODEL_H
