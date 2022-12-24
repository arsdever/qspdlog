#pragma once

#include <qtoolbar>

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
        QString text;
        bool isRegularExpression;
        bool isCaseSensitive;
    };

    enum AutoScrollPolicy {
        AutoScrollPolicyDisabled = 0,
        AutoScrollPolicyEnabled = 1,
        AutoScrollPolicyEnabledIfBottom = 2
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
    QAbstractItemModel* _completerData;
    QCompleter* _completer;
};
