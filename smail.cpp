#include "smail.h"
#include "QDebug"

sMail::sMail(QObject *parent)
    : QObject(parent)
{
    db = new DataBase(this);
}

sMail::~sMail()
{

}

bool sMail::getUserListFromDB()
{
    QList<UserInfo> list;
    list = db->getUserList();
    if(list.isEmpty())
        return false;
    curUser = list.at(0);
    return true;
}

void sMail::login()
{
    mailViewer = new MailViewer(this->db);
    mailViewer->show();
}

void sMail::addUser()
{
    accountManager = new AccountManager;
    connect(accountManager,SIGNAL(userInfoReady(UserInfo)),
            this,SLOT(getNewUserInfo(UserInfo)));
    accountManager->show();
}

void sMail::getNewUserInfo(UserInfo newUser)
{
    db->insertNewUser(newUser);
    login();
}


