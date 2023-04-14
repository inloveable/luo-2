
#include "mainwindow.hpp"

#include<QQmlApplicationEngine>
#include<QQuickWindow>
#include "modifycheckinfodialog.hpp"
#include"networkmanager.hpp"
#include"datamanager.h"
#include "qdatetime.h"
#include "qmessagebox.h"
#include "qmlcommunicator.hpp"
#include "qnamespace.h"
#include "qobjectdefs.h"
#include "qqml.h"
#include <QThread>
#include<QButtonGroup>
#include<QMessageBox>
#include <exception>
#include<QResource>
#include<QFileDialog>

#include<glog/logging.h>

#ifdef DEBUG
#include<QFileSystemWatcher>
#endif
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    LOG(INFO)<<"ENTER ING aminwindow construct";
    communicator=new QmlCommunicator(this,this);

    setWindowTitle("TCT分析");
     LOG(INFO)<<"ENTERING QML CONSTRUCT";
    qmlRegisterSingletonInstance("myCpp",1,0,"MainWindow",communicator);
    try{
        engine=new QQmlApplicationEngine(QUrl{"qrc:/qml/LoginWindow.qml"});
    }
    catch(std::exception& ex){
        //some trival kernel exception comes up
        LOG(INFO)<<"QML trigged kernal exceptionL"<<ex.what();
    }



    LOG(INFO)<<"LEAVING QML CONSTRUCT";

    network=new NetworkManager();
    backEndThread=new QThread;
    connectNetworkSignals();
    backEndThread->start();
    network->moveToThread(backEndThread);




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
    initUi();
     LOG(INFO)<<"leavING aminwindow construct";
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
    connect(network,&NetworkManager::letUiGeneratePreHtml,this,&MainWindow::onGeneratePreHtml);
    connect(network,&NetworkManager::letUiGeneratePostHtml,this,&MainWindow::onGeneratePostHtml);
}


void MainWindow::initUi()
{
    QAction* goBack=new QAction{"返回主界面",this};
    ui.menu->addAction(goBack);
    QAction* clearCache=new QAction{"清除缓存",this};
    clearCache->setToolTip("清除缓存的图片、文档(仅当软件过大时使用)");
    ui.menu->addAction(clearCache);

    connect(goBack,&QAction::triggered,this,[=]()
            {
               emit communicator->showMainArea();
               this->close();
            });

    connect(clearCache,&QAction::triggered,network,&NetworkManager::clearCache);
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

    buttons.push_back(ui.pushButton);
    buttons.push_back(ui.pushButton_7);
    buttons.push_back(ui.pushButton_9);
    buttons.push_back(ui.pushButton_4);
    buttons.push_back(ui.pushButton_6);
    buttons.push_back(ui.pushButton_3);


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



    ui.widget->resetInput();

    ui.label_8->setText("医院:"+DataManager::GetInstance()->hospital);
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



void MainWindow::uiStartOpen(bool start)
{
    for(auto&& button:buttons)
    {
        button->setEnabled(!start);
    }
}



void MainWindow::on_pushButton_4_clicked()
{
    QString svsFile= QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择图片"),
                                           QApplication::applicationDirPath()+"/InputData/", tr("*.svs *.tiff *.tif"));
    if(svsFile=="")
    {
        return;
    }

    auto& Data=*DataManager::GetInstance();
    Data.makeAnalizeMarkIdentical();

    QMetaObject::invokeMethod(network,"startOpen",Q_ARG(QString,svsFile),
                              Q_ARG(QString,Data.getDirectoryPath(DataManager::DirectoryPath::PRE_TIFF)
                                                 +"/"+Data.getHashMark()));
    DataManager::GetInstance()->controller->start();
    uiStartOpen(true);
}


void MainWindow::onGeneratePreHtml()
{
    //可优化的空间:width and height might should also be changed
    //which can be read from:/cutted/xxxx_tiles/vips_properties.xml

    uiStartOpen(false);

    LOG(INFO)<<"generating pre html";
    QFile file(DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::PRE_HTML_PATH)+"/test.html");//模板Html
    file.open(QIODevice::ReadOnly);   
    QString str = file.readAll();
       // 处理 HTML
     str.replace("./testlolNew_files/",QString(
                                       DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::PRE_TIFF)+"/"+
                                       DataManager::GetInstance()->getHashMark()+"_files"+"/"));
    file.setFileName(DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::PRE_HTML_PATH)+"/Newtest.html");
    file.remove();
    file.open(QIODevice::WriteOnly);
    file.write(str.toLocal8Bit());
    file.close();

    ui.widget_2->setUrl(QUrl{file.fileName()});

}

