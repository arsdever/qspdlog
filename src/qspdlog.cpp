#include "qspdlog/qspdlog.hpp"

#include "qspdlog_model.hpp"
#include "qspdlog_proxy_model.hpp"
#include "qt_logger_sink.hpp"

#include <qlineedit>
#include <spdlog/logger.h>

QSpdLog::QSpdLog(QWidget *parent)
    : QTreeView(parent), _sourceModel(new QSpdLogModel),
      _proxyModel(new QSpdLogProxyModel) {
  setModel(_proxyModel);

  _proxyModel->setSourceModel(_sourceModel);

  setRootIsDecorated(false);
}

QSpdLog::~QSpdLog() {}

void QSpdLog::registerLogger(std::shared_ptr<spdlog::logger> logger) {
  std::shared_ptr<qt_logger_sink_mt> sink = std::shared_ptr<qt_logger_sink_mt>(
      new qt_logger_sink_mt(*_sourceModel), [logger](qt_logger_sink_mt *p) {
        p->flush();
        auto &sinks = logger->sinks();
        logger->sinks().erase(
            std::find_if(sinks.begin(), sinks.end(),
                         [p](const std::shared_ptr<spdlog::sinks::sink> &s) {
                           return s.get() == p;
                         }));
        delete p;
      });
  logger->sinks().push_back(sink);
  _loggers.push_back(logger);
}

void QSpdLog::clear() { _sourceModel->clear(); }
