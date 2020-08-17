#include "hotkey.h"

#include <QJsonObject>

void Hotkey::write(QJsonArray &array)
{
  QJsonObject hotkey;
  hotkey["name"] = name;
  hotkey["activity"] = activity;
  hotkey["uuid"] = uuid;
  hotkey["combo"] = combo;

  array.append(hotkey);
}
