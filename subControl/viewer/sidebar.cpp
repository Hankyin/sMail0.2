#include "sidebar.h"
#include <QVBoxLayout>

SideBar::SideBar(QWidget *parent) : QWidget(parent)
{
    btWriteMail = new SideButton(":/icon/image/writeMail.png");
    btSelectUser = new SideButton(":/icon/image/user.png");
    btSelectDir = new SideButton(":/icon/image/folder.png");
    btCalendar = new SideButton(":/icon/image/calendar.png");
    btContact = new SideButton(":/icon/image/contact.png");
    btSetting = new SideButton(":/icon/image/setting.png");
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
    connect(btSelectUser,SIGNAL(clicked(bool)),this,SIGNAL(selectUserClick()));
    connect(btSelectDir,SIGNAL(clicked(bool)),this,SIGNAL(selectDirClick()));
    connect(btCalendar,SIGNAL(clicked(bool)),this,SIGNAL(calenderClick()));
    connect(btContact,SIGNAL(clicked(bool)),this,SIGNAL(contactClick()));
    connect(btSetting,SIGNAL(clicked(bool)),this,SIGNAL(settingClick()));
}
