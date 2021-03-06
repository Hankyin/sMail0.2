﻿#include "mailviewer.h"
#include <QHBoxLayout>
#include "utils/mime.h"

MailViewer::MailViewer(DataBase *db)
    : QWidget(nullptr)
{
    //init member
    this->db = db;
    imap = new IMAP(this);
    mailSender = nullptr;
    timer = new QTimer(this);//定时向imap发送noop

    //从数据库中读取数据
    userList = db->getUserList();
    user = userList.at(0);//默认第一个用户登录
    dir = "INBOX";
    mailList = db->getMailList(user.mail,dir);//默认INBOX

    initUI();
    initConnect();

    imap->setDebug(true);
    imap->connectServer(user.IMAPServer,user.IMAPPort,user.isIMAPSSL);
}

MailViewer::~MailViewer()
{
    imap->logout();
}


void MailViewer::writeMail()
{
    if(mailSender == nullptr)
    {
        mailSender = new MailSender(user);
    }
    mailSender->show();
}


//IMAP系列响应函数
void MailViewer::IMAPError(QString error)
{
    qDebug()<< error<<endl;
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
    dirList = dirs;
    sideBar->setDirList(dirList);
    this->dir = "INBOX";
    imap->select(this->dir);
}

void MailViewer::IMAPSelectFinish(QString dir, uint mails, uint recents)
{
    this->dir = dir;
    this->mailList = db->getMailList(user.mail,dir);
    this->mailPage->setMailList(mailList);
    if(mailList.isEmpty())
    {
        imap->search("ALL");
    }
    else
    {
        imap->search("NEW");
    }
}

void MailViewer::IMAPSearchFinish(QList<int> searchList)
{
    //搜索结果可能是不连续的，所以只能一个一个fetch
    for(auto i : searchList)
    {
        if(i == 0)
            continue;
        imap->fetch(QString::number(i),"BODY[]");
    }
}

void MailViewer::IMAPFetchFinsh(int index, QString mail)
{
    MailPraser praser(mail.toUtf8());
    MailInfo head;
    head.subject = praser.getSubject();
    head.to = praser.getTo();
    head.from = praser.getFrom();
    head.datetime = praser.getDateTime();
    head.index = index;
    head.hasRead = false;
    head.dir = this->dir;
    head.mailbox = user.mail;
    db->insertNewMail(head,mail);
    mailList.append(head);
    mailPage->setMailList(mailList);
}



void MailViewer::showMail(int i)
{
    QString mail = db->getMailByID(i,user.mail);
    mailBrowser->setMail(mail);
}

void MailViewer::changeDir(QString dir)
{
    imap->select(dir);
}

void MailViewer::changeUser(QString mail)
{
    //TODO
}

void MailViewer::initUI()
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    sideBar = new SideBar(this);
    mailPage = new MailPage(this);
    mailBrowser = new MailBrowser(this);
    sideBar->setUserList(this->userList);
    mailPage->setMailList(this->mailList);
    hLayout->addWidget(sideBar);
    hLayout->addWidget(mailPage);
    hLayout->addWidget(mailBrowser,1);
}

void MailViewer::initConnect()
{
    connect(sideBar,SIGNAL(selectDir(QString)),this,SLOT(changeDir(QString)));
    connect(sideBar,SIGNAL(selectUser(QString)),this,SLOT(changeUser(QString)));
    connect(sideBar,SIGNAL(writeMailClick()),this,SLOT(writeMail()));
    connect(mailPage,SIGNAL(itemClick(int)),this,SLOT(showMail(int)));

    connect(imap,SIGNAL(IMAPError(QString)),this,SLOT(IMAPError(QString)));
    connect(imap,SIGNAL(connected()),this,SLOT(IMAPConnected()));
    connect(imap,SIGNAL(loginFinish()),this,SLOT(IMAPLoginFinish()));
    connect(imap,SIGNAL(listFinish(QList<IMAPDir>)),this,SLOT(IMAPListFinish(QList<IMAPDir>)));
    connect(imap,SIGNAL(selectFinish(QString,uint,uint)),this,SLOT(IMAPSelectFinish(QString,uint,uint)));
    connect(imap,SIGNAL(fetchFinish(int,QString)),this,SLOT(IMAPFetchFinsh(int,QString)));
    connect(imap,SIGNAL(searchFinish(QList<int>)),this,SLOT(IMAPSearchFinish(QList<int>)));
}
