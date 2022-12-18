#pragma once

#include <qwidget>

class QSpdLogFilterWidget : public QWidget {
  Q_OBJECT

public:
  QSpdLogFilterWidget(QWidget *parent = nullptr);
  ~QSpdLogFilterWidget();

signals:
  void filterChanged(const QString &filter);

private:
  QWidget *_filterWidget;
};
