/****************************************************************************
** Meta object code from reading C++ file 'soundpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "soundpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'soundpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SoundPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   11,   10,   10, 0x0a,
      50,   46,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SoundPage[] = {
    "SoundPage\0\0state\0on_checkBox_changeState(int)\0"
    "tmp\0on_btn_clicked(int)\0"
};

void SoundPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SoundPage *_t = static_cast<SoundPage *>(_o);
        switch (_id) {
        case 0: _t->on_checkBox_changeState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SoundPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SoundPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_SoundPage,
      qt_meta_data_SoundPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SoundPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SoundPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SoundPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SoundPage))
        return static_cast<void*>(const_cast< SoundPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int SoundPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CSubPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
