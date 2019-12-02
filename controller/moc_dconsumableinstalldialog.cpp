/****************************************************************************
** Meta object code from reading C++ file 'dconsumableinstalldialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dconsumableinstalldialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dconsumableinstalldialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DConsumableInstallDialog[] = {

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
      26,   25,   25,   25, 0x05,
      54,   47,   25,   25, 0x05,
      81,   75,   25,   25, 0x05,

 // slots: signature, parameters, type, tag, flags
     108,   25,   25,   25, 0x09,
     130,   25,   25,   25, 0x09,
     160,  154,   25,   25, 0x09,
     215,  197,   25,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DConsumableInstallDialog[] = {
    "DConsumableInstallDialog\0\0"
    "installConusumable()\0isBusy\0"
    "setCheckStatus(bool)\0iType\0"
    "consumableTypeChanged(int)\0"
    "on_closeBtn_clicked()\0on_installBtn_clicked()\0"
    "index\0on_comboBox_currentIndexChanged(int)\0"
    "iType,catNo,lotNo\0"
    "setConsumableName(int,QString,QString)\0"
};

void DConsumableInstallDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DConsumableInstallDialog *_t = static_cast<DConsumableInstallDialog *>(_o);
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

const QMetaObjectExtraData DConsumableInstallDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DConsumableInstallDialog::staticMetaObject = {
    { &DBaseWindow::staticMetaObject, qt_meta_stringdata_DConsumableInstallDialog,
      qt_meta_data_DConsumableInstallDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DConsumableInstallDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DConsumableInstallDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DConsumableInstallDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DConsumableInstallDialog))
        return static_cast<void*>(const_cast< DConsumableInstallDialog*>(this));
    return DBaseWindow::qt_metacast(_clname);
}

int DConsumableInstallDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DBaseWindow::qt_metacall(_c, _id, _a);
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
void DConsumableInstallDialog::installConusumable()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DConsumableInstallDialog::setCheckStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DConsumableInstallDialog::consumableTypeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
