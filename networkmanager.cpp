
#include "networkmanager.hpp"
#include"datamanager.h"
#include"CoreFunctionality/cutpyramid.hpp"
#include<QThreadPool>
#include<glog/logging.h>
NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{

}

void NetworkManager::login(QString account,QString password)
{


    bool result=DataManager::checkIfUserInDataBase(account,password);

    if(!result)
    {
        emit loginResult(false,"用户名或密码错误");
        return;
    }

    emit loginResult(true,"success");
}

void NetworkManager::startOpen(QString file,QString dst)
{
    CutPyramid* pyramid=new CutPyramid{file,dst,1024,0};
    connect(pyramid,&CutPyramid::runningFinished,this,[=](bool sucess){
        if(sucess){
            //switch to the status when task is done.
            this->currentStatus=ANALIZE_STATUS::OPEN;
        }else {
            this->currentStatus=ANALIZE_STATUS::ERROR;
            LOG(WARNING)<<"error when Open image(cut)";
        }

    });
    QThreadPool::globalInstance()->start(pyramid);
}


