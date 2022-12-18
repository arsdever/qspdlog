#pragma once

#include <qtreeview>

namespace spdlog {
class logger;
}

class QSpdLogModel;
class QSpdLogProxyModel;

class QSpdLog : public QTreeView {
public:
  explicit QSpdLog(QWidget *parent = nullptr);
  ~QSpdLog();

public:
  void registerLogger(std::shared_ptr<spdlog::logger> logger);

  void clear();

  QWidget* filterWidget() const;

private:
  QSpdLogModel *_sourceModel;
  QSpdLogProxyModel *_proxyModel;
  std::list<std::weak_ptr<spdlog::logger>> _loggers;
  QWidget *_filterWidget;
};
