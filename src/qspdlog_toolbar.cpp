#include <QComboBox>
#include <QCompleter>
#include <QLayout>
#include <QLineEdit>
#include <QRegularExpression>
#include <QSettings>
#include <QStringListModel>

#include "qspdlog_toolbar.hpp"

QSpdLogToolBar::QSpdLogToolBar(QWidget* parent)
    : QToolBar(parent)
    , _filterWidget(new QLineEdit(this))
    , _completerData(new QStringListModel(this))
    , _completer(new QCompleter(_completerData, this))
{
    addWidget(_filterWidget);
    _filterWidget->setObjectName("filterText");

    _caseAction = addAction("Aa");
    _caseAction->setCheckable(true);
    _caseAction->setObjectName("caseSensitiveAction");

    _regexAction = addAction(".*");
    _regexAction->setCheckable(true);
    _regexAction->setObjectName("regexAction");

    QComboBox* autoScrollPolicySelection = new QComboBox();
    autoScrollPolicySelection->setObjectName("autoScrollPolicySelection");
    autoScrollPolicySelection->addItems(
        { "Manual Scroll", "Scroll To Bottom", "Smart Scroll" }
    );
    addWidget(autoScrollPolicySelection);

    auto lineEdit = static_cast<QLineEdit*>(_filterWidget);

    lineEdit->setPlaceholderText("Filter");

    _completer->setCaseSensitivity(Qt::CaseInsensitive);
    _completer->setCompletionMode(QCompleter::PopupCompletion);
    lineEdit->setCompleter(_completer);

    connect(
        lineEdit, &QLineEdit::textChanged, this, &QSpdLogToolBar::filterChanged
    );
    connect(lineEdit, &QLineEdit::editingFinished, this, [ this ]() {
        QStringListModel* model =
            static_cast<QStringListModel*>(_completerData);
        QString text = static_cast<QLineEdit*>(_filterWidget)->text();
        if (text.isEmpty() || model->stringList().contains(text))
            return;

        if (model->insertRow(model->rowCount())) {
            QModelIndex index = model->index(model->rowCount() - 1, 0);
            model->setData(index, text);
        }
        saveCompleterHistory();
    });
    connect(
        _caseAction, &QAction::toggled, this, &QSpdLogToolBar::filterChanged
    );
    connect(
        _regexAction, &QAction::toggled, this, &QSpdLogToolBar::filterChanged
    );
    connect(
        autoScrollPolicySelection,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        &QSpdLogToolBar::autoScrollPolicyChanged
    );
    connect(
        this,
        &QSpdLogToolBar::filterChanged,
        this,
        &QSpdLogToolBar::checkInputValidity
    );
    loadCompleterHistory();
}

QSpdLogToolBar::~QSpdLogToolBar() { }

QSpdLogToolBar::FilteringSettings QSpdLogToolBar::filteringSettings() const
{
    return { static_cast<QLineEdit*>(_filterWidget)->text(),
             _regexAction->isChecked(),
             _caseAction->isChecked() };
}

void QSpdLogToolBar::checkInputValidity()
{
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

void QSpdLogToolBar::clearCompleterHistory()
{
    QStringListModel* model = static_cast<QStringListModel*>(_completerData);
    model->setStringList({});
    saveCompleterHistory();
}

void QSpdLogToolBar::loadCompleterHistory()
{
    QStringListModel* model = static_cast<QStringListModel*>(_completerData);
    model->setStringList(QSettings().value("completerHistory").toStringList());
}

void QSpdLogToolBar::saveCompleterHistory()
{
    QStringListModel* model = static_cast<QStringListModel*>(_completerData);
    QSettings().setValue("completerHistory", model->stringList());
}
