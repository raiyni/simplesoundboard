#include "keymanager.h"
#include "pagemanager.h"
#include "soundmanager.h"

using namespace std;

static KeyManager* instance;

KeyManager* KeyManager::get() {
    if (!instance) {
        instance = new KeyManager;
      }

    return instance;
}

QObject* KeyManager::singletonTypeProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)

  return KeyManager::get();
}


void KeyManager::acceptHotkey(QString uuid)
{
  acceptMode = true;
  activeHotkey = uuid;
}

void KeyManager::setHotkey(int& mods) {
  if (activeHotkey == "previousPage") {
    previousPage = mods;
    emit previousPageChanged();
    emit save();
  } else if (activeHotkey == "nextPage") {
    nextPage = mods;
    emit nextPageChanged();
    emit save();
  } else if (activeHotkey == "currentPage") {
    auto pm = PageManager::get();
    pm->currentPage->combo = mods;
    emit pm->currentPage->hotkeyChanged();
    emit save();
  } else {
    auto pm = PageManager::get();
    int idx = 0;
    auto hkm = pm->currentPage->hotkeys;
    for (auto hk : hkm->hotkeys) {
      if (hk->uuid == activeHotkey) {
        auto modelIndex = hkm->index(idx);
        hkm->setData(modelIndex, mods, HotkeyModel::ComboRole);
        break;
      }
      idx++;
    }
  }

  acceptMode = false;
}

void KeyManager::handleHotkey(int& mods) {
  auto pm = PageManager::get();
  auto sm = SoundManager::get();
  for (auto hk : pm->currentPage->hotkeys->hotkeys) {
    if (hk->combo == mods) {
      sm->play(hk->activity);
    }
  }

  if (previousPage == mods) {
    pm->previousPage();
  } else if (nextPage == mods) {
    pm->nextPage();
  } else {
    int idx = 0;
    for (auto p : pm->pages) {
      if (p->combo == mods) {
        pm->setCurrentPage(idx);
        return;
      }

      idx++;
    }
  }
}

inline bool key_pressed(int key) { return (GetAsyncKeyState(key) != 0); }

LRESULT CALLBACK KeyManager::LowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    switch (wParam) {
      case WM_KEYUP:
      case WM_SYSKEYUP:
        PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
        switch (p->vkCode) {
          case VK_SHIFT:
          case VK_RSHIFT:
          case VK_LSHIFT:
          case VK_CONTROL:
          case VK_LCONTROL:
          case VK_RCONTROL:
          case VK_LMENU:
          case VK_RMENU:
          case VK_MENU:
            return CallNextHookEx(NULL, nCode, wParam, lParam);
            ;
        }

        auto km = KeyManager::get();

        int shift = key_pressed(VK_SHIFT);
        int ctrl = key_pressed(VK_CONTROL);
        int alt = key_pressed(VK_MENU);

        int mods = shift | (ctrl << 1) | (alt << 2) | (p->vkCode << 3);
        if (km->acceptMode) {
          km->setHotkey(mods);
        } else {
          km->handleHotkey(mods);
        }
    }
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void KeyManager::write(QJsonObject& obj) {
  obj["previousPage"] = previousPage;
  obj["nextPage"] = nextPage;
}

void KeyManager::read(const QJsonObject& obj) {
  previousPage = obj["previousPage"].toInt();
  nextPage = obj["nextPage"].toInt();
}
