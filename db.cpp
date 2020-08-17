#include "db.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "keymanager.h"
#include "pagemanager.h"
#include "soundmanager.h"

#include <QKeyEvent>

static Db* instance;

Db* Db::get()
{
  if (!instance) {
      instance = new Db;
    }

  return instance;
}


QObject* Db::singletonTypeProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)
  return Db::get();
}


void Db::write() {
  QJsonObject obj;
  auto pm = PageManager::get();
  pm->write(obj);

  auto km = KeyManager::get();
  km->write(obj);

  auto sm = SoundManager::get();
  sm->write(obj);

  QFile saveFile(QStringLiteral("save.json"));
  if (!saveFile.open(QIODevice::WriteOnly)) {
    qWarning("Couldn't open save file.");
    return;
  }

  QJsonDocument saveDoc(obj);
  saveFile.write(saveDoc.toJson());
}

bool Db::loadFile() {
  QFile loadFile(QStringLiteral("save.json"));

  QByteArray saveData;

  if (!loadFile.open(QIODevice::ReadOnly)) {
    std::string defaultData = R"({
  "currentPage": 0,
  "previousPage": 0,
  "nextPage": 0,
  "device1": "empty",
  "device2": "empty",
  "pages": [{
    "uuid": "1",
    "name": "New Page",
    "hotkeys": [],
    "combo": 0
  }]
})";
    saveData = QByteArray::fromStdString(defaultData);
  } else {
    saveData = loadFile.readAll();
  }

  QJsonParseError* err = new QJsonParseError;
  QJsonDocument loadDoc(QJsonDocument::fromJson(saveData, err));

  read(loadDoc.object());
  return true;
}

void Db::read(const QJsonObject& obj) {
  auto pm = PageManager::get();
  pm->read(obj);

  auto km = KeyManager::get();
  km->read(obj);

  auto sm = SoundManager::get();
  sm->read(obj);
}
