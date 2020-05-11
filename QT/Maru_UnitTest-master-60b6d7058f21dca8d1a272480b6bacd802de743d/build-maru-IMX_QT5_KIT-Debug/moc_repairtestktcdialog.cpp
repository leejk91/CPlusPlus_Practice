/****************************************************************************
** Meta object code from reading C++ file 'repairtestktcdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../maru/repairtestktcdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'repairtestktcdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RepairTestKTCDialog_t {
    QByteArrayData data[10];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RepairTestKTCDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RepairTestKTCDialog_t qt_meta_stringdata_RepairTestKTCDialog = {
    {
QT_MOC_LITERAL(0, 0, 19), // "RepairTestKTCDialog"
QT_MOC_LITERAL(1, 20, 25), // "RepairTestKTCDialogClosed"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 23), // "on_pushButton_3_clicked"
QT_MOC_LITERAL(4, 71, 23), // "on_debugMessageReceived"
QT_MOC_LITERAL(5, 95, 3), // "str"
QT_MOC_LITERAL(6, 99, 5), // "color"
QT_MOC_LITERAL(7, 105, 25), // "controllerMessageReceived"
QT_MOC_LITERAL(8, 131, 7), // "message"
QT_MOC_LITERAL(9, 139, 21) // "on_pushButton_clicked"

    },
    "RepairTestKTCDialog\0RepairTestKTCDialogClosed\0"
    "\0on_pushButton_3_clicked\0"
    "on_debugMessageReceived\0str\0color\0"
    "controllerMessageReceived\0message\0"
    "on_pushButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RepairTestKTCDialog[] = {

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
       4,    2,   41,    2, 0x08 /* Private */,
       7,    1,   46,    2, 0x08 /* Private */,
       9,    0,   49,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::QByteArray,    8,
    QMetaType::Void,

       0        // eod
};

void RepairTestKTCDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RepairTestKTCDialog *_t = static_cast<RepairTestKTCDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RepairTestKTCDialogClosed(); break;
        case 1: _t->on_pushButton_3_clicked(); break;
        case 2: _t->on_debugMessageReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->controllerMessageReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->on_pushButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RepairTestKTCDialog::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RepairTestKTCDialog::RepairTestKTCDialogClosed)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject RepairTestKTCDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_RepairTestKTCDialog.data,
      qt_meta_data_RepairTestKTCDialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RepairTestKTCDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RepairTestKTCDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RepairTestKTCDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int RepairTestKTCDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void RepairTestKTCDialog::RepairTestKTCDialogClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
