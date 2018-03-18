#include "dirlistview.h"

DirListView::DirListView(QWidget *parent) : DSimpleListView(parent)
{
    setFrame(true,QColor(Qt::red),1);
    setClipRadius(5);
}

void DirListView::setDirList(const QList<DirInfo> &dirList)
{
    QList<DSimpleListItem*> items;
    for(auto i : dirList)
    {
        DirItem *item = new DirItem(i.dirName);
        items << item;
    }
    addItems(items);
}
