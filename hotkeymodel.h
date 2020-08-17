#ifndef HOTKEYMODEL_H
#define HOTKEYMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QJsonArray>
#include <QJsonObject>
#include "hotkey.h"

class HotkeyModel : public QAbstractListModel
{
  Q_OBJECT
 public:
  enum HotkeyRols { NameRole = Qt::UserRole + 1, ActivityRole, UuidRole, ComboRole };

  ~HotkeyModel() {}

  QList<Hotkey *> hotkeys;

  HotkeyModel(QObject *parent = 0) : QAbstractListModel(parent) {}

  void insert(Hotkey *hotkey);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

 protected:
  QHash<int, QByteArray> roleNames() const;

 signals:
  void save();
};

#endif // HOTKEYMODEL_H
