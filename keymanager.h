#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include <Windows.h>
#include <QObject>
#include <QQmlEngine>
#include <QRunnable>
#include <ssbstd.hpp>

class KeyManager : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString previousPage READ previousPageStr NOTIFY previousPageChanged)
  Q_PROPERTY(QString nextPage READ nextPageStr NOTIFY nextPageChanged)
 public:
  static KeyManager* get();
  static QObject* singletonTypeProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
  static LRESULT CALLBACK LowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);

  Q_INVOKABLE void acceptHotkey(QString uuid);
  void write(QJsonObject& obj);
  void read(const QJsonObject& obj);

  void handleHotkey(int& mods);

 private:
  boolean acceptMode;
  QString activeHotkey;

  int nextPage = 0;
  int previousPage = 0;

  QString previousPageStr() { return getKeyStr(previousPage); }

  QString nextPageStr() { return getKeyStr(nextPage); }

  void setHotkey(int& mods);

 signals:
  void previousPageChanged();
  void nextPageChanged();
  void save();
};

class KeyRunner : public QRunnable {
  KeyManager* km;
  int mods;

 public:
  KeyRunner(KeyManager* k, int mods) {
    km = k;
    this->mods = mods;
  }

  void run() override { km->handleHotkey(mods); }
};
#endif // KEYMANAGER_H
