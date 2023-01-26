#include <QDateTime>
#include <QFile>
#include <QIcon>

#include "qspdlog_model.hpp"

namespace
{

static constexpr std::array<const char*, 6> icon_names = {
    ":/res/trace.png", ":/res/debug.png", ":/res/info.png",
    ":/res/warn.png",  ":/res/error.png", ":/res/critical.png"
};

static constexpr std::array<const char*, 7> level_names = {
    "Trace", "Debug", "Info", "Warning", "Error", "Critical", "Off"
};

enum class Column { Level = 0, Logger, Time, Message, Last };

static constexpr std::array<const char*, 4> column_names = {
    "Level", "Logger", "Time", "Message"
};

} // namespace

QSpdLogModel::QSpdLogModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

void QSpdLogModel::addEntry(entry_t entry)
{
    if(_maxEntries > 0 && _items.size() == _maxEntries) {
        beginRemoveRows(QModelIndex(), 0, 0);
        _items.pop_front();
        endRemoveRows();
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    _items.push_back(std::move(entry));

    endInsertRows();
}

void QSpdLogModel::setMaxEntries(std::optional<std::size_t> maxEntries) {
    _maxEntries = maxEntries;
    // Incase the new maximum is below the current amount of items.
    if(_maxEntries > 0 && _items.size() > _maxEntries) {
        std::size_t offset = _items.size() - _maxEntries.value();
        beginRemoveRows(QModelIndex(), 0, offset-1);
        _items.erase(_items.begin(), _items.begin()+offset);
        endRemoveRows();
    }
}

std::optional<std::size_t> QSpdLogModel::getMaxEntries() const
{
    return _maxEntries;
}

void QSpdLogModel::clear()
{
    beginResetModel();
    _items.clear();
    endResetModel();
}

int QSpdLogModel::rowCount(const QModelIndex& parent) const
{
    return _items.size();
}

int QSpdLogModel::columnCount(const QModelIndex& parent) const
{
    return static_cast<std::size_t>(Column::Last);
}

QVariant QSpdLogModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= _items.size())
        return QVariant();

    switch (role) {
        case Qt::DisplayRole: {
            auto const& item = _items[ index.row() ];
            switch (static_cast<Column>(index.column())) {
                case Column::Level: {
                    return QString(level_names[ item.level ]);
                }

                case Column::Logger: {
                    return QString::fromStdString(item.loggerName);
                }

                case Column::Time: {
                    return QDateTime::fromMSecsSinceEpoch(
                        std::chrono::duration_cast<std::chrono::milliseconds>(
                            item.time
                        )
                            .count()
                    );
                }

                case Column::Message: {
                    return QString::fromStdString(item.message);
                }

                default: {
                    break;
                }
            }

            break;
        }

        case Qt::DecorationRole: {
            if (index.column() == 0) {
                const auto& item = _items[ index.row() ];
                if (item.level >= 0 && item.level < icon_names.size()) {
                    return QIcon(
                        QString(icon_names[ _items[ index.row() ].level ])
                    );
                }
            }

            break;
        }

        default: {
            break;
        }
    }

    return QVariant();
}

QVariant QSpdLogModel::headerData(
    int section, Qt::Orientation orientation, int role
) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return QString(column_names[ section ]);

    return QVariant();
}
