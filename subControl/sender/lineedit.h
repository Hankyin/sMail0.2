#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QWidget>
#include <QString>
#include <QList>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QHBoxLayout>
#include <QCompleter>

class ItemButton;

class LineEdit : public QWidget
{
    Q_OBJECT
public:
    explicit LineEdit(QString title, bool enableItem = false, QWidget *parent = nullptr);
    void setTitle(QString title) {this->label->setText(title);}
    void setReadOnly(bool readonly) {this->edit->setReadOnly(readonly);}
    void setComplater(QCompleter *complater);
    QList<QString> getItemList();
    QString text() {return this->edit->text();}
signals:
    void itemCountChange();
private slots:
    void addItem();
    void delItem(uint id);
    void resizeTextMargins();
private:
    QLabel *label;
    QLineEdit *edit;
    QFrame *line;
    QHBoxLayout *editLayout;//用于存放ItemButton
    QList<ItemButton*> itemBtList;

};

#endif // LINEEDIT_H
