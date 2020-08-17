#include "hotkeymodel.h"
#include "uuid.hpp"

void HotkeyModel::insert(Hotkey *hotkey) {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  hotkeys << hotkey;
  endInsertRows();
}

int HotkeyModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return hotkeys.count();
}


QVariant HotkeyModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= hotkeys.count())
        return QVariant();

    Hotkey *hotkey = hotkeys[index.row()];
    if (role == NameRole)
      return hotkey->name;
    else if (role == ActivityRole)
      return hotkey->activity;
    else if (role == UuidRole)
      return hotkey->uuid;
    else if (role == ComboRole)
      return hotkey->text();

    return QVariant();
}

QHash<int, QByteArray> HotkeyModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ActivityRole] = "action";
    roles[UuidRole] = "uuid";
    roles[ComboRole] = "combo";
    return roles;
}

bool HotkeyModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (index.row() < 0 || index.row() >= hotkeys.count()) return false;

  Hotkey *hotkey = hotkeys[index.row()];
  if (role == NameRole) {
    hotkey->name = (value.toString());
    emit dataChanged(index, index);
    emit save();
    return true;
  }
  if (role == ActivityRole) {
    hotkey->activity = (value.toString());
    emit dataChanged(index, index);
    emit save();
    return true;
  }
  if (role == ComboRole) {
    hotkey->combo = (value.toUInt());
    emit dataChanged(index, index);
    emit save();
    return true;
  }

  return false;
}
