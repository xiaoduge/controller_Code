#ifndef DBDIALOG_H
#define DBDIALOG_H

#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QListView>
#include <QCalendarWidget>
#include <QComboBox>
#include <QStringListModel>


namespace Ui {
class mainLayoutWindow;
}

class DbDlg : public QDialog
{
    Q_OBJECT

public:
    DbDlg(QWidget* parent=0);
    ~DbDlg();

private slots:
    void QueryDb(int iIdx = 0);
    void QStart();
    void QEnd();

    void QStaCal();
    void QEndCal();
    void QQuery();

    void DelDb();
    void DStart();
    void DEnd();
    
    void DStaCal();
    void DEndCal();
    void DQuery();
    void DAction();

    
    void Exit();

private:

    QLabel *m_pLabel[2];

    QComboBox *m_pCombBox[2];

    QPushButton *m_pBtnStart[2];
    QPushButton *m_pBtnend[2];

    QLineEdit   *m_pEditsta[2];
    QLineEdit   *m_pEditend[2];

    QPushButton *m_pBtnQuery[2];
    QPushButton *m_pBtnAction[2];

    QPushButton *m_pBtnExit;

    QCalendarWidget *m_pCalSta[2];
    QCalendarWidget *m_pCalEnd[2];

    QListView *m_pDataList[2];
    QStringListModel *m_pListmodel[2];
    QStringList m_strList[2];


    QTabWidget  *m_pTblWidget;

};

#endif // DBDIALOG_H
