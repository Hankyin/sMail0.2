#ifndef EMOJIPICKER_H
#define EMOJIPICKER_H

#include <QTabWidget>
#include <QList>

class EmojiSection
{
    EmojiSection(uint unicodeStart,uint unicodeEnd) {start = unicodeStart;
                                                     end = unicodeEnd;}
    uint start;
    uint end;
};

class EmojiPicker : public QTabWidget
{
    Q_OBJECT
public:
    explicit EmojiPicker(QWidget *parent = nullptr);
    void addEmojiSection(uint unicodeStart, uint unicodeEnd, const QString &label);

signals:
    void pickEmoji(QString emoji);
public slots:
    void emojiClick();
private:
//    QList<EmojiSection> emojiSectionList;
//    QList<QString> emojiStrList;
    QString getEmoji(uint unicode);
};

#endif // EMOJIPICKER_H
