#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class Translator : public QObject
{
    Q_OBJECT
public:
    explicit Translator(QObject *parent = nullptr);
    void requireTranslate(const QString &str, const QString &targetLanguage);
    QStringList getLanguageList();
signals:
    void translateResult(QString result);
public slots:

private slots:
    void replyFinished(QNetworkReply* reply);
private:
    QString youdaoURL;
    QNetworkAccessManager *manager;
};

#endif // TRANSLATOR_H
