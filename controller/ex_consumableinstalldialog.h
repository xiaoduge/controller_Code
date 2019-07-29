#ifndef EX_CONSUMABLEINSTALLDIALOG_H
#define EX_CONSUMABLEINSTALLDIALOG_H

#include <QDialog>
#include "ex_basewindow.h"

class QComboBox;
class DLineEdit;
class QLabel;

class Ex_ConsumableInstallDialog : public Ex_BaseWindow
{
    Q_OBJECT
public:
    explicit Ex_ConsumableInstallDialog(int id, QWidget * parent = 0);

    void switchLanguage();
    void buildTranslation();

    void updatePage();
    
signals:
    void installConusumable();
    void setCheckStatus(bool isBusy);

    void consumableTypeChanged(int iType);

protected:
    void closeEvent(QCloseEvent *);

protected slots:
    void on_closeBtn_clicked();
    void on_installBtn_clicked();
    void on_comboBox_currentIndexChanged(int index);
    
public slots:
    void setConsumableName(int iType, const QString& catNo, const QString& lotNo);

private:
    void initUI();
    bool checkUserInfo(const QString& userName);

private:
    int m_instanceID;

    int m_iType;

    QComboBox *m_pcomboBox;
    QLabel *m_pCatLabel;
    QLabel *m_pLotLabel;
    QLabel *m_pUserLabel;
    DLineEdit *m_pLineEditCat;
    DLineEdit *m_pLineEditLot;
    DLineEdit *m_pLineEditUser;
    QPushButton *m_pInstallBtn;

};

#endif // EX_CONSUMABLEINSTALLDIALOG_H
