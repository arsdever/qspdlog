#include <qlineedit>
#include <qscrollbar>
#include <spdlog/logger.h>

#include "qspdlog/qspdlog.hpp"

#include "qspdlog_model.hpp"
#include "qspdlog_proxy_model.hpp"
#include "qspdlog_toolbar.hpp"
#include "qt_logger_sink.hpp"

QSpdLog::QSpdLog(QWidget* parent)
    : QTreeView(parent)
    , _sourceModel(new QSpdLogModel)
    , _proxyModel(new QSpdLogProxyModel)
    , _toolbar(new QSpdLogToolBar)
{
    Q_INIT_RESOURCE(qspdlog_resources);
    setModel(_proxyModel);

    _proxyModel->setSourceModel(_sourceModel);

    QSpdLogToolBar* toolbar = static_cast<QSpdLogToolBar*>(_toolbar);

    connect(
        toolbar, &QSpdLogToolBar::filterChanged, this, &QSpdLog::updateFiltering
    );
    connect(
        toolbar,
        &QSpdLogToolBar::autoScrollPolicyChanged,
        this,
        &QSpdLog::updateAutoScrollPolicy
    );
    connect(
        _sourceModel,
        &QAbstractItemModel::rowsAboutToBeInserted,
        this,
        [ this ](const QModelIndex& parent, int first, int last) {
        auto bar = verticalScrollBar();
        _scrollIsAtBottom = bar ? (bar->value() == bar->maximum()) : false;
        });

    setRootIsDecorated(false);

    _sink = std::make_shared<qt_logger_sink_mt>(_sourceModel);
}

QSpdLog::~QSpdLog()
{
    std::static_pointer_cast<qt_logger_sink_mt>(_sink)->invalidate();
}

void QSpdLog::clear() { _sourceModel->clear(); }

QWidget* QSpdLog::toolbar() const { return _toolbar; }

void QSpdLog::updateFiltering()
{
    QSpdLogToolBar* toolbar = static_cast<QSpdLogToolBar*>(_toolbar);

    QSpdLogToolBar::FilteringSettings settings = toolbar->filteringSettings();

    _proxyModel->setFilterCaseSensitivity(
        settings.isCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive
    );

    if (settings.isRegularExpression) {
        QRegularExpression regex(settings.text);

        if (!regex.isValid())
            return;

        _proxyModel->setFilterRegularExpression(settings.text);
    } else {
        _proxyModel->setFilterFixedString(settings.text);
    }
}

void QSpdLog::updateAutoScrollPolicy(int index)
{
    QSpdLogToolBar::AutoScrollPolicy policy =
        static_cast<QSpdLogToolBar::AutoScrollPolicy>(index);

    QObject::disconnect(_scrollConnection);

    switch (policy) {
        case QSpdLogToolBar::AutoScrollPolicyEnabled: {
            _scrollConnection = connect(
                _sourceModel,
                &QSpdLogModel::rowsInserted,
                this,
                &QSpdLog::scrollToBottom
            );
            break;
        }

        case QSpdLogToolBar::AutoScrollPolicyEnabledIfBottom: {
            _scrollConnection = connect(
                _sourceModel,
                &QSpdLogModel::rowsInserted,
                this,
                [ this ]() {
                // We can't check if the scrollbar is at the bottom here because
                // the new rows are already inserted and the position of the
                // scrollbar may not be at the bottom of the widget anymore.
                // That's why the scroll position is checked before actually
                // adding the rows (AKA in the rowsAboutToBeInserted signal).
                if (_scrollIsAtBottom)
                    scrollToBottom();
                });
            break;
        }

        default: {
            // The connection is already disconnected. No need for handling the
            // AutoScrollPolicyDisabled case.
            break;
        }
    }
}

spdlog::sink_ptr QSpdLog::sink() { return _sink; }
