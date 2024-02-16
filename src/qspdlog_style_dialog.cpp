#include <QBoxLayout>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLineEdit>

#include "qspdlog_style_dialog.hpp"

#include "qspdlog_model.hpp"

QSpdLogStyleDialog::QSpdLogStyleDialog(QWidget* parent)
    : QDialog(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLineEdit* loggerNameEdit = new QLineEdit();
    loggerNameEdit->setPlaceholderText("Logger name");
    loggerNameEdit->setObjectName("loggerNameEdit");
    QLineEdit* backgroundColorEdit = new QLineEdit();
    backgroundColorEdit->setPlaceholderText("Background color");
    backgroundColorEdit->setObjectName("backgroundColorEdit");
    QLineEdit* textColorEdit = new QLineEdit();
    textColorEdit->setPlaceholderText("Text color");
    textColorEdit->setObjectName("textColorEdit");
    QCheckBox* checkBoxBold = new QCheckBox("Bold");
    checkBoxBold->setObjectName("checkBoxBold");

    layout->addWidget(loggerNameEdit);
    layout->addWidget(backgroundColorEdit);
    layout->addWidget(textColorEdit);
    layout->addWidget(checkBoxBold);

    QDialogButtonBox* buttonBox =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);
    buttonBox->setObjectName("buttonBox");

    connect(
        loggerNameEdit,
        &QLineEdit::textChanged,
        this,
        [ this, backgroundColorEdit, textColorEdit, checkBoxBold ](
            const QString& name
        ) {
        std::string namestdstr = name.toStdString();
        auto bg = _model->getLoggerBackground(namestdstr);
        auto fg = _model->getLoggerForeground(namestdstr);
        auto fnt = _model->getLoggerFont(namestdstr);

        if (bg)
            backgroundColorEdit->setText(bg.value().color().name());
        else
            backgroundColorEdit->setText("");

        if (fg)
            textColorEdit->setText(fg.value().name());
        else
            textColorEdit->setText("");

        if (fnt) {
            bool isBold = fnt->bold();
            checkBoxBold->setChecked(isBold);
        } else {
            checkBoxBold->setChecked(false);
        }
        });

    connect(
        buttonBox,
        &QDialogButtonBox::accepted,
        this,
        [ this,
          loggerNameEdit,
          backgroundColorEdit,
          textColorEdit,
          checkBoxBold ]() {
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

        _result.fontBold = checkBoxBold->isChecked();

        accept();
        });

    connect(buttonBox, &QDialogButtonBox::rejected, this, [ this ]() {
        reject();
    });
}

QSpdLogStyleDialog::~QSpdLogStyleDialog() = default;

QSpdLogStyleDialog::Style QSpdLogStyleDialog::result() const { return _result; }

void QSpdLogStyleDialog::setModel(const QSpdLogModel* model) { _model = model; }
