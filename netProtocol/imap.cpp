#include "imap.h"
#include <QTime>
#include <QDebug>
#include <QByteArray>
#include <QRegularExpressionMatch>

IMAP::IMAP(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    //connect 4 slots, connected, tcp error, change state, ready read;
    connect(tcpSocket,SIGNAL(connected()),
            this,SLOT(socketConnected()));
    connect(tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(socketError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(readyRead()),
            this, SLOT(socketReadyRead()));

    sslSocket = new QSslSocket(this);
    //connect 5 slots, connected, tcp error, ssl error, change state, ready read;
    connect(sslSocket, SIGNAL(encrypted()),
            this, SLOT(socketEncrypted()));
    connect(sslSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(sslSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(sslSocket, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(sslErrors(QList<QSslError>)));
    connect(sslSocket, SIGNAL(readyRead()),
            this, SLOT(socketReadyRead()));

    //设置IMAP连接初始状态
    state = "LOGOUT";

    untaggedRE.setPattern("\\* (?<type>[A-Z-]+)( (?<data>.*))?");//* LIST ()asdfasdf
    untaggedRE.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    untaggedNumRE.setPattern("\\* (?<data>\\d+) (?<type>[A-Z-]+)( (?<data2>.*))?");//* 55 EXISTS
    untaggedNumRE.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    responseCodeRE.setPattern("\\[(?<type>[A-Z-]+)( (?<data>[^\\]]*))?\\]");//[UNSEEN <n>]响应码
    responseCodeRE.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

    addRespHandler("OK", &IMAP::okHandler);
    addRespHandler("NO", &IMAP::noHandler);
    addRespHandler("BAD", &IMAP::badHandler);
    addRespHandler("CAPABILITY",&IMAP::capabilityHandler);
    addRespHandler("LIST", &IMAP::listHandler);
    addRespHandler("RECENT", &IMAP::recentHandler);
    addRespHandler("EXISTS", &IMAP::existsHandler);
    addRespHandler("FETCH", &IMAP::fetchHandler);
}

void IMAP::connectServer(const QString hostName, quint16 port, bool ssl)
{
    this->ssl = ssl;
    if(ssl)
    {
        sslSocket->connectToHostEncrypted(hostName,port);
    }
    else
    {
        tcpSocket->connectToHost(hostName,port);
    }

    QTime t = QTime::currentTime();
    int seed = t.msec() + t.second() *1000;
    qsrand(seed);
    tag = int2AP(4096 + qrand() % (65535-4096));
    taggedRE.setPattern(QString("(?<tag>%1\\d+) (?<type>[A-Z]+) (?<data>.*)").arg(tag));
    taggedRE.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    if(debug)
    {
        printMsg(QString("new IMAP4 connection,tag = %1").arg(tag));
    }
}

void IMAP::socketEncrypted()
{
    printMsg("IMAP4 encrypted connection established");
}

void IMAP::socketConnected()
{
    if(debug)
    {
        printMsg("IMAP4 connection established");
    }
}

void IMAP::socketError(QAbstractSocket::SocketError error)
{

}

void IMAP::sslErrors(const QList<QSslError> &errors)
{

}

void IMAP::socketStateChanged(QAbstractSocket::SocketState)
{

}

void IMAP::socketReadyRead()
{
    //读取全部数据
    QByteArray block;
    if(ssl)
    {
        block = sslSocket->readAll();
    }
    else
    {
        block = tcpSocket->readAll();
    }
    //注意在在新获得的数据前加上上次余留的数据。
    block = remains + block;
    //获取的数据不一定是准确的一行，所以以\r\n为标志分割为行，
    QByteArray CRLF("\r\n", 2);
    int idx = 0;
    int from = 0;
    while ((idx = block.indexOf(CRLF, from)) != -1)
    {
        QByteArray lineB = block.mid(from,idx - from);
        QString line = QString::fromLatin1(lineB);
        from = idx + CRLF.length();
        if(startLiteral)
        {
            //说明这一行是邮件文本，不是imap命令
            literalBuf.append(line + CRLF);
            if(debug)
            {
                printMsg("literal:" + line);
            }
            if(literalBuf.size() >= literalSize)
            {
                printMsg(QString("receive literal message: %1, expect %2").arg(
                             QString::number(literalBuf.size()),
                             QString::number(literalSize)));
                startLiteral = false;

            }
        }
        else
        {
            //处理IMAP命令回复
            if(debug)
            {
                printRecvMsg(line);
            }
            //处理一行回复
            IMAPResp resp = respPraser(line);
            //IMAP连接建立，服务器会发一个问好消息，这个消息独立处理
            if(!hasGreeting)
            {
                //表明是第一次接收到消息。
                hasGreeting = true;
                if(resp.type.toUpper() == "PREAUTH")
                {
                    state = "AUTH";

                }
                else if(resp.type.toUpper() == "OK")
                {
                    state = "NONAUTH";
                }
                else
                {
                    emit IMAPError(QString("server can not login:%1").arg(line));
                }
                capability();
                stateChangedProcess();
                continue;
            }
            //调用回复处理函数
            respHandler(resp);
        }
    }
    remains = block.right(block.size() - from);
}

void IMAP::sendCommand(const QString &command, const QString &arg)
{
    if(!CommandList[command].contains(state))
    {
        emit IMAPError(QString("command %1 illegal in state %2").arg(command,state));
        return;
    }
    QStringList sr;
    sr<<"OK"<<"BAD"<<"NO";
    for(auto i : sr)
    {
        if(untaggedResponse.contains(i))
        {
            untaggedResponse.remove(i);
        }
    }
    QString tag = newTag();
    taggedCommand.insert(tag,command);
    QString data = tag + " " + command + " " + arg + "\r\n";
    if(debug)
    {
        printSendMsg(data);
    }
    if(ssl)
    {
        sslSocket->write(data.toLatin1());
    }
    else
    {
        tcpSocket->write(data.toLatin1());
    }
}

void IMAP::respHandler(const IMAPResp &resp)
{
    if(respHanderList.contains(resp.type))
    {
        Handler handler = respHanderList[resp.type];
        (this->*handler)(resp);
    }
}

void IMAP::addRespHandler(const QString &command, const Handler handler)
{
    respHanderList.insert(command,handler);
}

QString IMAP::int2AP(int num)
{
    //convert intger to A-P string representation
    //why A-P ??? 16?
    QString AP = "ABCDEFGHIJKLMNOP";
    QString val = "";
    while(num)
    {
        int mod = num % 16;
        num = num / 16;
        val = AP[mod] + val;
    }
    return val;
}

QString IMAP::newTag()
{
    QString nt = tag + QString::number(tagNum);
    tagNum++;
    return nt;
}

IMAPResp IMAP::respPraser(const QString &resp)
{
    QRegularExpressionMatch match;
    QString tag;
    QString type;
    QString data;
    QString data2;
    IMAPResp msg;
    if(taggedRE.match(resp).hasMatch())
    {
        //tagged 响应
        match = taggedRE.match(resp);
        tag = match.captured("tag");
        if(!taggedCommand.contains(tag))
        {
            emit IMAPError(QString("unexpected tagged response:").arg(resp));
            return msg;
        }
        type = match.captured("type");
        data = match.captured("data");
    }
    else if(untaggedRE.match(resp).hasMatch())
    {
        //untagged 响应1 * LIST (\\Marked \\HasNoChildren) "/" Inbox
        match = untaggedRE.match(resp);
        type = match.captured("type");
        data = match.captured("data");
    }
    else if(untaggedNumRE.match(resp).hasMatch())
    {
        //* 55 EXISTS
        match = untaggedNumRE.match(resp);
        type = match.captured("type");
        data = match.captured("data");
        data2 = match.captured("data2");
        if(!data2.isEmpty())
        {
            data = data + " " + data2;
        }
    }
    else
    {
        emit IMAPError(QString("unexpected response: %1").arg(resp));
        return msg;
    }

//response code的处理
//    QStringList status;
//    status<<"OK"<<"NO"<<"BAD";
//    match = responseCodeRE.match(data);
//    if(status.contains(type,Qt::CaseInsensitive) && match.hasMatch())
//    {
//        //response code
//        type = match.captured("type");
//        data = match.captured("data");
//    }

    msg.tag = tag;
    msg.data = data;
    msg.type = type;
    return msg;
}

void IMAP::printMsg(const QString &msg)
{
    QTime time = QTime::currentTime();
    QString ts = time.toString();
    qDebug()<<ts<<" "<<msg<<endl;
}

void IMAP::printSendMsg(const QString &msg)
{
    printMsg("C: " + msg);
}

void IMAP::printRecvMsg(const QString &msg)
{
    printMsg("S: " + msg);
}

void IMAP::stateChangedProcess()
{
    emit stateChanged(state);
    if(debug)
    {
        printMsg(QString("state changed: %1").arg(state));
    }
}

void IMAP::capabilityHandler(const IMAPResp &resp)
{
    QString caps = resp.data;
    QStringList capList = caps.split(' ',QString::SkipEmptyParts);
    if(!capList.contains("IMAP4") && !capList.contains("IMAP4rev1"))
    {
        emit IMAPError("server not IMAP4 compliant");
        return;
    }
    if(!capList.contains("AUTH=PLAIN") || capList.contains("LOGINDISABLED"))
    {
        emit IMAPError("server can not login");
        return;
    }
    if(!hasConnected)
    {
        emit connected();
    }
}

void IMAP::listHandler(const IMAPResp &resp)
{
    //(\\Marked \\HasNoChildren) \"/\" Inbox
    QRegularExpression dirRE("\\((?<attr>[A-Za-z \\\\]*)\\) \"(?<spliter>.)\" (?<dir>.*)");
    QRegularExpressionMatch match;
    IMAPDir dir;
    match = dirRE.match(resp.data);
    QString dirName = match.captured("dir");
    QString spliter = match.captured("spliter");
    dir.dirName = dirName;//TODO 邮箱名需要进行特种base64解编码
    QString attr = match.captured("attr");
    QStringList attrList = attr.split('\\');
    for(auto i : attrList)
    {
        dir.dirAttr.append(i.trimmed());
    }
    dirList.append(dir);
}

void IMAP::recentHandler(const IMAPResp &resp)
{
    bool ok;
    uint count = resp.data.toUInt(&ok);
    if(!ok)
        emit IMAPError(QString("recent mail count read error:%1").arg(resp.data));
    this->recentMailCount = count;
}

void IMAP::existsHandler(const IMAPResp &resp)
{
    bool ok;
    uint count = resp.data.toUInt(&ok);
    if(!ok)
        emit IMAPError(QString("exists mail count read error:%1").arg(resp.data));
    this->mailCount = count;
}

void IMAP::fetchHandler(const IMAPResp &resp)
{
    //* 1 FETCH (BODY[] {24250}'
    QRegularExpression sizeRE(".*(?<index>\\d+)+.*{(?<size>\\d+)}");
    QRegularExpressionMatch match;
    match = sizeRE.match(resp.data);
    if(match.hasMatch())
    {
        literalSize = match.captured("size").toInt();
    }
    else
    {
        emit IMAPError(QString("fetch data size is zero:%1").arg(resp.data));
        return;
    }
    fetchMailIndex = match.captured("index").toInt();
    startLiteral = true;
}

void IMAP::searchHandler(const IMAPResp &resp)
{
    QStringList indexList = resp.data.split(' ');
    for(auto i : indexList)
    {
        int a = i.toInt();
        searchList.append(a);
    }
}

void IMAP::okHandler(const IMAPResp &resp)
{
    if(taggedCommand.contains(resp.tag))
    {
        //说明是标签回复
        //该命令成功
        QString tag = resp.tag;
        QString cmd = taggedCommand[tag].toUpper();
        if(cmd == "LOGIN")
        {
            state = "AUTH";
            stateChangedProcess();
            emit loginFinish();
        }
        if(cmd == "LIST")
        {
            emit listFinish(dirList);
        }
        if(cmd == "SELECT")
        {
            state = "SELECTED";
            stateChangedProcess();
            emit selectFinish(curDir,mailCount,recentMailCount);
        }
        if(cmd == "FETCH")
        {
            QString mail = literalBuf.left(literalSize);
            literalBuf = "";
            literalSize = 0;
            emit fetchFinish(fetchMailIndex,mail);
        }
        if(cmd == "SEARCH")
        {
            emit searchFinish(searchList);
            searchList.clear();
        }
    }
}

void IMAP::badHandler(const IMAPResp &resp)
{
    if(taggedCommand.contains(resp.tag))
    {
        //说明是标签回复
        //该命令未知或参数错误
        printMsg(QString("command execute fail: %1").arg(resp.data));
    }
}

void IMAP::noHandler(const IMAPResp &resp)
{
    if(taggedCommand.contains(resp.tag))
    {
        //说明是标签回复
        //该命令执行失败
        printMsg(QString("command execute fail: %1").arg(resp.data));
    }
}

void IMAP::setDebug(bool value)
{
    debug = value;
}

void IMAP::capability()
{
    QString command = "CAPABILITY";
    QString arg = "";
    sendCommand(command,arg);
}

void IMAP::noop()
{
    QString command = "NOOP";
    QString arg = "";
    sendCommand(command,arg);
}

void IMAP::logout()
{
    QString command = "LOGOUT";
    QString arg = "";
    sendCommand(command,arg);
}

void IMAP::login(const QString &user, const QString &pass)
{
    QString command = "LOGIN";
    QString arg;
    arg.append(user);
    arg.append(" ");
    arg.append(pass);
    sendCommand(command,arg);
}

void IMAP::list(const QString &dir, const QString &pattern)
{
    QString command = "LIST";
    QString arg;
    QString d;
    QString p;
    if(dir.isEmpty())
    {
        d = "\"\"";
    }
    else
    {
        d = dir;
    }
    if(pattern.isEmpty())
    {
        p = "\"\"";
    }
    else
    {
        p = pattern;
    }
    arg.append(d);
    arg.append(' ');
    arg.append(p);
    sendCommand(command,arg);
}

void IMAP::select(const QString &mailBox)
{
    QString command = "SELECT";
    QString arg = mailBox;
    sendCommand(command,arg);
}

void IMAP::fetch(const QString &mailSet, const QString &mailPart)
{
    QString command = "FETCH";
    QString arg = mailSet;
    arg.append(' ');
    arg.append(mailPart);
    sendCommand(command,arg);
}

void IMAP::search(const QString &criteria)
{
    QString command = "SEARCH";
    QString arg = criteria;
    sendCommand(command,arg);
}
