#ifndef TYPE_H
#define TYPE_H

#include <QString>
#include <QDateTime>
//邮件摘要信息，可用于邮件列表的显示。
class MailInfo
{
public:
    MailInfo() {}
    QString mailbox;
    QString dir;
    int index;
    bool hasRead;
    QString from;
    QString to;
    QString subject;
    QDateTime datetime;
};

class UserInfo
{
public:
    UserInfo() {}
    QString name;
    QString mail;
    QString pass;
    QString IMAPServer;
    uint IMAPPort;
    bool isIMAPSSL;
    QString SMTPServer;
    uint SMTPPort;
    bool isSMTPSSL;
};



class DirInfo
{
public:
    DirInfo() {}
    QString dirName;
    QStringList dirAttr;
    QList<DirInfo> subDirList;

    bool operator ==(const DirInfo &another)
    {
        if(dirName.compare(another.dirName,Qt::CaseInsensitive))
            return false;
        return true;
    }
};

typedef DirInfo IMAPDir;
#endif // TYPE_H
