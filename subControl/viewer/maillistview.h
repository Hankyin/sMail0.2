#ifndef MAILLISTVIEW_H
#define MAILLISTVIEW_H

#include <DSimpleListView>
#include "mailitem.h"

DWIDGET_USE_NAMESPACE

class MailListView : public DSimpleListView
{
    Q_OBJECT
public:
    explicit MailListView(QWidget *parent = nullptr);
    void setMailList(const QList<MailInfo> &mailList);
signals:

public slots:
};

#endif // MAILLIST_H
