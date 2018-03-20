#include "sidebar.h"
#include <QVBoxLayout>
#include <QAction>

SideBar::SideBar(QWidget *parent) : QWidget(parent)
{
    btWriteMail = new SideButton(":/icon/image/writeMail.png");
    btSelectUser = new SideButton(":/icon/image/user.png");
    btSelectDir = new SideButton(":/icon/image/folder.png");
    btCalendar = new SideButton(":/icon/image/calendar.png");
    btContact = new SideButton(":/icon/image/contact.png");
    btSetting = new SideButton(":/icon/image/setting.png");

    userMenu = new QMenu;
    dirMenu = new QMenu;

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setMargin(1);
    vLayout->addWidget(btWriteMail);
    vLayout->addWidget(btSelectUser);
    vLayout->addWidget(btSelectDir);
    vLayout->addStretch();
    vLayout->addWidget(btCalendar);
    vLayout->addWidget(btContact);
    vLayout->addWidget(btSetting);

    connect(btWriteMail,SIGNAL(clicked(bool)),this,SIGNAL(writeMailClick()));
    connect(btCalendar,SIGNAL(clicked(bool)),this,SIGNAL(calenderClick()));
    connect(btContact,SIGNAL(clicked(bool)),this,SIGNAL(contactClick()));
    connect(btSetting,SIGNAL(clicked(bool)),this,SIGNAL(settingClick()));
}

void SideBar::setUserList(const QList<UserInfo> &userList)
{
    userMenu->clear();
    for(auto i : userList)
    {
        QAction *act = new QAction(userMenu);
        act->setText(i.mail);
        userMenu->addAction(act);
        connect(act,SIGNAL(triggered(bool)),this,SLOT(onUserClick()));
    }
    btSelectUser->setMenu(userMenu);
    btSelectUser->setPopupMode(QToolButton::InstantPopup);
}


void SideBar::onUserClick()
{
    QString mail;
    mail = qobject_cast<QAction*>(QObject::sender())->text();
    emit selectUser(mail);
}

void SideBar::setDirList(const QList<DirInfo> &dirList)
{
    dirMenu->clear();
    for(auto i : dirList)
    {
        QAction *act = new QAction(dirMenu);
        act->setText(i.dirName);
        dirMenu->addAction(act);
        connect(act,SIGNAL(triggered(bool)),this,SLOT(onDirClick()));
    }
    btSelectDir->setMenu(dirMenu);
    btSelectDir->setPopupMode(QToolButton::InstantPopup);
}

void SideBar::onDirClick()
{
    QString dir;
    dir = qobject_cast<QAction*>(QObject::sender())->text();
    emit selectDir(dir);
}
