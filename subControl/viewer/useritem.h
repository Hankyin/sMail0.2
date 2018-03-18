#ifndef USERITEM_H
#define USERITEM_H

#include <DSimpleListItem>
#include <QDateTime>
#include "utils/type.h"

DWIDGET_USE_NAMESPACE

class UserItem : public DSimpleListItem
{
    Q_OBJECT
public:
    UserItem(const QString &user);

    bool sameAs(DSimpleListItem *item);//判定item是否相同
    void drawBackground(QRect rect, QPainter *painter, int index, bool isSelect, bool isHover);
    void drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect, bool isHover);
private:
    QString userName;
};

#endif // USERITEM_H
