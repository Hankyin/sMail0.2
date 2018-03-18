#include "liststack.h"

ListStack::ListStack(QWidget *parent) : QStackedWidget(parent)
{
    mailPage = new MailPage(this);
    dirPage = new DirPage(this);
    userPage = new UserPage(this);

    connect(mailPage,SIGNAL(itemClick(int)),this,SIGNAL(mailClick(int)));
    connect(dirPage,SIGNAL(itemClick(QString)),this,SIGNAL(dirClick(QString)));

    addWidget(mailPage);
    addWidget(dirPage);
    addWidget(userPage);

    setCurrentIndex(0);
}

void ListStack::setMailList(const QList<MailInfo> &mailList)
{
    mailPage->setMailList(mailList);
}

void ListStack::setDirList(const QList<DirInfo> &dirList)
{
    dirPage->setDirList(dirList);
}

void ListStack::setUserList(const QList<UserInfo> &userList)
{
    userPage->setUserList(userList);
}
