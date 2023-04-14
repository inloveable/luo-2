
#pragma once




#include <QObject>

//TODO:Maybe access login info remotely
//which is why called networkmanager
//and this should be making total domination of anylizing job
//DataManager runs in mainThread;
class CutPyramid;
class MedicalDetector;
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

    ~NetworkManager();

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
public:
    enum class ANALIZE_STATUS{WAIT=0,OPEN=1,FILITER=2,ANALIZE=3,
                                RECOVER_PYRAMID=4,RECOVER_TIFF=5,DONE=6,ERROR=7};

public slots:
    void startOpen(QString file,QString dst);

    void startAnalize(){
        aboutStatus=ANALIZE_STATUS::ANALIZE;
        checkStatus();
    };


    void clearCache();

signals:
    void loginResult(bool result,QString message);

    void movingToNextStage(bool reset=false);

    void endStartOpen();

    void letUiGeneratePreHtml();
    void letUiGeneratePostHtml();

    void quit();

    void backEndError(QString message);

private:
    //信号更新stage，timer轮询
    ANALIZE_STATUS currentStatus=ANALIZE_STATUS::WAIT;

    void checkStatus();


    ANALIZE_STATUS aboutStatus=ANALIZE_STATUS::WAIT;

    void preparing();
    void analizing();
    void movingPost();

    QString errormessage;
    QString preCutBuffer;

    MedicalDetector* analize=nullptr;
    CutPyramid*      cutPtramid=nullptr;

    void buildDirectoryInPostTiff();

    bool generalFlag=false;

};

