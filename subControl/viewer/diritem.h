#ifndef DIRITEM_H
#define DIRITEM_H


#include <DSimpleListItem>
#include <QDateTime>
#include "utils/type.h"

DWIDGET_USE_NAMESPACE

class DirItem : public DSimpleListItem
{
    Q_OBJECT
public:
    DirItem(const QString &dir);

    bool sameAs(DSimpleListItem *item);//判定item是否相同
    void drawBackground(QRect rect, QPainter *painter, int index, bool isSelect, bool isHover);
    void drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect, bool isHover);

    QString dirName;
};

#endif // DIRITEM_H
