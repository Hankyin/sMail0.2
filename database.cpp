#include "database.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>

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
                             "  mail  TEXT    NOT NULL,"
                             "  pass  TEXT    NOT NULL,"
                             "  name      TEXT    NOT NULL,"
                             "  IMAPServer TEXT    NOT NULL,"
                             "  IMAPPort   INT     NOT NULL,"
                             "  IMAPSSL    INT     NOT NULL,"
                             "  SMTPServer TEXT   NOT NULL,"
                             "  SMTPPort  INT     NOT NULL,"
                             "  SMTPSSL   INT     NOT NULL"
                             ");");
        if(!ok)
        {
            this->lastErrorString = query.lastError().text();
            emit error();
            return;
        }
    }

    query.exec("SELECT count(*) "
               "FROM sqlite_master "
               "WHERE type = 'table' AND name = 'Mail' ;");
    //判断mails是否存在，不存在则创建一个
    int hasMailTable;
    while (query.next())
    {
        hasMailTable = query.value(0).toInt();
    }
    if(!hasMailTable)
    {
        bool ok = query.exec("CREATE TABLE Mail"
                             "("
                             "  index INT NOT NULL,"
                             "  content TEXT NOT NULL"
                             ");");
        if(!ok)
        {
            this->lastErrorString = query.lastError().text();
            emit error();
            return;
        }
    }

    query.exec("SELECT count(*) "
               "FROM sqlite_master "
               "WHERE type = 'table' AND name = 'Dir' ;");
    //判断dir是否存在，不存在则创建一个
    int hasDirTable;
    while (query.next())
    {
        hasDirTable = query.value(0).toInt();
    }
    if(!hasDirTable)
    {
        bool ok = query.exec("CREATE TABLE Dir"
                             "("
                             "  mail  TEXT    NOT NULL,"
                             "  dir  TEXT    NOT NULL,"
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

QList<DirInfo> DataBase::getDirList(const QString &mail)
{
    bool ok = false;
    QList<DirInfo> list;
    QSqlQuery query(db);
    ok = query.exec("SELECT *"
                     "FROM Dir;");
     while (query.next())
     {
         DirInfo u;
         u.dirName = query.value(1).toString();
         list.append(u);
     }
    return list;
}

QList<MailInfo> DataBase::getMailList(const QString &mail,const QString &dir)
{
    bool ok = false;
    QList<MailInfo> list;
    QSqlQuery query(db);
    ok = query.exec("SELECT *"
                     "FROM Mail;");
     while (query.next())
     {
         MailInfo u;
         u.subject = query.value(1).toString();
         list.append(u);
     }
    return list;
}

void DataBase::insertNewUser(const UserInfo &newUser)
{
     //在User表中插入新记录
     QString insert = "INSERT INTO User"
                      "("
                      "  mail,"
                      "  pass,"
                      "  name,"
                      "  IMAPServer,"
                      "  IMAPPort,"
                      "  IMAPSSL,"
                      "  SMTPServer,"
                      "  SMTPPort,"
                      "  SMTPSSL"
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
}

void DataBase::insertNewDir(const DirInfo &newDir)
{
    //在User表中插入新记录
    QString insert = "INSERT INTO Dir"
                     "("
                     "  mail,"
                     "  dir,"
                     ")"
                     "VALUES"
                     "(?,?);";
    QSqlQuery query(db);
    query.prepare(insert);
    query.addBindValue("yin");
    query.addBindValue(newDir.dirName);

    bool ok = query.exec();
}

void DataBase::insertNewMail(int index,const QString &mail)
{
    //在User表中插入新记录
    QString insert = "INSERT INTO Mail"
                     "("
                     "  index,"
                     "  content"
                     ")"
                     "VALUES"
                     "(?,?);";
    QSqlQuery query(db);
    query.prepare(insert);
    query.addBindValue(index);
    query.addBindValue(mail);
    bool ok = query.exec();
}
