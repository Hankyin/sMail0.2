#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QStackedWidget>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>
#include "utils/type.h"
#include "smail.h"

class sMail;

class AccountManager : public QStackedWidget
{
    Q_OBJECT
public:
    explicit AccountManager();

signals:
    void userInfoReady(UserInfo newUser);
public slots:

private slots:
    void toUserPage();
    void toServerPage();
    void finish();
private:
    QLabel *labSetMail;
    QLineEdit *leName;
    QLineEdit *leMail;
    QLineEdit *lePass;
    QPushButton *btNext;

    QLabel *labSetServer;
    QGroupBox *boxIMAP;
    QLineEdit *leIMAPServer;
    QSpinBox *spIMAPPort;
    QCheckBox *chIMAPSSL;
    QGroupBox *boxSMTP;
    QLineEdit *leSMTPServer;
    QSpinBox *spSMTPPort;
    QCheckBox *chSMTPSSL;
    QPushButton *btPrevious;
    QPushButton *btFinish;

    QWidget *userInfoPage;
    QWidget *serverInfoPage;
    UserInfo user;
};

#endif // ACCOUNTMANAGER_H
