#ifndef USERPAGE_H
#define USERPAGE_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include "userlistview.h"

class UserPage : public QWidget
{
    Q_OBJECT
public:
    explicit UserPage(QWidget *parent = nullptr);
    void setUserList(const QList<UserInfo> &userList);
signals:

public slots:

private:
    UserListView *userList;
    QToolButton *btNewUser;
    QLabel *labUser;

};
#endif // USERPAGE_H
