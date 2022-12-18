#pragma once

#include <qwidget>

class QSpdLogFilterWidget : public QWidget {
  Q_OBJECT

public:
  struct FilteringSettings {
    QString text;
    bool isRegularExpression;
    bool isCaseSensitive;
  };

public:
  QSpdLogFilterWidget(QWidget *parent = nullptr);
  ~QSpdLogFilterWidget();

  FilteringSettings filteringSettings() const;

signals:
  void filterChanged();

private:
  QWidget *_filterWidget;
};
