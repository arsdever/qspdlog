#include "qspdlog_model.hpp"

#include <qdatetime>

namespace {

const char *level_names[] = {"Trace", "Debug",    "Info", "Warning",
                             "Error", "Critical", "Off"};

const char *column_names[] = {"Time", "Level", "Message"};

} // namespace

QSpdLogModel::QSpdLogModel(QObject *parent) : QAbstractTableModel(parent) {}

void QSpdLogModel::addEntry(entry_t entry) {
  insertRows(rowCount(), 1);
  _items.push_back(std::move(entry));
}

int QSpdLogModel::rowCount(const QModelIndex &parent) const {
  return _items.size();
}

int QSpdLogModel::columnCount(const QModelIndex &parent) const { return 3; }

QVariant QSpdLogModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    auto const &item = _items[index.row()];
    switch (index.column()) {
    case 0:
      return QDateTime::fromMSecsSinceEpoch(
                 std::chrono::duration_cast<std::chrono::milliseconds>(
                     item.time)
                     .count())
          .toString();
    case 1:
      return QString(level_names[item.level]);
    case 2:
      return QString::fromStdString(item.message);
    }
  }
  return QVariant();
}

QVariant QSpdLogModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    return QString(column_names[section]);
  return QVariant();
}