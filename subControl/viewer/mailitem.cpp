#include "mailitem.h"
#include <QDebug>
#include <QPainterPath>
#include <QPainter>
#include <QRect>

MailItem::MailItem(const MailInfo &item)
{
    itemInfo = item;
}

bool MailItem::sameAs(DSimpleListItem *item)
{
//    return itemInfo.subject == (static_cast<MailItem*>(item))->itemInfo.subject;
    return false;
}

void MailItem::drawBackground(QRect rect, QPainter *painter, int index, bool isSelect, bool isHover)
{
    QPainterPath path;
    path.addRect(QRectF(rect));
    painter->setOpacity(0.8);
    if(isSelect)
    {
        painter->fillPath(path,QColor(Qt::blue));
    }
    else if(isHover)
    {
        painter->fillPath(path,QColor(Qt::darkGray));
    }
    else if(index % 2 == 1)
    {
        painter->fillPath(path,QColor(Qt::gray));
    }
}

void MailItem::drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect, bool isHover)
{
    painter->setOpacity(1);
    if(isSelect)
    {
        painter->setPen(QPen(QColor(Qt::white)));
    }
    else
    {
        painter->setPen(QPen(QColor(Qt::black)));
    }
    int padding = 10;
    painter->drawText(QRect(rect.x() + 1,rect.y(),rect.width()/2,rect.height()/3),Qt::AlignLeft|Qt::AlignTop,itemInfo.from);
    painter->drawText(QRect(rect.x() + padding,rect.y()+rect.height()/3,rect.width() - padding * 2,rect.height()/3*2),
                      Qt::AlignLeft|Qt::AlignVCenter,itemInfo.subject);
}

bool MailItem::search(const DSimpleListItem *item, QString searchContent)
{
    const MailItem *mailItem = static_cast<const MailItem*>(item);
    return (mailItem->itemInfo.subject.contains(searchContent,Qt::CaseInsensitive) ||
            mailItem->itemInfo.from.contains(searchContent));
}
