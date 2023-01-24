/****************************************************************************
** Meta object code from reading C++ file 'qspdlog_toolbar.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../src/qspdlog_toolbar.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qspdlog_toolbar.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QSpdLogToolBar_t {
    const uint offsetsAndSize[10];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_QSpdLogToolBar_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_QSpdLogToolBar_t qt_meta_stringdata_QSpdLogToolBar = {
    {
QT_MOC_LITERAL(0, 14), // "QSpdLogToolBar"
QT_MOC_LITERAL(15, 13), // "filterChanged"
QT_MOC_LITERAL(29, 0), // ""
QT_MOC_LITERAL(30, 23), // "autoScrollPolicyChanged"
QT_MOC_LITERAL(54, 5) // "index"

    },
    "QSpdLogToolBar\0filterChanged\0\0"
    "autoScrollPolicyChanged\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QSpdLogToolBar[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    2, 0x06,    1 /* Public */,
       3,    1,   27,    2, 0x06,    2 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,

       0        // eod
};

void QSpdLogToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QSpdLogToolBar *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->filterChanged(); break;
        case 1: _t->autoScrollPolicyChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QSpdLogToolBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QSpdLogToolBar::filterChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QSpdLogToolBar::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QSpdLogToolBar::autoScrollPolicyChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject QSpdLogToolBar::staticMetaObject = { {
    QMetaObject::SuperData::link<QToolBar::staticMetaObject>(),
    qt_meta_stringdata_QSpdLogToolBar.offsetsAndSize,
    qt_meta_data_QSpdLogToolBar,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QSpdLogToolBar_t
, QtPrivate::TypeAndForceComplete<QSpdLogToolBar, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>



>,
    nullptr
} };


const QMetaObject *QSpdLogToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QSpdLogToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QSpdLogToolBar.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QAbstractSpdLogToolBar"))
        return static_cast< QAbstractSpdLogToolBar*>(this);
    return QToolBar::qt_metacast(_clname);
}

int QSpdLogToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QSpdLogToolBar::filterChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QSpdLogToolBar::autoScrollPolicyChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
