#include "qspdlog/qspdlog.hpp"

#include "qspdlog_model.hpp"
#include "qt_logger_sink.hpp"

#include <spdlog/logger.h>

QSpdLog::QSpdLog(QWidget *parent)
    : QTreeView(parent), _model(new QSpdLogModel) {

  setModel(_model);
}

QSpdLog::~QSpdLog() {}

void QSpdLog::registerLogger(std::shared_ptr<spdlog::logger> logger) {
  std::shared_ptr<qt_logger_sink_mt> sink = std::shared_ptr<qt_logger_sink_mt>(
      new qt_logger_sink_mt(*_model), [logger](qt_logger_sink_mt *p) {
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