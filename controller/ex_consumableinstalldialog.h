#ifndef EX_CONSUMABLEINSTALLDIALOG_H
#define EX_CONSUMABLEINSTALLDIALOG_H

#include <QDialog>

class QComboBox;
class QLineEdit;
class QLabel;

class Ex_ConsumableInstallDialog : public QDialog
{
    Q_OBJECT
public:
    explicit Ex_ConsumableInstallDialog(int id, QWidget * parent = 0, Qt::WindowFlags f = 0);

    void switchLanguage();
    void buildTranslation();
    
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
    QLineEdit *m_pLineEditCat;
    QLineEdit *m_pLineEditLot;
    QLineEdit *m_pLineEditUser;
    QPushButton *m_pInstallBtn;
    QPushButton *m_pCloseBtn;
};

#endif // EX_CONSUMABLEINSTALLDIALOG_H
