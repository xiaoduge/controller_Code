/********************************************************************************
** Form generated from reading UI file 'drunwarningdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRUNWARNINGDIALOG_H
#define UI_DRUNWARNINGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DRunWarningDialog
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_confirm;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_quit;
    QSpacerItem *horizontalSpacer_5;

    void setupUi(QDialog *DRunWarningDialog)
    {
        if (DRunWarningDialog->objectName().isEmpty())
            DRunWarningDialog->setObjectName(QString::fromUtf8("DRunWarningDialog"));
        DRunWarningDialog->resize(399, 259);
        verticalLayout = new QVBoxLayout(DRunWarningDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 83, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(DRunWarningDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 83, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pushButton_confirm = new QPushButton(DRunWarningDialog);
        pushButton_confirm->setObjectName(QString::fromUtf8("pushButton_confirm"));
        pushButton_confirm->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_confirm->sizePolicy().hasHeightForWidth());
        pushButton_confirm->setSizePolicy(sizePolicy1);
        pushButton_confirm->setMinimumSize(QSize(98, 0));

        horizontalLayout_2->addWidget(pushButton_confirm);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        pushButton_quit = new QPushButton(DRunWarningDialog);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        sizePolicy1.setHeightForWidth(pushButton_quit->sizePolicy().hasHeightForWidth());
        pushButton_quit->setSizePolicy(sizePolicy1);
        pushButton_quit->setMinimumSize(QSize(98, 0));

        horizontalLayout_2->addWidget(pushButton_quit);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(DRunWarningDialog);

        QMetaObject::connectSlotsByName(DRunWarningDialog);
    } // setupUi

    void retranslateUi(QDialog *DRunWarningDialog)
    {
        DRunWarningDialog->setWindowTitle(QApplication::translate("DRunWarningDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DRunWarningDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        pushButton_confirm->setText(QApplication::translate("DRunWarningDialog", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_quit->setText(QApplication::translate("DRunWarningDialog", "PushButton", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DRunWarningDialog: public Ui_DRunWarningDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRUNWARNINGDIALOG_H
