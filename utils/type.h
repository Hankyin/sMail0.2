#ifndef TYPE_H
#define TYPE_H

#include <QString>
#include <QDate>
//邮件摘要信息，可用于邮件列表的显示。
class MailInfo
{
public:
    MailInfo() {}
    QString sender;
    QString subject;
    QString abstract;
    bool hasRead;
    QDate date;
    int index;
    MailInfo& operator =(const MailInfo &item)
    {
        this->subject = item.subject;
        this->sender = item.sender;
        this->abstract = item.abstract;
        this->date = item.date;
        this->hasRead = item.hasRead;
        this->index = item.index;
        return *this;
    }
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
