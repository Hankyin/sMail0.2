#ifndef DIRPAGE_H
#define DIRPAGE_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include "dirlistview.h"

class DirPage : public QWidget
{
    Q_OBJECT
public:
    explicit DirPage(QWidget *parent = nullptr);
    void setDirList(const QList<DirInfo> &dirList);
signals:
    void itemClick(QString dir);
public slots:
    void selectItem(DSimpleListItem* item);
private:
    DirListView *dirList;
    QToolButton *btAdd;
    QLabel *labDir;

};

#endif // DIRPAGE_H
