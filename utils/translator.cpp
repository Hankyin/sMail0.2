#include <QUrl>
#include <QUrlQuery>
#include <QDateTime>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include "translator.h"

Translator::Translator(QObject *parent) : QObject(parent)
{
    youdaoURL = "http://openapi.youdao.com/api";//有道云翻译
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

QStringList Translator::getLanguageList()
{
    QStringList list;
    list<<"中文 zh-CHS"<<"日文 ja"<<"英文 EN"<<"韩文 ko"
       <<"法文 fr"<<"俄文 ru"<<"葡萄牙文 pt"<<"西班牙文 es";
    return list;
}

void Translator::requireTranslate(const QString &str, const QString &targetLanguage)
{
    //youdaoAPI 接口参数
    QString q = str;
    QString appKey = "65a48ed2eb2e1860";
    QString from = "auto";
    QString to = targetLanguage.split(' ',QString::SkipEmptyParts).at(1);
    QString salt = QString::number(QDateTime::currentDateTime().toTime_t());
    QString appSecret = "IuEIT7g1rzEAHkpaWUTd2jhx7HwEreo3";
    QString md5Str = appKey + q + salt + appSecret;
    QString sign = QCryptographicHash::hash(md5Str.toUtf8(),
                                            QCryptographicHash::Md5).toHex();
    QUrl url(youdaoURL);
    QUrlQuery query;
    query.addQueryItem("q",q);
    query.addQueryItem("appKey",appKey);
    query.addQueryItem("from",from);
    query.addQueryItem("to",to);
    query.addQueryItem("salt",salt);
    query.addQueryItem("sign",sign);
    url.setQuery(query);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    manager->get(request);
}

void Translator::replyFinished(QNetworkReply *reply)
{
    reply->errorString();
    QByteArray translation = reply->readAll();
    QString key = "translation";
    QString translatedText;
    QJsonDocument json = QJsonDocument::fromJson(translation);
    QJsonObject object = json.object();
    if(object.contains(key))
    {
        QJsonValue result;
        result = object.value(key);
        translatedText = result.toArray().at(0).toString();
    }
    emit translateResult(translatedText);
}
//QNetworkAccessManager *manager = new QNetworkAccessManager(this);
//connect(manager, SIGNAL(finished(QNetworkReply*)),
//        this, SLOT(replyFinished(QNetworkReply*)));

//manager->get(QNetworkRequest(QUrl("http://qt-project.org")));
