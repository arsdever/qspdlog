#include "qspdlog_proxy_model.hpp"

#include "qspdlog_model.hpp"

QSpdLogProxyModel::QSpdLogProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {
  setFilterKeyColumn(-1);
}
