#include "diritem.h"

DirItem::DirItem(const QString &dir)
{
    dirName = dir;
}

bool DirItem::sameAs(DSimpleListItem *item)
{
    return dirName == (static_cast<DirItem*>(item))->dirName;
}

void DirItem::drawBackground(QRect rect, QPainter *painter, int index, bool isSelect, bool isHover)
{
    QPainterPath path;
    path.addRect(QRectF(rect));
    painter->setOpacity(0.8);
    if(isSelect)
    {
        painter->fillPath(path,QColor(Qt::blue));
    }
    else if(index % 2 == 1)
    {
        painter->fillPath(path,QColor(Qt::gray));
    }
}

void DirItem::drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect, bool isHover)
{
    painter->setOpacity(1);
    int padding = 10;
    painter->drawText(QRect(rect.x() + padding,rect.y(),rect.width() - padding * 2,rect.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,dirName);
}
