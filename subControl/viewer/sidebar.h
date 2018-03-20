#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QList>
#include <QMenu>
#include "sidebutton.h"
#include "utils/type.h"

class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);
    void setDirList(const QList<DirInfo> &dirList);
    void setUserList(const QList<UserInfo> &userList);
signals:
    void writeMailClick();
    void selectDir(QString dirName);
    void selectUser(QString userMail);
    void calenderClick();
    void contactClick();
    void settingClick();
public slots:

private slots:
    void onDirClick();
    void onUserClick();

private:
    SideButton *btWriteMail;
    SideButton *btSelectDir;
    SideButton *btSelectUser;

    SideButton *btCalendar;
    SideButton *btContact;
    SideButton *btSetting;

    QMenu *userMenu;
    QMenu *dirMenu;
};

#endif // SIDEBAR_H
