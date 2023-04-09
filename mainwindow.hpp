
#pragma once

#include "./ui_mainwindow.h"
#include "qtmetamacros.h"


#include <QMainWindow>

class NetworkManager;
class QQmlApplicationEngine;
class QmlCommunicator;
class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //qml slots

    void  reEnter();


protected:
    bool eventFilter(QObject* obj,QEvent*event) override;

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

