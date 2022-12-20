#pragma once

#include <qtreeview>

namespace spdlog {
class logger;
namespace sinks {
class sink;
} // namespace sinks
} // namespace spdlog

class QSpdLogModel;
class QSpdLogProxyModel;

class QSpdLog : public QTreeView {
public:
  explicit QSpdLog(QWidget *parent = nullptr);
  ~QSpdLog();

public:
  void clear();
  QWidget* filterWidget() const;
  std::shared_ptr<spdlog::sinks::sink> sink();

private slots:
  void updateFiltering();

private:
  QSpdLogModel *_sourceModel;
  QSpdLogProxyModel *_proxyModel;
  QWidget *_filterWidget;
  std::shared_ptr<spdlog::sinks::sink> _sink;
};
