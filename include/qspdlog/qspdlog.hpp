#pragma once

#include <qtreeview>

namespace spdlog {
class logger;
}

class QSpdLogModel;

class QSpdLog : public QTreeView {
public:
  Q_OBJECT

public:
  explicit QSpdLog(QWidget *parent = nullptr);
  ~QSpdLog();

public:
  void registerLogger(std::shared_ptr<spdlog::logger> logger);

private:
  class QSpdLogModel *_model;
  std::list<std::weak_ptr<spdlog::logger>> _loggers;
};