#include "qspdlog_filter_widget.hpp"

#include <qlayout>
#include <qlineedit>

QSpdLogFilterWidget::QSpdLogFilterWidget(QWidget *parent)
    : QWidget(parent), _filterWidget(new QLineEdit) {
  setLayout(new QHBoxLayout);
  layout()->addWidget(_filterWidget);

  auto lineEdit = static_cast<QLineEdit *>(_filterWidget);

  lineEdit->setPlaceholderText("Filter");
  connect(lineEdit, &QLineEdit::textChanged, this,
          &QSpdLogFilterWidget::filterChanged);
}

QSpdLogFilterWidget::~QSpdLogFilterWidget() {}

QSpdLogFilterWidget::FilteringSettings
QSpdLogFilterWidget::filteringSettings() const {
  return {static_cast<QLineEdit *>(_filterWidget)->text(), false, false};
}
