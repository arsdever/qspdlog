#pragma once

#include <qtreeview>

namespace spdlog {
class logger;
namespace sinks {
class sink;
} // namespace sinks
} // namespace spdlog

class QSpdLogModel;

class QSpdLog : public QTreeView {
public:
  explicit QSpdLog(QWidget *parent = nullptr);
  ~QSpdLog();

public:
  void clear();

  std::shared_ptr<spdlog::sinks::sink> sink();

private:
  class QSpdLogModel *_model;
  std::list<std::weak_ptr<spdlog::logger>> _loggers;
  std::shared_ptr<spdlog::sinks::sink> _sink;
};
