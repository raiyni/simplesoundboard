#pragma once

#include <QList>
#include <QObject>
#include <QQmlEngine>
#include <QQmlListProperty>
#include <page.hpp>

class PageManager : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Page* currentPage MEMBER currentPage NOTIFY currentPageChanged);
  Q_PROPERTY(QQmlListProperty<Page> pages READ pageList NOTIFY pagesChanged);

 public:
  QList<Page*> pages;
  Page* currentPage;

  PageManager(QObject* parent = 0) : QObject(parent) { QObject::connect(this, &PageManager::currentPageChanged, this, &PageManager::signalProxy); }

  static PageManager* get();
  static QObject* singletonTypeProvider(QQmlEngine* engine, QJSEngine* scriptEngine);

  void nextPage() {
    int idx = pages.indexOf(currentPage);
    setCurrentPage(++idx);
  }

  void previousPage() {
    int idx = pages.indexOf(currentPage);
    setCurrentPage(--idx);
  }

  Q_INVOKABLE void newPage() {
    Page* p = new Page;
    p->name = "New Page";
    p->uuid = QString::fromStdString(uuid::generate());

    QObject::connect(p, &Page::save, this, &PageManager::save);

    currentPage = p;
    pages.append(currentPage);

    emit currentPageChanged();
  }

  Q_INVOKABLE void setCurrentPage(int i) {
    if (i >= pages.count())
      i = 0;
    else if (i < 0)
      i = pages.count() - 1;

    currentPage = pages[i];
    emit currentPageChanged();
  }

  Q_INVOKABLE void deletePage() {
    if (pages.count() < 2) return;
    int idx = pages.indexOf(currentPage);
    pages.removeAt(idx);

    if (idx >= pages.count()) idx--;

    currentPage = pages[idx];
    emit currentPageChanged();
  }

  Q_INVOKABLE void newHotkey() { currentPage->newHotkey(); }

  QQmlListProperty<Page> pageList() { return QQmlListProperty<Page>(this, pages); }
  void read(const QJsonObject& obj);
  void write(QJsonObject& obj);
 signals:
  void currentPageChanged();
  void pagesChanged();
  void save();

 private slots:
  void signalProxy() {
    emit pagesChanged();
    emit save();
  }
};

