#include "qspdlog_model.hpp"

#include <qdatetime>
#include <qfile>
#include <qicon>

namespace {

static constexpr std::array<const char *, 6> icon_names = {
    ":/res/trace.png", ":/res/debug.png", ":/res/info.png",
    ":/res/warn.png",  ":/res/error.png", ":/res/critical.png"};

static constexpr std::array<const char *, 7> level_names = {
    "Trace", "Debug", "Info", "Warning", "Error", "Critical", "Off"};

static constexpr std::array<const char *, 3> column_names = {"Level", "Time",
                                                             "Message"};

} // namespace

QSpdLogModel::QSpdLogModel(QObject *parent) : QAbstractListModel(parent) {}

void QSpdLogModel::addEntry(entry_t entry) {

  beginInsertRows(QModelIndex(), rowCount(), rowCount() + 1);

  _items.push_back(std::move(entry));

  endInsertRows();
}

int QSpdLogModel::rowCount(const QModelIndex &parent) const {
  return _items.size();
}

int QSpdLogModel::columnCount(const QModelIndex &parent) const { return 3; }

QVariant QSpdLogModel::data(const QModelIndex &index, int role) const {
  switch (role) {
  case Qt::DisplayRole: {
    auto const &item = _items[index.row()];
    switch (index.column()) {
    case 0:
      return QString(level_names[item.level]);
    case 1:
      return QDateTime::fromMSecsSinceEpoch(
                 std::chrono::duration_cast<std::chrono::milliseconds>(
                     item.time)
                     .count())
          .toString();
    case 2:
      return QString::fromStdString(item.message);
    }
  }
  case Qt::DecorationRole: {
    if (index.column() == 0) {
      const auto &item = _items[index.row()];
      if (item.level >= 0 && item.level < icon_names.size()) {
        return QIcon(QString(icon_names[_items[index.row()].level]));
      }
    }
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