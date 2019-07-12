/****************************************************************************
** Meta object code from reading C++ file 'dconsumableinstallwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dconsumableinstallwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dconsumableinstallwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DConsumableInstallWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      31,   26,   25,   25, 0x0a,
      54,   47,   25,   25, 0x0a,
      80,   77,   25,   25, 0x0a,
      95,   25,   91,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DConsumableInstallWidget[] = {
    "DConsumableInstallWidget\0\0tags\0"
    "setTag(QString)\0status\0setInstallStatus(bool)\0"
    "id\0setID(int)\0int\0id()\0"
};

void DConsumableInstallWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DConsumableInstallWidget *_t = static_cast<DConsumableInstallWidget *>(_o);
        switch (_id) {
        case 0: _t->setTag((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->setInstallStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setID((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: { int _r = _t->id();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DConsumableInstallWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DConsumableInstallWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DConsumableInstallWidget,
      qt_meta_data_DConsumableInstallWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DConsumableInstallWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DConsumableInstallWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DConsumableInstallWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DConsumableInstallWidget))
        return static_cast<void*>(const_cast< DConsumableInstallWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int DConsumableInstallWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
