#ifndef SMAIL_H
#define SMAIL_H

#include <QObject>
#include <QList>
#include <QString>
#include "mailviewer.h"
#include "accountmanager.h"
#include "database.h"
#include "netProtocol/imap.h"

class AccountManager;

class sMail : public QObject
{
    Q_OBJECT

public:
    sMail(QObject *parent = 0);
    ~sMail();

    bool getUserListFromDB();
    void login();
    void addUser();

private slots:
    void getNewUserInfo(UserInfo newUser);
private:
    AccountManager *accountManager;
    MailViewer *mailViewer;
    DataBase *db;
    UserInfo curUser;
};

#endif // SMAIL_H
