#pragma once

#include <QToolBar>

class QWidget;
class QAction;
class QCompleter;
class QAbstractItemModel;
class QSettings;

class QSpdLogToolBar : public QToolBar
{
    Q_OBJECT

public:
    struct FilteringSettings {
        QString text;             // The text to filter by.
        bool isRegularExpression; // Whether the text is a regular expression.
        bool isCaseSensitive;     // Whether the filtering is case sensitive.
    };

public:
    QSpdLogToolBar(QWidget* parent = nullptr);
    ~QSpdLogToolBar();

    FilteringSettings filteringSettings() const;
    void checkInputValidity();
    void clearCompleterHistory();

signals:
    void filterChanged();
    void autoScrollPolicyChanged(int index);

private:
    void loadCompleterHistory();
    void saveCompleterHistory();

private:
    QWidget* _filterWidget;
    QAction* _caseAction;
    QAction* _regexAction;
    QAction* _clearHistory;
    QAbstractItemModel* _completerData;
    QCompleter* _completer;
};
