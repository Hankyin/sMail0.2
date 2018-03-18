#include "userpage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

UserPage::UserPage(QWidget *parent) : QWidget(parent)
{
    userList = new UserListView;
    btNewUser = new QToolButton;
    labUser = new QLabel;

    labUser->setText("用户");

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(labUser);
    hLayout->addWidget(btNewUser);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout);
    vLayout->addWidget(userList);
    setLayout(vLayout);
}

void UserPage::setUserList(const QList<UserInfo> &userList)
{
    this->userList->setUserList(userList);
}
