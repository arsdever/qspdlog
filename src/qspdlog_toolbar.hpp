#pragma once

#include <QToolBar>

class QSpdLogToolBar : public QToolBar {
  Q_OBJECT

public:
  struct FilteringSettings {
    QString text;
    bool isRegularExpression;
    bool isCaseSensitive;
  };

public:
  QSpdLogToolBar(QWidget *parent = nullptr);
  ~QSpdLogToolBar();

  FilteringSettings filteringSettings() const;
  void checkInputValidity();

signals:
  void filterChanged();

private:
  QWidget *_filterWidget;
  QAction *_caseAction;
  QAction *_regexAction;
};
