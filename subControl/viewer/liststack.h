#ifndef LISTSTACK_H
#define LISTSTACK_H

#include <QStackedWidget>
#include "mailpage.h"
#include "dirpage.h"
#include "userpage.h"

class ListStack : public QStackedWidget
{
    Q_OBJECT
public:
    explicit ListStack(QWidget *parent = 0);
    void setMailList(const QList<MailInfo> &mailList);
    void setDirList(const QList<DirInfo> &dirList);
    void setUserList(const QList<UserInfo> &userList);
signals:
    void mailClick(int i);
    void dirClick(QString dir);
private:
    MailPage *mailPage;
    DirPage *dirPage;
    UserPage *userPage;
};

#endif // LISTSTACK_H
