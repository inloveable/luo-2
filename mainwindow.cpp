
#include "mainwindow.hpp"

#include<QQmlApplicationEngine>
#include<QQuickWindow>
#include"networkmanager.hpp"
#include"datamanager.h"
#include "qfilesystemwatcher.h"
#include "qmlcommunicator.hpp"
#include "qnamespace.h"
#include "qqml.h"
#include <QThread>
#include<QButtonGroup>
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
        //some trival kernel exception comes up
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

    ui.radioButton->setAutoExclusive(false);
    ui.radioButton_2->setAutoExclusive(false);
    ui.radioButton_3->setAutoExclusive(false);
    ui.radioButton_4->setAutoExclusive(false);

    ui.radioButton->installEventFilter(this);
    ui.radioButton_2->installEventFilter(this);
    ui.radioButton_3->installEventFilter(this);
    ui.radioButton_4->installEventFilter(this);


    auto group1=QButtonGroup(this);
    group1.addButton(ui.radioButton);
    group1.addButton(ui.radioButton_2);

    auto group2=QButtonGroup(this);
    group2.addButton(ui.radioButton_3);
    group2.addButton(ui.radioButton_4);


}

void MainWindow::reEnter()
{
   //TODO:
   //refresh ui with newly changed data,or may flush for nex Analize

    LOG(WARNING)<<"reentering";
    const auto& currentPatient=DataManager::GetInstance()->getCurretnPatient();
    if(currentPatient==nullptr)
    {
        return;
    }
    LOG(INFO)<<"current Patient not null,populating...";
    ui.label_3->setText(currentPatient->m_patientName);
    ui.label_5->setText(currentPatient->m_patientAge);
    ui.label_7->setText(currentPatient->m_patientSex);
    ui.label_10->setText(currentPatient->m_patientPhone);
    ui.label_12->setText(currentPatient->m_patientId);

    if(currentPatient->m_patientState1=="有")
    {
        ui.radioButton->setChecked(true);

    }
    else if(currentPatient->m_patientState1=="无")
    {

        ui.radioButton_2->setChecked(true);
    }

    if(currentPatient->m_patientState2=="有")
    {
        ui.radioButton_3->setChecked(true);

    }
    else if(currentPatient->m_patientState2=="无")
    {
        ui.radioButton_4->setChecked(true);
    }
}
//make radio button ininteractive
bool MainWindow::eventFilter(QObject* obj,QEvent* event)
{
    if(obj==ui.radioButton||obj==ui.radioButton_2
        ||obj==ui.radioButton_3||ui.radioButton_4)
    {
        if(event->type()==QEvent::MouseButtonPress
            ||event->type()==QEvent::MouseButtonDblClick
            ||event->type()==QEvent::MouseButtonRelease)
        {
            return true;
        }
    }
    return false;
}






