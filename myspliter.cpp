
#include "myspliter.hpp"
#include "qnamespace.h"
#include "qsplitter.h"



void mySpliter::saveMyState()
{
    lastState=this->saveState();
}

void mySpliter::restoreMyState()
{
    restoreState(lastState);
}

void mySpliter::setHidden(QWidget* widget)
{
    int index=this->indexOf(widget);
    if(index==-1)
    {
        qDebug()<<"widget not found";
        return;
    }
    moveSplitter(0,index);

}

void mySpliter::setDownHidden(QWidget* widget)
{
    int index=this->indexOf(widget);
    if(index==-1)
    {
        qDebug()<<"widget not found";
        return;
    }
    moveSplitter(rect().height(),index);
}

void mySpliter::setRightHidden(QWidget* widget)
{
    int index=this->indexOf(widget);
    if(index==-1)
    {
        qDebug()<<"widget not found";
        return;
    }
    moveSplitter(this->rect().width(),index);
}
