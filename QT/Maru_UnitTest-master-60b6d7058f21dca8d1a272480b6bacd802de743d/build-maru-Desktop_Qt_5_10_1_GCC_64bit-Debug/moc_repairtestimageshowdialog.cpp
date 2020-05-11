/****************************************************************************
** Meta object code from reading C++ file 'repairtestimageshowdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../maru/repairtestimageshowdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'repairtestimageshowdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RepairTestImageShowDialog_t {
    QByteArrayData data[7];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RepairTestImageShowDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RepairTestImageShowDialog_t qt_meta_stringdata_RepairTestImageShowDialog = {
    {
QT_MOC_LITERAL(0, 0, 25), // "RepairTestImageShowDialog"
QT_MOC_LITERAL(1, 26, 31), // "RepairTestImageShowDialogClosed"
QT_MOC_LITERAL(2, 58, 0), // ""
QT_MOC_LITERAL(3, 59, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(4, 83, 10), // "on_timeout"
QT_MOC_LITERAL(5, 94, 11), // "on_stimeout"
QT_MOC_LITERAL(6, 106, 21) // "on_pushButton_clicked"

    },
    "RepairTestImageShowDialog\0"
    "RepairTestImageShowDialogClosed\0\0"
    "on_pushButton_2_clicked\0on_timeout\0"
    "on_stimeout\0on_pushButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RepairTestImageShowDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RepairTestImageShowDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RepairTestImageShowDialog *_t = static_cast<RepairTestImageShowDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RepairTestImageShowDialogClosed(); break;
        case 1: _t->on_pushButton_2_clicked(); break;
        case 2: _t->on_timeout(); break;
        case 3: _t->on_stimeout(); break;
        case 4: _t->on_pushButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RepairTestImageShowDialog::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RepairTestImageShowDialog::RepairTestImageShowDialogClosed)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject RepairTestImageShowDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_RepairTestImageShowDialog.data,
      qt_meta_data_RepairTestImageShowDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RepairTestImageShowDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RepairTestImageShowDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RepairTestImageShowDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int RepairTestImageShowDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void RepairTestImageShowDialog::RepairTestImageShowDialogClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
