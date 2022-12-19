#include "qspdlog_filter_widget.hpp"

#include <qlayout>
#include <qlineedit>

QSpdLogFilterWidget::QSpdLogFilterWidget(QWidget *parent)
    : QToolBar(parent), _filterWidget(new QLineEdit) {
  addWidget(_filterWidget);

  _caseAction = addAction("Aa");
  _caseAction->setCheckable(true);

  _regexAction = addAction(".*");
  _regexAction->setCheckable(true);

  auto lineEdit = static_cast<QLineEdit *>(_filterWidget);

  lineEdit->setPlaceholderText("Filter");
  connect(lineEdit, &QLineEdit::textChanged, this,
          &QSpdLogFilterWidget::filterChanged);
  connect(_caseAction, &QAction::toggled, this,
          &QSpdLogFilterWidget::filterChanged);
  connect(_regexAction, &QAction::toggled, this,
          &QSpdLogFilterWidget::filterChanged);
}

QSpdLogFilterWidget::~QSpdLogFilterWidget() {}

QSpdLogFilterWidget::FilteringSettings
QSpdLogFilterWidget::filteringSettings() const {
  return {static_cast<QLineEdit *>(_filterWidget)->text(),
          _regexAction->isChecked(), _caseAction->isChecked()};
}
