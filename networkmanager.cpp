
#include "networkmanager.hpp"
#include"DataManager/datamanager.h"
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
    CutPyramid* pyramid=new CutPyramid{file,dst,1024,0,
          [manager=this](int sucess)
            {
                if(sucess>0)
                {
                //switch to the correct status when task is done.
                manager->currentStatus=ANALIZE_STATUS::OPEN;
                }else {
                manager->currentStatus=ANALIZE_STATUS::ERROR;
                LOG(WARNING)<<"error when Open image(cut)";
                }

           }};

    QThreadPool::globalInstance()->start(pyramid);
}


