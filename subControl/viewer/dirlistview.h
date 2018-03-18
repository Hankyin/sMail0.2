#ifndef DIRLISTVIEW_H
#define DIRLISTVIEW_H

#include <DSimpleListView>
#include <QList>
#include "diritem.h"

DWIDGET_USE_NAMESPACE

class DirListView : public DSimpleListView
{
public:
    explicit DirListView(QWidget *parent = nullptr);
    void setDirList(const QList<DirInfo> &dirList);

};

#endif // DIRLISTVIEW_H
