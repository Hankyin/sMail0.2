#include "maillistview.h"

MailListView::MailListView(QWidget *parent) : DSimpleListView(parent)
{
    setFrame(true,QColor(Qt::red),1);
    setClipRadius(5);
    setSearchAlgorithm(&MailItem::search);
}

void MailListView::setMailList(const QList<MailInfo> &mailList)
{
    QList<DSimpleListItem*> items;
    for(auto i : mailList)
    {

        MailItem *item = new MailItem(i);
        items.append(item);
    }
    this->clearItems();
    this->addItems(items);
}
