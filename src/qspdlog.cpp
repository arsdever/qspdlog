#include "qspdlog/qspdlog.hpp"

#include "qspdlog_model.hpp"
#include "qspdlog_proxy_model.hpp"
#include "qspdlog_toolbar.hpp"
#include "qt_logger_sink.hpp"

#include <qlineedit>
#include <spdlog/logger.h>

QSpdLog::QSpdLog(QWidget *parent)
    : QTreeView(parent), _sourceModel(new QSpdLogModel),
      _proxyModel(new QSpdLogProxyModel), _toolbar(new QSpdLogToolBar) {
  Q_INIT_RESOURCE(qspdlog_resources);
  setModel(_proxyModel);

  _proxyModel->setSourceModel(_sourceModel);

  QSpdLogToolBar *toolbar = static_cast<QSpdLogToolBar *>(_toolbar);

  connect(toolbar, &QSpdLogToolBar::filterChanged, this,
          &QSpdLog::updateFiltering);

  setRootIsDecorated(false);

  _sink = std::make_shared<qt_logger_sink_mt>(_sourceModel);
}

QSpdLog::~QSpdLog() {
  std::static_pointer_cast<qt_logger_sink_mt>(_sink)->invalidate();
}

void QSpdLog::clear() { _sourceModel->clear(); }

QWidget *QSpdLog::toolbar() const { return _toolbar; }

void QSpdLog::updateFiltering() {
  QSpdLogToolBar *toolbar = static_cast<QSpdLogToolBar *>(_toolbar);

  QSpdLogToolBar::FilteringSettings settings = toolbar->filteringSettings();

  _proxyModel->setFilterCaseSensitivity(
      settings.isCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);

  if (settings.isRegularExpression) {
    QRegularExpression regex(settings.text);

    if (!regex.isValid()) {
      return;
    }

    _proxyModel->setFilterRegularExpression(settings.text);
  } else {
    _proxyModel->setFilterFixedString(settings.text);
  }
}

spdlog::sink_ptr QSpdLog::sink() { return _sink; }
