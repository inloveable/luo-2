
#include "mainwindow.hpp"

#include<QQmlApplicationEngine>
#include<QQuickWindow>
#include"networkmanager.hpp"
#include"datamanager.h"
#include "qnamespace.h"
#include "qqml.h"
#include "qqmlapplicationengine.h"
#include <QThread>
#include<QFileDialog>
#include<QMessageBox>
#include <exception>

#include<glog/logging.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    communicator=new QmlCommunicator(this,this);
    qmlRegisterSingletonInstance("myCpp",1,0,"MainWindow",communicator);
    try{
        engine=new QQmlApplicationEngine(QUrl{"qrc:/qml/LoginWindow.qml"});
    }
    catch(std::exception& ex){
        //some trival kernel exception comes up only in debuggers
        LOG(INFO)<<"QML trigged kernal exceptionL"<<ex.what();
    }

    network=new NetworkManager();
    backEndThread=new QThread;
    connectNetworkSignals();
    backEndThread->start();
    network->moveToThread(backEndThread);
    DataManager::GetInstance()->moveToThread(backEndThread);

}

MainWindow::~MainWindow()
{
    engine->deleteLater();

    backEndThread->quit();
    backEndThread->wait();
    network->deleteLater();
}

void MainWindow::connectNetworkSignals()
{
    connect(network,&NetworkManager::loginResult,communicator,&QmlCommunicator::onLoginResult);
    connect(communicator,&QmlCommunicator::tryLogin,network,&NetworkManager::login);
}

void QmlCommunicator::onLoginResult(bool result,QString message)
{
    if(result==true)
    {
        //mainwindow->show();
    }
    emit loginResult(result,message);

}

void QmlCommunicator::login(QString account,QString password)
{
    if(account==""||password=="")
    {
        emit loginResult(false,"账号或密码为空");
        return;
    }
    emit tryLogin(account,password);
}

int QmlCommunicator::tryRegister(QString account,QString password,QString doctorId)
{
    QMessageBox::information(mainwindow,"提示","医生您好，注册账号请提供个人手写签名照片!");
    auto path=QFileDialog::getOpenFileName(mainwindow,"选择签名照片");
    if(path=="")
    {
        return -1;
    }

    int ret=DataManager::GetInstance()->writeAccountInfoToDatabase(account,password,doctorId,path);
    if(ret==-2)
    {
        QMessageBox::warning(mainwindow,"警告","账号已存在");
    }
    if(ret==-3)
    {
        QMessageBox::warning(mainwindow,"警告","该医生姓名已注册");
    }
    return ret;
}


