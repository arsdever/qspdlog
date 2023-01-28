#pragma once

#include <QDialog>
#include <optional>

class QSpdLogStyleDialog : public QDialog
{
    Q_OBJECT

public:
    struct Style {
        std::string loggerName;
        std::optional<QColor> backgroundColor;
        std::optional<QColor> textColor;
    };

public:
    explicit QSpdLogStyleDialog(QWidget* parent = nullptr);
    ~QSpdLogStyleDialog() override;

    static std::optional<Style> getLoggerStyle();

private:
    Style _result;
};
