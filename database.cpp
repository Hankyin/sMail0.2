#include "database.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>

//#define ERROR_HANDLER         if(!ok)\
//{\
//    this->lastErrorString = query.lastError().text();\
//    emit error();\
//    return;\
//}

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("sMail.db");
    if(!db.open())
    {
        this->lastErrorString = "can't open datebase";
        emit error();
        return ;
    }
    //sqlite会自动维护一个系统表sqlite_master，该表存储了我们所创建的各个table, view, trigger等等信息。
    QSqlQuery query(db);
    query.exec("SELECT count(*) "
               "FROM sqlite_master "
               "WHERE type = 'table' AND name = 'User' ;");
    //判断user是否存在，不存在则创建一个
    int hasUserTable;
    while (query.next())
    {
        hasUserTable = query.value(0).toInt();
    }
    if(!hasUserTable)
    {
        bool ok = query.exec("CREATE TABLE User"
                             "("
                             "  mail        TEXT    NOT NULL,"
                             "  pass        TEXT    NOT NULL,"
                             "  name        TEXT    NOT NULL,"
                             "  imapServer  TEXT    NOT NULL,"
                             "  imapPort    INT     NOT NULL,"
                             "  imapSsl     INT     NOT NULL,"
                             "  smtpServer  TEXT    NOT NULL,"
                             "  smtpPort    INT     NOT NULL,"
                             "  smtpSsl     INT     NOT NULL"
                             ");");
        if(!ok)
        {
            this->lastErrorString = query.lastError().text();
            emit error();
            return;
        }
        //mail,dir,index,hasRead,from,to,subject,date, CONTENT.
        ok = query.exec("CREATE TABLE Mail"
                         "("
                         "  mailbox TEXT    NOT NULL,"
                         "  dir     TEXT    NOT NULL,"
                         "  id      INT     NOT NULL,"
                         "  hasRead INT     NOT NULL,"
                         "  sender  TEXT    NOT NULL,"
                         "  receiver TEXT    NOT NULL,"
                         "  subject TEXT    NOT NULL,"
                         "  datetime TEXT   NOT NULL,"
                         "  content TEXT    NOT NULL "
                         ");");
        if(!ok)
        {
            this->lastErrorString = query.lastError().text();
            emit error();
            return;
        }
    }
}

QList<UserInfo> DataBase::getUserList()
{
    bool ok = false;
    QList<UserInfo> list;
    QSqlQuery query(db);
    ok = query.exec("SELECT *"
                     "FROM User;");
     while (query.next())
     {
         UserInfo u;
         u.mail = query.value(0).toString();
         u.pass = query.value(1).toString();
         u.name = query.value(2).toString();
         u.IMAPServer = query.value(3).toString();
         u.IMAPPort = query.value(4).toInt();
         u.isIMAPSSL = query.value(5).toBool();
         u.SMTPServer = query.value(6).toString();
         u.SMTPPort = query.value(7).toInt();
         u.isSMTPSSL = query.value(8).toBool();
         list.append(u);
     }
    return list;
}

QList<MailInfo> DataBase::getMailList(const QString &mailbox,const QString &dir)
{
    bool ok = false;
    QList<MailInfo> list;
    QSqlQuery query(db);
    QString sql = "SELECT * FROM Mail WHERE mailbox like '%1' AND dir like '%2';";
    ok = query.exec(sql.arg(mailbox,dir));

    if(!ok)
    {
        this->lastErrorString = query.lastError().text();
        emit error();
        return list;
    }

    while (query.next())
    {
        MailInfo m;
        m.mailbox = query.value("mailbox").toString();
        m.dir = query.value("dir").toString();
        m.index = query.value("id").toInt();
        m.hasRead = query.value("hasRead").toBool();
        m.from = query.value("sender").toString();
        m.to = query.value("receiver").toString();
        m.subject = query.value("subject").toString();
        m.datetime = query.value("datetime").toDateTime();
        list.append(m);
    }
    return list;
}

QString DataBase::getMailByID(int index, const QString &mailbox)
{
    QString mail;
    QSqlQuery query(db);
    QString sql = "SELECT content FROM Mail WHERE mailbox like '%1' AND id=%2";
    bool ok = query.exec(sql.arg(mailbox,QString::number(index)));
    if(!ok)
    {
        this->lastErrorString = query.lastError().text();
        emit error();
        return mail;
    }
    while(query.next())
    {
        mail = query.value("content").toString();
    }
    return mail;
}

void DataBase::insertNewUser(const UserInfo &newUser)
{
     //在User表中插入新记录
     QString insert = "INSERT INTO User"
                      "("
                      "  mail,"
                      "  pass,"
                      "  name,"
                      "  imapServer,"
                      "  imapPort,"
                      "  imapSsl,"
                      "  smtpServer,"
                      "  smtpPort,"
                      "  smtpSsl"
                      ")"
                      "VALUES"
                      "(?,?,?,?,?,?,?,?,?);";
     QSqlQuery query(db);
     query.prepare(insert);
     query.addBindValue(newUser.mail);
     query.addBindValue(newUser.pass);
     query.addBindValue(newUser.name);
     query.addBindValue(newUser.IMAPServer);
     query.addBindValue(newUser.IMAPPort);
     query.addBindValue(newUser.isIMAPSSL);
     query.addBindValue(newUser.SMTPServer);
     query.addBindValue(newUser.SMTPPort);
     query.addBindValue(newUser.isSMTPSSL);
     bool ok = query.exec();
     if(!ok)
     {
         this->lastErrorString = query.lastError().text();
         emit error();
         return;
     }
}

void DataBase::insertNewMail(const MailInfo &mailinfo, const QString &mail)
{
    //在User表中插入新记录
    QString insert = "INSERT INTO Mail"
                     "("
                     "  mailbox,"
                     "  id,"
                     "  dir,"
                     "  hasRead,"
                     "  sender,"
                     "  receiver,"
                     "  subject,"
                     "  datetime,"
                     "  content "
                     ")"
                     "VALUES"
                     "(?,?,?,?,?,?,?,?,?);";
    QSqlQuery query(db);
    query.prepare(insert);
    query.addBindValue(mailinfo.mailbox);
    query.addBindValue(mailinfo.index);
    query.addBindValue(mailinfo.dir);
    query.addBindValue(mailinfo.hasRead);
    query.addBindValue(mailinfo.from);
    query.addBindValue(mailinfo.to);
    query.addBindValue(mailinfo.subject);
    query.addBindValue(mailinfo.datetime);
    query.addBindValue(mail);
    bool ok = query.exec();
    if(!ok)
    {
        this->lastErrorString = query.lastError().text();
        emit error();
        return;
    }
}
