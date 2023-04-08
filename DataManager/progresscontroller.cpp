
#include "progresscontroller.hpp"
#include "qprogressbar.h"
#include<QTimer>
ProgressController::ProgressController(QObject *parent)
    : QObject{parent}
{

}

void ProgressController::start(){
    if(timer==nullptr)
    {
        timer=new QTimer(this);
        timer->setInterval(3000);
        connect(timer,&QTimer::timeout,this,[=](){
            if(currentVal==100)
            {
                timer->stop();
            }
            currentVal++;
            emit updateProgressBar(currentVal);
        });
    }
}

void ProgressController::adoptProgressBar(QProgressBar* bar)
{
    connect(this,&ProgressController::updateProgressBar,bar,&QProgressBar::setValue);
}

bool ProgressController::moveToNextStage()
{
    ++currentStage;
    if(currentStage==stages)
    {
        currentStage=0;
        emit updateProgressBar(100);
        currentVal=100;
        if(timer!=nullptr)
        {
            timer->stop();
        }
        return false;
    }

    currentVal=(currentStage/stages)*100;
    emit updateProgressBar(currentVal);
    return true;
}

