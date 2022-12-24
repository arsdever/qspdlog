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

static constexpr std::array<const char*, 3> column_names = { "Level",
                                                             "Time",
                                                             "Message" };

} // namespace

QSpdLogModel::QSpdLogModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

void QSpdLogModel::addEntry(entry_t entry)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    _items.push_back(std::move(entry));

    endInsertRows();
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

int QSpdLogModel::columnCount(const QModelIndex& parent) const { return 3; }

QVariant QSpdLogModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= _items.size())
        return QVariant();

    switch (role) {
        case Qt::DisplayRole: {
            auto const& item = _items[ index.row() ];
            switch (index.column()) {
                case 0: {
                    return QString(level_names[ item.level ]);
                }

                case 1: {
                    return QDateTime::fromMSecsSinceEpoch(
                        std::chrono::duration_cast<std::chrono::milliseconds>(
                            item.time
                        )
                            .count()
                    );
                }

                case 2: {
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
