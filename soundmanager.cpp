#include "soundmanager.h"

static SoundManager* instance;

static QString empty = "empty";
static QString defaultDevice = "default";

SoundManager* SoundManager::get() {
  if (!instance) {
    instance = new SoundManager;
  }

  return instance;
}

QObject* SoundManager::singletonTypeProvider(QQmlEngine* engine, QJSEngine* scriptEngine) {
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)
  return SoundManager::get();
}

void SoundManager::loadEngine(QString index, QString deviceId) {
  ISoundEngine* engine = 0;
  if (deviceId != empty) {
    if (deviceId == defaultDevice) {
      engine = createIrrKlangDevice();
    } else {
      engine = createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT, irrklang::ESEO_DEFAULT_OPTIONS, deviceId.toStdString().c_str());
    }
  }

  if (index == "device1") {
    if (engine1) {
      engine1->drop();
    }

    engine1 = engine;
    device1 = deviceId;
  } else {
    if (engine2) {
      engine2->drop();
    }

    engine2 = engine;
    device2 = deviceId;
  }
}

void SoundManager::write(QJsonObject& obj) {
  if (engine1) {
    obj["device1"] = device1;
  } else {
    obj["device1"] = empty;
  }

  if (engine2) {
    obj["device2"] = device2;
  } else {
    obj["device2"] = empty;
  }
}

void SoundManager::read(const QJsonObject& obj) {
  loadEngine("device1", obj["device1"].toString());
  loadEngine("device2", obj["device2"].toString());
}

void SoundManager::play(QString file) {
  if (file == "") return;

  std::string str = file.toStdString();
  const char* c = str.c_str();
  if (engine1) {
    engine1->play2D(c);
  }

  if (engine2) {
    engine2->play2D(c);
  }
}
