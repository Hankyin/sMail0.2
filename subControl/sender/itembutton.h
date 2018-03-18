#ifndef ITEMBUTTON_H
#define ITEMBUTTON_H

#include <QPushButton>
#include <QString>
#include <QIcon>

class QMenu;
class QAction;

class ItemButton : public QPushButton
{
    Q_OBJECT
public:
    ItemButton(uint id, const QString &text, QWidget *parent = Q_NULLPTR);
    void addAction(QAction *action);
    QString getData() const;
    void setData(const QString &value);
    uint getID() const {return this->id;}
    void setIcon(const QIcon &icon);

signals:
    void remove(uint id);
private slots:
    void del();
private:
    QMenu *menu;
    QString data;
    uint id;//用来标识Item身份，Item通常成群出现，在删除它的时候可能会需要
};

#endif // ITEMBUTTON_H
