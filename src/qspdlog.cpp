#include "qspdlog/qspdlog.hpp"

#include "qspdlog_filter_widget.hpp"
#include "qspdlog_model.hpp"
#include "qspdlog_proxy_model.hpp"
#include "qt_logger_sink.hpp"

#include <qlineedit>
#include <spdlog/logger.h>

QSpdLog::QSpdLog(QWidget *parent)
    : QTreeView(parent), _sourceModel(new QSpdLogModel),
      _proxyModel(new QSpdLogProxyModel),
      _filterWidget(new QSpdLogFilterWidget) {
  Q_INIT_RESOURCE(qspdlog_resources);
  setModel(_proxyModel);

  _proxyModel->setSourceModel(_sourceModel);

  QSpdLogFilterWidget *filterWidget =
      static_cast<QSpdLogFilterWidget *>(_filterWidget);

  connect(filterWidget, &QSpdLogFilterWidget::filterChanged, this,
          &QSpdLog::updateFiltering);

  setRootIsDecorated(false);

  _sink = std::make_shared<qt_logger_sink_mt>(_sourceModel);
}

QSpdLog::~QSpdLog() {
  std::static_pointer_cast<qt_logger_sink_mt>(_sink)->invalidate();
}

void QSpdLog::clear() { _sourceModel->clear(); }

QWidget *QSpdLog::filterWidget() const { return _filterWidget; }

void QSpdLog::updateFiltering() {
  QSpdLogFilterWidget *filterWidget =
      static_cast<QSpdLogFilterWidget *>(_filterWidget);

  QSpdLogFilterWidget::FilteringSettings settings =
      filterWidget->filteringSettings();

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
