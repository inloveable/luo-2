
#pragma once


#include "qsplitter.h"
#include<QSplitter>


class mySpliter : public QSplitter
{
public:
    mySpliter(QWidget* parent=nullptr):QSplitter(parent)
        {

        };


    void setHidden(QWidget* widget);
    void setDownHidden(QWidget* widget);
    void setRightHidden(QWidget* widget);

    void saveMyState();
    void restoreMyState();

private:



    QByteArray lastState;
};

