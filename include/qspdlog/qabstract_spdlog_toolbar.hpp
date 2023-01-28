#pragma once

class QLineEdit;
class QAction;
class QComboBox;
class QSpdLog;

class QAbstractSpdLogToolBar
{
public:
    explicit QAbstractSpdLogToolBar() = default;
    virtual ~QAbstractSpdLogToolBar();

public:
    void setParent(QSpdLog* parent);
    virtual QLineEdit* filter() = 0;
    virtual QAction* caseSensitive() = 0;
    virtual QAction* regex() = 0;
    virtual QAction* clearHistory() = 0;
    virtual QAction* style() = 0;
    virtual QComboBox* autoScrollPolicy() = 0;

private:
    QSpdLog* _parent;
};

extern QAbstractSpdLogToolBar* createToolBar();
