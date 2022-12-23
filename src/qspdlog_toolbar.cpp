#include "qspdlog_toolbar.hpp"

#include <qcombobox>
#include <qlayout>
#include <qlineedit>
#include <qregularexpression>

QSpdLogToolBar::QSpdLogToolBar(QWidget *parent)
    : QToolBar(parent), _filterWidget(new QLineEdit) {
  addWidget(_filterWidget);

  _caseAction = addAction("Aa");
  _caseAction->setCheckable(true);

  _regexAction = addAction(".*");
  _regexAction->setCheckable(true);

  QComboBox *autoScrollPolicySelection = new QComboBox();
  autoScrollPolicySelection->addItems(
      {"Manual Scroll", "Scroll To Bottom", "Smart Scroll"});
  addWidget(autoScrollPolicySelection);

  auto lineEdit = static_cast<QLineEdit *>(_filterWidget);

  lineEdit->setPlaceholderText("Filter");

  connect(lineEdit, &QLineEdit::textChanged, this,
          &QSpdLogToolBar::filterChanged);
  connect(_caseAction, &QAction::toggled, this, &QSpdLogToolBar::filterChanged);
  connect(_regexAction, &QAction::toggled, this,
          &QSpdLogToolBar::filterChanged);
  connect(autoScrollPolicySelection,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &QSpdLogToolBar::autoScrollPolicyChanged);
  connect(this, &QSpdLogToolBar::filterChanged, this,
          &QSpdLogToolBar::checkInputValidity);
}

QSpdLogToolBar::~QSpdLogToolBar() {}

QSpdLogToolBar::FilteringSettings QSpdLogToolBar::filteringSettings() const {
  return {static_cast<QLineEdit *>(_filterWidget)->text(),
          _regexAction->isChecked(), _caseAction->isChecked()};
}

void QSpdLogToolBar::checkInputValidity() {
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
