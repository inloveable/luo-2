
#include "progresscontroller.hpp"
#include "qprogressbar.h"
#include<QTimer>
#include<glog/logging.h>
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
            if(currentVal==100||currentVal==maxVal)
            {
                timer->stop();
            }
            currentVal++;
            emit updateProgressBar(currentVal);
        });
    }
    timer->start();
}

void ProgressController::adoptProgressBar(QProgressBar* bar)
{
    connect(this,&ProgressController::updateProgressBar,bar,&QProgressBar::setValue);
}

bool ProgressController::moveToNextStage()
{
    LOG(INFO)<<"MOVING TO NEXT STAGE";
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
    timer->start();
    maxVal=((double)(currentStage+1)/(double)stages)*100;
    currentVal=((double)currentStage/(double)stages)*100;
    emit updateProgressBar(currentVal);
    return true;
}

