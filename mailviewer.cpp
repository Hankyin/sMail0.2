#include "mailviewer.h"
#include <QHBoxLayout>
#include "utils/type.h"
#include "utils/mime.h"
#include "smail.h"
#include "mailsender.h"

MailViewer::MailViewer(UserInfo user)
    : QWidget(nullptr)
{
//    this->smail = smail;
    this->user = user;
    imap = new IMAP(this);
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    sideBar = new SideBar(this);
    listStack = new ListStack(this);
    webView = new QWebView(this);
    hLayout->addWidget(sideBar);
    hLayout->addWidget(listStack);
    hLayout->addWidget(webView,1);

    connect(sideBar,SIGNAL(selectDirClick()),this,SLOT(toDirList()));
    connect(sideBar,SIGNAL(selectUserClick()),this,SLOT(toUserList()));
    connect(listStack,SIGNAL(mailClick(int)),this,SLOT(showMail(int)));
    connect(listStack,SIGNAL(dirClick(QString)),this,SLOT(changeDir(QString)));

    connect(imap,SIGNAL(IMAPError(QString)),this,SLOT(IMAPError(QString)));
    connect(imap,SIGNAL(connected()),this,SLOT(IMAPConnected()));
    connect(imap,SIGNAL(loginFinish()),this,SLOT(IMAPLoginFinish()));
    connect(imap,SIGNAL(listFinish(QList<IMAPDir>)),this,SLOT(IMAPListFinish(QList<IMAPDir>)));
    connect(imap,SIGNAL(selectFinish(QString,uint,uint)),this,SLOT(IMAPSelectFinish(QString,uint,uint)));
    connect(imap,SIGNAL(fetchFinish(int,QString)),this,SLOT(IMAPFetchFinsh(int,QString)));
    connect(imap,SIGNAL(searchFinish(QList<int>)),this,SLOT(IMAPSearchFinish(QList<int>)));

    imap->setDebug(true);
    imap->connectServer(user.IMAPServer,user.IMAPPort,user.isIMAPSSL);
}

MailViewer::~MailViewer()
{

}

void MailViewer::setMailList(const QList<MailInfo> &mailList)
{
    listStack->setMailList(mailList);
}

void MailViewer::setDirList(const QList<DirInfo> &dirList)
{
    listStack->setDirList(dirList);
}

void MailViewer::setUserList(const QList<UserInfo> &userList)
{
    listStack->setUserList(userList);
}

void MailViewer::toMailList()
{
    listStack->setCurrentIndex(0);
}

void MailViewer::toUserList()
{
    listStack->setCurrentIndex(1);
}

void MailViewer::toDirList()
{
    listStack->setCurrentIndex(2);
}

void MailViewer::writeMail()
{
    MailSender *mailSender = new MailSender(user);
}

void MailViewer::IMAPError(QString error)
{

}

void MailViewer::IMAPConnected()
{
    imap->login(user.mail,user.pass);
}

void MailViewer::IMAPLoginFinish()
{
    imap->list();
}

void MailViewer::IMAPListFinish(QList<IMAPDir> dirs)
{
    this->setDirList(dirs);
    imap->select("INBOX");
}

void MailViewer::IMAPSelectFinish(QString dir, uint mails, uint recents)
{
    curDir = dir;
    for(uint i = 1;i < mails;i++)
    {
        imap->fetch(QString::number(i),"BODY[]");
    }
}

void MailViewer::IMAPFetchFinsh(int index, QString mail)
{
    this->mails<<mail;
    MailPraser praser(mail.toUtf8());
    MailInfo head;
    head.subject = praser.getSubject();
    head.sender = praser.getSenderMail();
    head.date = praser.getDateTime().date();
    head.index = index;
    this->headList.append(head);
    this->listStack->setMailList(headList);
}

void MailViewer::IMAPSearchFinish(QList<int> searchList)
{

}

void MailViewer::showMail(int i)
{
    QString mail = mails.at(i-1);
    MailPraser praser(mail.toUtf8());
    QString html = praser.getHtml();
    this->webView->setHtml(html);
}

void MailViewer::changeDir(QString dir)
{
    imap->select(dir);
    toMailList();
}
