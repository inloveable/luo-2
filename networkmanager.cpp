
#include "networkmanager.hpp"
#include"datamanager.h"
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


