/****************************************************************************
** Meta object code from reading C++ file 'syszuxpinyin.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "syszuxpinyin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'syszuxpinyin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SyszuxPingyinCtlInfo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_SyszuxPingyinCtlInfo[] = {
    "SyszuxPingyinCtlInfo\0"
};

void SyszuxPingyinCtlInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData SyszuxPingyinCtlInfo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SyszuxPingyinCtlInfo::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SyszuxPingyinCtlInfo,
      qt_meta_data_SyszuxPingyinCtlInfo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SyszuxPingyinCtlInfo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SyszuxPingyinCtlInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SyszuxPingyinCtlInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SyszuxPingyinCtlInfo))
        return static_cast<void*>(const_cast< SyszuxPingyinCtlInfo*>(this));
    return QObject::qt_metacast(_clname);
}

int SyszuxPingyinCtlInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_SyszuxPinyin[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   14,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SyszuxPinyin[] = {
    "SyszuxPinyin\0\0gemfield\0sendPinyin(QString)\0"
    "buttonClickResponse(int)\0"
};

void SyszuxPinyin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SyszuxPinyin *_t = static_cast<SyszuxPinyin *>(_o);
        switch (_id) {
        case 0: _t->sendPinyin((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->buttonClickResponse((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SyszuxPinyin::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SyszuxPinyin::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SyszuxPinyin,
      qt_meta_data_SyszuxPinyin, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SyszuxPinyin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SyszuxPinyin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SyszuxPinyin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SyszuxPinyin))
        return static_cast<void*>(const_cast< SyszuxPinyin*>(this));
    if (!strcmp(_clname, "Ui::IMEall"))
        return static_cast< Ui::IMEall*>(const_cast< SyszuxPinyin*>(this));
    return QDialog::qt_metacast(_clname);
}

int SyszuxPinyin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SyszuxPinyin::sendPinyin(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
