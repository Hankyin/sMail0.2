#include "userlistview.h"

UserListView::UserListView(QWidget *parent) : DSimpleListView(parent)
{
    setFrame(true,QColor(Qt::red),1);
    setClipRadius(5);
}

void UserListView::setUserList(const QList<UserInfo> &userList)
{
    QList<DSimpleListItem*> items;
    for(auto i : userList)
    {
        UserItem *item = new UserItem(i.name);
        items << item;
    }
    addItems(items);
}
