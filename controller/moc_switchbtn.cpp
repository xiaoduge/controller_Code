/****************************************************************************
** Meta object code from reading C++ file 'switchbtn.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "switchbtn.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'switchbtn.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SwitchButton[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   13,   13,   13, 0x08,
      72,   64,   13,   13, 0x0a,
     101,   89,   13,   13, 0x0a,
     150,  129,   13,   13, 0x0a,
     205,  176,   13,   13, 0x0a,
     260,  235,   13,   13, 0x0a,
     303,  288,   13,   13, 0x0a,
     345,  328,   13,   13, 0x0a,
     377,  371,   13,   13, 0x0a,
     402,  391,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SwitchButton[] = {
    "SwitchButton\0\0checked,id\0"
    "checkedChanged(bool,int)\0updateValue()\0"
    "checked\0setChecked(bool)\0buttonStyle\0"
    "setButtonStyle(ButtonStyle)\0"
    "bgColorOff,bgColorOn\0setBgColor(QColor,QColor)\0"
    "sliderColorOff,sliderColorOn\0"
    "setSliderColor(QColor,QColor)\0"
    "textColorOff,textColorOn\0"
    "setTextColor(QColor,QColor)\0textOff,textOn\0"
    "setText(QString,QString)\0imageOff,imageOn\0"
    "setImage(QString,QString)\0space\0"
    "setSpace(int)\0rectRadius\0setRectRadius(int)\0"
};

void SwitchButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SwitchButton *_t = static_cast<SwitchButton *>(_o);
        switch (_id) {
        case 0: _t->checkedChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->updateValue(); break;
        case 2: _t->setChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setButtonStyle((*reinterpret_cast< ButtonStyle(*)>(_a[1]))); break;
        case 4: _t->setBgColor((*reinterpret_cast< QColor(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2]))); break;
        case 5: _t->setSliderColor((*reinterpret_cast< QColor(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2]))); break;
        case 6: _t->setTextColor((*reinterpret_cast< QColor(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2]))); break;
        case 7: _t->setText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 8: _t->setImage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: _t->setSpace((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setRectRadius((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SwitchButton::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SwitchButton::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SwitchButton,
      qt_meta_data_SwitchButton, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SwitchButton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SwitchButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SwitchButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SwitchButton))
        return static_cast<void*>(const_cast< SwitchButton*>(this));
    return QWidget::qt_metacast(_clname);
}

int SwitchButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void SwitchButton::checkedChanged(bool _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