void MainWindow::onGeneratePostHtml()
{
    LOG(INFO)<<"generating post html";
    QFile file(DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::POST_HTML_PATH)+"/test.html");//模板Html
    file.open(QIODevice::ReadOnly);
    QString str = file.readAll();
        // 处理 HTML
    str.replace("./testlolNew_files/",QString(
                                           DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::POST_TIFF)+"/"+
                                           DataManager::GetInstance()->getHashMark()+"_files"+"/"));
    file.setFileName(DataManager::GetInstance()
                         ->getDirectoryPath(DataManager::DirectoryPath::POST_HTML_PATH)+"/Newtest.html");
    file.remove();
    file.open(QIODevice::WriteOnly);
    file.write(str.toLocal8Bit());
    file.close();

    ui.widget_3->setUrl(QUrl{file.fileName()});
}

void MainWindow::on_pushButton_6_clicked()
{
    QMetaObject::invokeMethod(network,"startAnalize");
}


void MainWindow::on_pushButton_9_clicked()
{
    auto image=ui.widget_3->screenShot();
    auto& currentPatient=DataManager::GetInstance()->getCurretnPatient();
    const QString path=DataManager::GetInstance()->
                         getDirectoryPath(DataManager::DirectoryPath::DEFAULT_DOCUMENTS)+"/"
                         +currentPatient->m_patientName+QDateTime::currentDateTime().toString("_hh.mm.ss.jpg");
    image.save(path);
    QMessageBox::information(this,"提示","截图已保存至"+path);
}

//generate report
void MainWindow::on_pushButton_3_clicked()
{
    QStringList pics;
    while(pics.size()<2)
    {
        int originalSize=pics.size();
        pics<<QFileDialog::getOpenFileNames(this,QString("选择报告图片"),
                                                        DataManager::GetInstance()->
                                                    getDirectoryPath(
                                                  DataManager::DirectoryPath::DEFAULT_DOCUMENTS),
                                              "Images (*.png *.xpm *.jpg)");
        if(originalSize==pics.size())
        {
            return;
        }
    }
    DataManager::GetInstance()->printReport(pics[0],pics[1],
                                            ui.widget->getIllAbstract(),ui.widget->getCheckResult());
}


void MainWindow::on_pushButton_clicked()
{
    auto button=QMessageBox::question(this,"是否保存?","是否保存检查结果?");
    if(button!=QMessageBox::Yes)
    {
        return;
    }
    CheckInfo info;
    info.illAbstract=ui.widget->getIllAbstract();
    info.CheckResult=ui.widget->getCheckResult();
    DataManager::GetInstance()->addCheckInfo(info);
    QMessageBox::information(this,"提示","保存检查信息完成");
}




void MainWindow::on_pushButton_2_clicked()
{
    ModifyCheckInfoDialog* dialog=new ModifyCheckInfoDialog(this);
    connect(dialog,&QDialog::accepted,this,&MainWindow::reEnter);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}


void MainWindow::on_pushButton_5_clicked()
{

    if(ui.pushButton_5->text()=="全屏")
    {
        ui.splitter->saveMyState();
        ui.splitter_2->saveMyState();
        ui.splitter->setHidden(ui.widget_4);
        ui.splitter_2->setRightHidden(ui.widget_6);

        ui.pushButton_5->setText("恢复");
    }
    else if(ui.pushButton_5->text()=="恢复")
    {
        ui.splitter->restoreMyState();
        ui.splitter_2->restoreMyState();
        ui.pushButton_5->setText("全屏");
    }

}

void MainWindow::updateTargetNum(int val)
{
    ui.targetLabel->setText(QString("目标个数:%1").arg(val));
}


void MainWindow::on_pushButton_8_clicked()
{
    if(ui.pushButton_8->text()=="全屏")
    {
        ui.splitter->saveMyState();
        ui.splitter_2->saveMyState();
        ui.splitter->setDownHidden(ui.widget_4);
        ui.splitter_2->setRightHidden(ui.widget_6);

        ui.pushButton_8->setText("恢复");
    }
    else if(ui.pushButton_8->text()=="恢复")
    {
        ui.splitter->restoreMyState();
        ui.splitter_2->restoreMyState();
        ui.pushButton_8->setText("全屏");
    }
}

