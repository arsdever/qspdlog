#include "qspdlog_filter_widget.hpp"

#include <qlayout>
#include <qlineedit>
#include <qregularexpression>

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

  connect(this, &QSpdLogFilterWidget::filterChanged, this,
          &QSpdLogFilterWidget::checkInputValidity);
}

QSpdLogFilterWidget::~QSpdLogFilterWidget() {}

QSpdLogFilterWidget::FilteringSettings
QSpdLogFilterWidget::filteringSettings() const {
  return {static_cast<QLineEdit *>(_filterWidget)->text(),
          _regexAction->isChecked(), _caseAction->isChecked()};
}

void QSpdLogFilterWidget::checkInputValidity() {
  FilteringSettings settings = filteringSettings();

  if (!settings.isRegularExpression) {
    // everything is ok, the input text is valid
    _filterWidget->setPalette(QWidget::palette());
    _filterWidget->setToolTip("");
    return;
  }

  QRegularExpression regex(settings.text);
  if (regex.isValid()) {
    _filterWidget->setPalette(QWidget::palette());
    _filterWidget->setToolTip("");
    return;
  }

  QPalette palette = _filterWidget->palette();
  palette.setColor(QPalette::Text, Qt::red);
  _filterWidget->setPalette(palette);
  _filterWidget->setToolTip(regex.errorString());
}