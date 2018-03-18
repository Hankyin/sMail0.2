#ifndef MAILITEM_H
#define MAILITEM_H

#include <DSimpleListItem>
#include <QDateTime>
#include "utils/type.h"

DWIDGET_USE_NAMESPACE

class MailItem : public DSimpleListItem
{
    Q_OBJECT
public:
    MailItem(const MailInfo &item);

    bool sameAs(DSimpleListItem *item);//判定item是否相同
    void drawBackground(QRect rect, QPainter *painter, int index, bool isSelect, bool isHover);
    void drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect, bool isHover);

    static bool search(const DSimpleListItem* item,QString searchContent);

    MailInfo itemInfo;
};

#endif // MAILITEM_H
