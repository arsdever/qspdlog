#pragma once

#include <qtreeview>

namespace spdlog {
class logger;
}

class QSpdLogModel;

class QSpdLog : public QTreeView {
public:
  explicit QSpdLog(QWidget *parent = nullptr);
  ~QSpdLog();

public:
  void registerLogger(std::shared_ptr<spdlog::logger> logger);

  void clear();

private:
  class QSpdLogModel *_model;
  std::list<std::weak_ptr<spdlog::logger>> _loggers;
};