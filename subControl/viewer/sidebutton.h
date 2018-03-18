#ifndef SIDEBUTTON_H
#define SIDEBUTTON_H

#include <QToolButton>
#include <QSize>

#define BUTTON_SIZE 60

class SideButton : public QToolButton
{
    Q_OBJECT
public:
    SideButton(const QString icon,QWidget *parent = Q_NULLPTR);
};

#endif // SIDEBUTTON_H
