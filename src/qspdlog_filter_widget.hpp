#pragma once

#include <QToolBar>

class QSpdLogFilterWidget : public QToolBar {
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
  void checkInputValidity();

signals:
  void filterChanged();

private:
  QWidget *_filterWidget;
  QAction *_caseAction;
  QAction *_regexAction;
};
