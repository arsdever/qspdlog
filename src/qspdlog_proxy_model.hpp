#pragma once

#include <qsortfilterproxymodel>

class QSpdLogProxyModel : public QSortFilterProxyModel {
  Q_OBJECT

public:
  QSpdLogProxyModel(QObject *parent = nullptr);
};
