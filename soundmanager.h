#pragma once
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QQmlEngine>
#include <QQmlListProperty>

#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib")

using namespace irrklang;

class SoundDevice : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString description MEMBER description CONSTANT);
  Q_PROPERTY(QString id MEMBER id CONSTANT);

 public:
  QString description;
  QString id;

  SoundDevice(QString id, QString desc) {
    this->id = id;
    this->description = desc;
  }
};

class SoundManager : public QObject {
  Q_OBJECT

  Q_PROPERTY(QList<QObject*> deviceList MEMBER deviceList CONSTANT);

  Q_PROPERTY(QString device1 MEMBER device1 NOTIFY device1Changed);
  Q_PROPERTY(QString device2 MEMBER device2 NOTIFY device2Changed);

 private:
  QList<QObject*> deviceList;

 public:
  ISoundEngine* engine1 = 0;
  ISoundEngine* engine2 = 0;
  QString device1 = "empty";
  QString device2 = "empty";

  static SoundManager* get();
  static QObject* singletonTypeProvider(QQmlEngine* engine, QJSEngine* scriptEngine);

  Q_INVOKABLE void loadEngine(QString index, QString deviceId);

  SoundManager(QObject* parent = 0) : QObject(parent) {
    deviceList = {new SoundDevice("empty", "")};
    ISoundDeviceList* devices = createSoundDeviceList();
    if (!devices) {
      return;
    }

    for (int i = 0; i < devices->getDeviceCount(); ++i) {
      QString id = QString::fromStdString(devices->getDeviceID(i));
      QString description = QString::fromStdString(devices->getDeviceDescription(i));

      QMap<QString, QString> map;

      SoundDevice* device = new SoundDevice(id, description);
      deviceList << device;
    }

    devices->drop();
  }

  Q_INVOKABLE int indexOf(QString str) {
    int idx = 0;
    for (auto obj : deviceList) {
      SoundDevice* sd = static_cast<SoundDevice*>(obj);
      if (sd->id == str) return idx;
      idx++;
    }

    return 0;
  }

  void write(QJsonObject& obj);
  void read(const QJsonObject& obj);

  Q_INVOKABLE void play(QString file);
 signals:
  void save();
  void device1Changed();
  void device2Changed();
};
