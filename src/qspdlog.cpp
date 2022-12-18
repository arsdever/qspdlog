#include "qspdlog/qspdlog.hpp"

#include "qspdlog_model.hpp"
#include "qt_logger_sink.hpp"

#include <spdlog/logger.h>

QSpdLog::QSpdLog(QWidget *parent)
    : QTreeView(parent), _model(new QSpdLogModel) {
  Q_INIT_RESOURCE(qspdlog_resources);
  setModel(_model);
  _sink = std::make_shared<qt_logger_sink_mt>(_model);
}

QSpdLog::~QSpdLog() {
  std::static_pointer_cast<qt_logger_sink_mt>(_sink)->invalidate();
}

void QSpdLog::clear() { _model->clear(); }

spdlog::sink_ptr QSpdLog::sink() { return _sink; }
