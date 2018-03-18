#include "useritem.h"

UserItem::UserItem(const QString &user)
{
    userName = user;
}

bool UserItem::sameAs(DSimpleListItem *item)
{
    return false;
}

void UserItem::drawBackground(QRect rect, QPainter *painter, int index, bool isSelect, bool isHover)
{

}

void UserItem::drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect, bool isHover)
{
    painter->setOpacity(1);
    int padding = 10;
    painter->drawText(QRect(rect.x() + padding,rect.y(),rect.width() - padding * 2,rect.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,userName);
}
