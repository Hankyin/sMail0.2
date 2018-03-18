#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QList>
#include "sidebutton.h"

class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);
    void addTopButton();
signals:
    void writeMailClick();
    void selectDirClick();
    void selectUserClick();
    void calenderClick();
    void contactClick();
    void settingClick();
public slots:

private:
    SideButton *btWriteMail;
    SideButton *btSelectDir;
    SideButton *btSelectUser;

    SideButton *btCalendar;
    SideButton *btContact;
    SideButton *btSetting;
};

#endif // SIDEBAR_H
