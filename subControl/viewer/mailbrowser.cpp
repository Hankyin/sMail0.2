#include "mailbrowser.h"
#include <QVBoxLayout>
#include <QUrl>
#include <QDesktopServices>
#include <QLabel>
#include <QPushButton>
#include "utils/mime.h"

MailBrowser::MailBrowser(QWidget *parent) : QStackedWidget(parent)
{
    browser = new QWidget;
    conversation = new QWidget;
    QVBoxLayout *bvLayout = new QVBoxLayout();
    from = new QLineEdit();
    to = new QLineEdit();
    subject = new QLineEdit();
    webView = new QWebView();
    from->setReadOnly(true);
    to->setReadOnly(true);
    subject->setReadOnly(true);
    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    bvLayout->addWidget(from);
    bvLayout->addWidget(to);
    bvLayout->addWidget(subject);
    bvLayout->addWidget(webView);
    browser->setLayout(bvLayout);
    connect(this->webView,SIGNAL(linkClicked(QUrl)),this,SLOT(openExternalLink(QUrl)));

    QVBoxLayout *cvLayout = new QVBoxLayout;
    QLabel *cLabel = new QLabel("会话模式");
    cvLayout->addWidget(cLabel);
    conversation->setLayout(cvLayout);
    this->addWidget(browser);
    this->addWidget(conversation);
}

void MailBrowser::setMailList(const QList<QString> &mails)
{
    setCurrentWidget(conversation);
//    for(auto i : mails)
//    {
//        MailPraser praser(i);
//        QPushButton *bt = new QPushButton;
//        bt->setText(praser.getSubject());
//        bt->da
//    }
    this->mailList = mails;
    for(int i = 0;i < mails.count();i++)
    {
        //用按钮暂代邮件摘要框
        QPushButton *bt = new QPushButton;
        bt->setText(QString::number(i));
        connect(bt,SIGNAL(clicked(bool)),this,SLOT(openMail(int)));
        conversation->layout()->addWidget(bt);
    }
}

void MailBrowser::setMail(const QString &mail)
{
    setCurrentWidget(browser);
    MailPraser praser(mail.toUtf8());
    from->setText(praser.getFrom());
    to->setText(praser.getTo());
    webView->setHtml(praser.getHtml());
}

void MailBrowser::openExternalLink(QUrl url)
{
    QDesktopServices::openUrl(url);
}

void MailBrowser::openMail(int index)
{
    setMail(mailList.at(index).toUtf8());
}
