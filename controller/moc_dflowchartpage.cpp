/****************************************************************************
** Meta object code from reading C++ file 'dflowchartpage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dflowchartpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dflowchartpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DFlowChartPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   16,   15,   15, 0x0a,
      41,   15,   15,   15, 0x0a,
      65,   55,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DFlowChartPage[] = {
    "DFlowChartPage\0\0tmp\0on_navi_clicked(int)\0"
    "updateUnits()\0msg,isAdd\0"
    "on_updateAlarmMsg(QString,bool)\0"
};

void DFlowChartPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DFlowChartPage *_t = static_cast<DFlowChartPage *>(_o);
        switch (_id) {
        case 0: _t->on_navi_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->updateUnits(); break;
        case 2: _t->on_updateAlarmMsg((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DFlowChartPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DFlowChartPage::staticMetaObject = {
    { &CPage::staticMetaObject, qt_meta_stringdata_DFlowChartPage,
      qt_meta_data_DFlowChartPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DFlowChartPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DFlowChartPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DFlowChartPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DFlowChartPage))
        return static_cast<void*>(const_cast< DFlowChartPage*>(this));
    return CPage::qt_metacast(_clname);
}

int DFlowChartPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
