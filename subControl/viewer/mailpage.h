#ifndef MAILPAGE_H
#define MAILPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QToolButton>
#include "maillistview.h"

class MailPage : public QWidget
{
    Q_OBJECT
public:
    explicit MailPage(QWidget *parent = nullptr);
    void setMailList(const QList<MailInfo> &mailList);
signals:
    void itemClick(int i);
public slots:
    void selectItem(DSimpleListItem* item);
    void searchMail();
private:
    MailListView *mailList;
    QLineEdit *leSearch;
    QToolButton *btRefresh;
};

#endif // MAILPAGE_H
