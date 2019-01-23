/****************************************************************************
** Meta object code from reading C++ file 'ex_consumableinstalldialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ex_consumableinstalldialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ex_consumableinstalldialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ex_ConsumableInstallDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   27,   27,   27, 0x05,
      56,   49,   27,   27, 0x05,
      83,   77,   27,   27, 0x05,

 // slots: signature, parameters, type, tag, flags
     110,   27,   27,   27, 0x09,
     132,   27,   27,   27, 0x09,
     162,  156,   27,   27, 0x09,
     217,  199,   27,   27, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Ex_ConsumableInstallDialog[] = {
    "Ex_ConsumableInstallDialog\0\0"
    "installConusumable()\0isBusy\0"
    "setCheckStatus(bool)\0iType\0"
    "consumableTypeChanged(int)\0"
    "on_closeBtn_clicked()\0on_installBtn_clicked()\0"
    "index\0on_comboBox_currentIndexChanged(int)\0"
    "iType,catNo,lotNo\0"
    "setConsumableName(int,QString,QString)\0"
};

void Ex_ConsumableInstallDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Ex_ConsumableInstallDialog *_t = static_cast<Ex_ConsumableInstallDialog *>(_o);
        switch (_id) {
        case 0: _t->installConusumable(); break;
        case 1: _t->setCheckStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->consumableTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_closeBtn_clicked(); break;
        case 4: _t->on_installBtn_clicked(); break;
        case 5: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setConsumableName((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Ex_ConsumableInstallDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Ex_ConsumableInstallDialog::staticMetaObject = {
    { &Ex_BaseWindow::staticMetaObject, qt_meta_stringdata_Ex_ConsumableInstallDialog,
      qt_meta_data_Ex_ConsumableInstallDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ex_ConsumableInstallDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ex_ConsumableInstallDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ex_ConsumableInstallDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ex_ConsumableInstallDialog))
        return static_cast<void*>(const_cast< Ex_ConsumableInstallDialog*>(this));
    return Ex_BaseWindow::qt_metacast(_clname);
}

int Ex_ConsumableInstallDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Ex_BaseWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Ex_ConsumableInstallDialog::installConusumable()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Ex_ConsumableInstallDialog::setCheckStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Ex_ConsumableInstallDialog::consumableTypeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
