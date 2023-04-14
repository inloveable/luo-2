
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
    void  onGeneratePreHtml();
    void  onGeneratePostHtml();


protected:
    bool eventFilter(QObject* obj,QEvent*event) override;

 signals:

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void updateTargetNum(int);



    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::MainWindow ui;

    QQmlApplicationEngine* engine;
    QmlCommunicator* communicator;

    QThread* backEndThread=nullptr;
    NetworkManager* network=nullptr;
    void connectNetworkSignals();

    void initUi();

    void uiStartOpen(bool start);
    void uiStartAnalize(bool start);

    std::vector<QPushButton*> buttons;



};

