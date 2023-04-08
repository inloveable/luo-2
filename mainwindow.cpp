
#include "mainwindow.hpp"

#include<QQmlApplicationEngine>
#include<QQuickWindow>
#include"networkmanager.hpp"
#include"datamanager.h"
#include "qfilesystemwatcher.h"
#include "qnamespace.h"
#include "qqml.h"
#include "qqmlapplicationengine.h"
#include <QThread>
#include<QFileDialog>
#include<QMessageBox>
#include <exception>

#include<glog/logging.h>

#ifdef DEBUG
#include<QFileSystemWatcher>
#endif
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    communicator=new QmlCommunicator(this,this);

    setWindowTitle("TCT分析");
    qmlRegisterSingletonInstance("myCpp",1,0,"MainWindow",communicator);
    try{
        engine=new QQmlApplicationEngine(QUrl{"qrc:/qml/LoginWindow.qml"});
    }
    catch(std::exception& ex){
        //some trival kernel exception comes up only in debuggers
        LOG(INFO)<<"QML trigged kernal exceptionL"<<ex.what();
    }



    initUi();

    network=new NetworkManager();
    backEndThread=new QThread;
    connectNetworkSignals();
    backEndThread->start();
    network->moveToThread(backEndThread);
    //DataManager::GetInstance()->moveToThread(backEndThread);

#ifdef DEBUG
    QFileSystemWatcher* watcher=new QFileSystemWatcher(this);
    watcher->addPath("./appCache/mainwindow.qss");

    connect(watcher,&QFileSystemWatcher::fileChanged,this,[=]()
            {
                QFile file{"./appCache/mainwindow.qss"};
                file.open(QIODevice::ReadOnly);
                auto array=file.readAll();

                this->setStyleSheet(array);
            });
#endif

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

int QmlCommunicator::intoTCTAnalize(){
    mainwindow->reEnter();
    mainwindow->show();
    return 0;
}

void MainWindow::initUi()
{
    QAction* goBack=new QAction{"返回主界面",this};
    ui.menu->addAction(goBack);

    connect(goBack,&QAction::triggered,this,[=]()
            {
               emit communicator->showMainArea();
               this->close();
            });

    DataManager::GetInstance()->adoptProgressBar(ui.progressBar);

}

void MainWindow::reEnter()
{
   //TODO:
   //refresh ui with newly changed data,or may flush for nex Analize

    LOG(WARNING)<<"reentering";
}

void QmlCommunicator::setCurrentPatient(QString name,QString age,QString sex,
                                     QString checkTime,
                                     QString identity
                                     ,QString phoneNum,
                                     QString hasCance,
                                     QString hasHPV)
{

}

void QmlCommunicator::addPatient(QString name,QString age,QString sex,
                                        QString checkTime,
                                        QString identity
                                        ,QString phoneNum,
                                        QString hasCance,
                                        QString hasHPV)
{
    PatientInfo patient;
    patient.m_patientName=name;
    patient.m_patientAge=age;
    patient.m_patientId=identity;
    patient.m_patientPhone=phoneNum;
    patient.m_patientSex=sex;
    patient.m_patientState1=hasCance;
    patient.m_patientState2=hasHPV;
    patient.loginDate=checkTime;

}




