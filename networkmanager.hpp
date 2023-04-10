
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




    /*
             libvips cutting
//打开分析图片----------------->
---->预处理(将空白图片与待分析图片分流,空白图片直接到/appCache/postPyramidBuffer(按照金字塔格式)
，待分析图片到/appCache/pyramidBuffer/input)
-------------------->MedicalDetector()---->将ouput中的分析后图片再放到金字塔文件夹下---->提示分析完成
-------------------->生成新html---->显示在结果webview
自动地将最底层的图片result.tiff---------->客户点击保存就让他等待

*/

    enum class ANALIZE_STATUS{WAIT,OPEN,FILITER,ANALIZE,RECOVER_PYRAMID,RECOVER_TIFF,DONE,ERROR};


    void startOpen(QString file,QString dst);
    void startAnalize();



    void onStartOpenFinished();
signals:
    void loginResult(bool result,QString message);


private:
    //信号更新stage，timer轮询
    ANALIZE_STATUS currentStatus=ANALIZE_STATUS::WAIT;
    QTimer* tryTimer=nullptr;


    bool requestAnalize=false;

};

