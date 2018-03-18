#ifndef USERLISTVIEW_H
#define USERLISTVIEW_H

#include <DSimpleListView>
#include <QList>
#include "useritem.h"

DWIDGET_USE_NAMESPACE

class UserListView : public DSimpleListView
{
public:
    explicit UserListView(QWidget *parent = nullptr);
    void setUserList(const QList<UserInfo> &userList);
};
#endif // USERLISTVIEW_H
