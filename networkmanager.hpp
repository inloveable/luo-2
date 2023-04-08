
#pragma once


#include <QObject>

//TODO:Maybe access login info remotely
//which is why called networkmanager
//and this should be making total domination of anylizing job
//DataManager runs in mainThread;
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

    void login(QString account,QString password);
signals:
    void loginResult(bool result,QString message);

};

