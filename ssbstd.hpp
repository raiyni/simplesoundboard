#pragma once
#include <Windows.h>
#include <QList>
#include <QString>

static QString GetKeyName(int virtualKey) {
  unsigned int scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

  // because MapVirtualKey strips the extended bit for some keys
  switch (virtualKey) {
    case VK_LEFT:
    case VK_UP:
    case VK_RIGHT:
    case VK_DOWN:  // arrow keys
    case VK_PRIOR:
    case VK_NEXT:  // page up and page down
    case VK_END:
    case VK_HOME:
    case VK_INSERT:
    case VK_DELETE:
    case VK_DIVIDE:  // numpad slash
    case VK_NUMLOCK: {
      scanCode |= 0x100;  // set extended bit
      break;
    }
  }

  LPSTR keyName = new CHAR[50];
  if (GetKeyNameTextA(scanCode << 16, keyName, sizeof(keyName)) != 0) {
    return keyName;
  } else {
    return "[Error]";
  }
}

static QString getKeyStr(int& mods) {
  if (mods == 0) {
    return "undefined";
  }

  QList<QString> list;
  if (mods & 1) {
    list << "shift";
  }
  if (mods & 2) {
    list << "ctrl";
  }
  if (mods & 4) {
    list << "alt";
  }

  list << GetKeyName(mods >> 3);
  return list.join(" + ");
}
