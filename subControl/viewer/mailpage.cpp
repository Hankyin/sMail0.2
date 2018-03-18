#include "mailpage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

MailPage::MailPage(QWidget *parent) : QWidget(parent)
{
    leSearch = new QLineEdit;
    btRefresh = new QToolButton;
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(leSearch);
    hLayout->addWidget(btRefresh);
    mailList = new MailListView();
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(mailList);
    setLayout(vLayout);

    connect(mailList,SIGNAL(mousePressChanged(DSimpleListItem*,int,QPoint)),
            this,SLOT(selectItem(DSimpleListItem*)));
    connect(leSearch,SIGNAL(textChanged(QString)),this,SLOT(searchMail()));
}

void MailPage::setMailList(const QList<MailInfo> &mailList)
{
    this->mailList->setMailList(mailList);
}

void MailPage::selectItem(DSimpleListItem *item)
{
    int i = (static_cast<MailItem*>(item))->itemInfo.index;
    emit itemClick(i);
}

void MailPage::searchMail()
{
    QString searchContent = leSearch->text();
    mailList->search(searchContent);
}
