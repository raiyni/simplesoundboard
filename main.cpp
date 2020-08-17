#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickStyle>

#include "db.h"
#include "keymanager.h"
#include "page.hpp"
#include "soundmanager.h"

#include <Windows.h>
#include <pagemanager.h>
#include <iostream>
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[]) {
  Db::get()->loadFile();

  QQuickStyle::setStyle("Universal");
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  app.setOrganizationName("Dutchmen Technology");
  app.setOrganizationDomain("dutchmen.tech");

  QQmlApplicationEngine engine;
  engine.addImportPath(QStringLiteral("qrc:/"));

  qmlRegisterType<Page>("Page", 1, 0, "Page");
  qmlRegisterType<Hotkey>("Hotkey", 1, 0, "Hotkey");

  qmlRegisterSingletonType<Db>("Db", 1, 0, "Db", Db::singletonTypeProvider);
  qmlRegisterSingletonType<KeyManager>("KeyManager", 1, 0, "KeyManager", KeyManager::singletonTypeProvider);
  qmlRegisterSingletonType<PageManager>("PageManager", 1, 0, "PageManager", PageManager::singletonTypeProvider);
  qmlRegisterSingletonType<SoundManager>("SoundManager", 1, 0, "SoundManager", SoundManager::singletonTypeProvider);

  QObject::connect(KeyManager::get(), &KeyManager::save, Db::get(), &Db::save);
  QObject::connect(PageManager::get(), &PageManager::save, Db::get(), &Db::save);
  QObject::connect(SoundManager::get(), &SoundManager::save, Db::get(), &Db::save);

  const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                   &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
  }, Qt::QueuedConnection);
  engine.load(url);

  SetWindowsHookEx(WH_KEYBOARD_LL, KeyManager::LowLevelKeyBoardProc, NULL,0);

  return app.exec();
}
