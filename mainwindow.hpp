
#pragma once

#include "./ui_mainwindow.h"
#include "qtmetamacros.h"


#include <QMainWindow>

class NetworkManager;
class QQmlApplicationEngine;

class MainWindow;
class QmlCommunicator:public QObject
{
    Q_OBJECT
public:
    QmlCommunicator(MainWindow* m,QObject* parent=nullptr):QObject{parent}{mainwindow=m;};
    Q_INVOKABLE void login(QString account,QString password);
    void onLoginResult(bool result,QString message);

    Q_INVOKABLE QString getCurrentDoctorName();

    Q_INVOKABLE int tryRegister(QString account,QString password,QString doctorName);
    Q_INVOKABLE int intoTCTAnalize();
    Q_INVOKABLE void setCurrentPatient(QString name,QString age,QString sex,
                                    QString checkTime,
                                    QString identity
                                   ,QString phoneNum,
                                    QString hasCance,
                                    QString hasHPV);
    //添加病人和病人检查日志是两个不同的概念
    Q_INVOKABLE void addPatient(QString name,QString age,QString sex,
                                    QString checkTime,
                                    QString identity
                                    ,QString phoneNum,
                                    QString hasCance,
                                    QString hasHPV);

    Q_INVOKABLE void showMassageBox(QString message);

signals:
    void loginResult(bool result,QString message);
    void tryLogin(QString account,QString password);

    int askRegister(const QString& account,const QString& password
                    ,const QString& docId,const QString& picturePath);

    //reactivates LoginWindow
    void showMainArea();
private:

    MainWindow* mainwindow;
};

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    //qml slots

    void  reEnter();

 signals:

private:
    Ui::MainWindow ui;

    QQmlApplicationEngine* engine;
    QmlCommunicator* communicator;

    QThread* backEndThread=nullptr;
    NetworkManager* network=nullptr;
    void connectNetworkSignals();

    void initUi();



};

