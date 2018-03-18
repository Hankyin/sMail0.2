#include "smail.h"
#include "QDebug"

sMail::sMail(QObject *parent)
    : QObject(parent)
{
    accountManager = new AccountManager(this);
//    mailViewer = new MailViewer(this);
    imap = new IMAP(this);
    db = new DataBase(this);

    connect(accountManager,SIGNAL(userInfoReady(UserInfo)),this,SLOT(login(UserInfo)));

    connect(imap,SIGNAL(IMAPError(QString)),this,SLOT(IMAPError(QString)));
    connect(imap,SIGNAL(connected()),this,SLOT(IMAPConnected()));
    connect(imap,SIGNAL(loginFinish()),this,SLOT(IMAPLoginFinish()));
    connect(imap,SIGNAL(listFinish(QList<IMAPDir>)),this,SLOT(IMAPListFinish(QList<IMAPDir>)));
    connect(imap,SIGNAL(selectFinish(QString,uint,uint)),this,SLOT(IMAPSelectFinish(QString,uint,uint)));
    connect(imap,SIGNAL(fetchFinish(int,QString)),this,SLOT(IMAPFetchFinsh(int,QString)));
    connect(imap,SIGNAL(searchFinish(QList<int>)),this,SLOT(IMAPSearchFinish(QList<int>)));

    userList = db->getUserList();
    if(userList.isEmpty())
    {
        return;
    }
    else
    {
        login(userList.at(0));
    }
}

sMail::~sMail()
{

}

void sMail::login(UserInfo user)
{
    curUser= user;
    mailViewer = new MailViewer(curUser);
//    db->insertNewUser(user);//如果用户存在，则忽略，否则插入
//    dirList = db->getDirList(curUser.mail);
//    mailList = db->getMailList(curUser.mail,"INBOX");
//    mailViewer->setUserList(userList);
//    mailViewer->setDirList(dirList);
//    mailViewer->setMailList(mailList);
    mailViewer->show();

    //开始连接imap,
//    imap->setDebug(true);
//    imap->connectServer(curUser.IMAPServer,curUser.IMAPPort,curUser.isIMAPSSL);

}

void sMail::IMAPError(QString error)
{
    qDebug()<<error<<endl;
}

void sMail::IMAPConnected()
{
    imap->login(curUser.mail,curUser.pass);
}

void sMail::IMAPLoginFinish()
{
    imap->list();
}

void sMail::IMAPListFinish(QList<IMAPDir> dirs)
{
    if(dirList != dirs)
    {
        dirList = dirs;
        mailViewer->setDirList(dirList);
    }
    imap->select("INBOX");
}

void sMail::IMAPSelectFinish(QString dir, uint mails, uint recents)
{
    if(mailList.isEmpty())
    {
        //如果邮件列表为空，全部下载，否则下载recents
        for(uint i = 1;i <= mails; i++)
        {
            imap->fetch(QString::number(i),"BODY[HEADER]");
        }
    }
    else
    {

    }
}

void sMail::IMAPFetchFinsh(int index, QString mail)
{
    db->insertNewMail(index,mail);
    mailList = db->getMailList(curUser.mail,"INBOX");
    mailViewer->setMailList(mailList);
}

void sMail::IMAPSearchFinish(QList<int> searchList)
{
    qDebug()<<searchList<<endl;
}


