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
    QList<UserInfo> &getUserList(){return userList;}
    AccountManager *getAccountManager() {return accountManager;}
    MailViewer *getMailViewer() {return mailViewer;}

signals:
    void dirListChanged(QList<DirInfo> newDirs);
    void mailListChangedd(QList<MailInfo> newMails);

private slots:
    void IMAPError(QString error);
    void IMAPConnected();
    void IMAPLoginFinish();
    void IMAPListFinish(QList<IMAPDir> dirs);
    void IMAPSelectFinish(QString dir,uint mails,uint recents);
    void IMAPFetchFinsh(int index,QString mail);
    void IMAPSearchFinish(QList<int> searchList);

    void login(UserInfo user);
private:
    AccountManager *accountManager;
    MailViewer *mailViewer;
    DataBase *db;

    UserInfo curUser;
    QList<UserInfo> userList;
    QList<DirInfo> dirList;
    QList<MailInfo> mailList;

    IMAP *imap;
};

#endif // SMAIL_H
