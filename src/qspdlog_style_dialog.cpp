#include <QLineEdit>
#include <QBoxLayout>
#include <QDialogButtonBox>

#include "qspdlog_style_dialog.hpp"

QSpdLogStyleDialog::QSpdLogStyleDialog(QWidget* parent)
    : QDialog(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLineEdit* loggerNameEdit = new QLineEdit();
    loggerNameEdit->setPlaceholderText("Logger name");
    QLineEdit* backgroundColorEdit = new QLineEdit();
    backgroundColorEdit->setPlaceholderText("Background color");
    QLineEdit* textColorEdit = new QLineEdit();
    textColorEdit->setPlaceholderText("Text color");

    layout->addWidget(loggerNameEdit);
    layout->addWidget(backgroundColorEdit);
    layout->addWidget(textColorEdit);

    QDialogButtonBox* buttonBox =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(
        buttonBox,
        &QDialogButtonBox::accepted,
        this,
        [ this, loggerNameEdit, backgroundColorEdit, textColorEdit ]() {
        if (!loggerNameEdit->text().isEmpty())
            reject();

        _result.loggerName = loggerNameEdit->text().toStdString();

        if (!backgroundColorEdit->text().isEmpty())
            _result.backgroundColor = QColor(backgroundColorEdit->text());
        else
            _result.backgroundColor = std::nullopt;

        if (!textColorEdit->text().isEmpty())
            _result.textColor = QColor(textColorEdit->text());
        else
            _result.textColor = std::nullopt;

        accept();
        });
    connect(buttonBox, &QDialogButtonBox::rejected, this, [ this ]() {
        reject();
    });
}

QSpdLogStyleDialog::~QSpdLogStyleDialog() = default;

std::optional<QSpdLogStyleDialog::Style> QSpdLogStyleDialog::getLoggerStyle()
{
    QSpdLogStyleDialog dialog;
    if (dialog.exec())
        return dialog._result;

    return {};
}
