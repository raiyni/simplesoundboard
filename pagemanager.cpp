#include "pagemanager.h"
#include <QJsonArray>
#include <QJsonObject>

static PageManager* instance;

PageManager* PageManager::get() {
  if (!instance) {
    instance = new PageManager;
  }

  return instance;
}

QObject* PageManager::singletonTypeProvider(QQmlEngine* engine, QJSEngine* scriptEngine) {
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)
  return PageManager::get();
}

void PageManager::read(const QJsonObject& obj) {
  const QJsonValue& ref = obj["pages"];
  if (!ref.isArray()) return;

  QJsonArray array = ref.toArray();
  for (auto p : array) {
    if (!p.isObject()) continue;

    auto obj = p.toObject();
    auto page = new Page;
    page->name = obj["name"].toString();
    page->uuid = obj["uuid"].toString();
    page->combo = obj["combo"].toInt();

    this->pages << page;

    QObject::connect(page, &Page::save, this, &PageManager::save);
    page->read(obj);
  }

  this->currentPage = this->pages.at(obj["currentPage"].toInt());
}

void PageManager::write(QJsonObject& obj) {
  QJsonArray array;

  for (auto page : pages) {
    page->write(array);
  }

  obj["pages"] = array;
  obj["currentPage"] = pages.indexOf(currentPage);
}
