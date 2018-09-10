/****************************************************************************
** Meta object code from reading C++ file 'parametercalibration.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "parametercalibration.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'parametercalibration.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ParameterCalibrationPage[] = {

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
      30,   26,   25,   25, 0x0a,
      55,   50,   25,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ParameterCalibrationPage[] = {
    "ParameterCalibrationPage\0\0tmp\0"
    "on_btn_clicked(int)\0item\0"
    "ItemClicked(QListWidgetItem*)\0"
};

void ParameterCalibrationPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ParameterCalibrationPage *_t = static_cast<ParameterCalibrationPage *>(_o);
        switch (_id) {
        case 0: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->ItemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ParameterCalibrationPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ParameterCalibrationPage::staticMetaObject = {
    { &CSubPage::staticMetaObject, qt_meta_stringdata_ParameterCalibrationPage,
      qt_meta_data_ParameterCalibrationPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ParameterCalibrationPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ParameterCalibrationPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ParameterCalibrationPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ParameterCalibrationPage))
        return static_cast<void*>(const_cast< ParameterCalibrationPage*>(this));
    return CSubPage::qt_metacast(_clname);
}

int ParameterCalibrationPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
