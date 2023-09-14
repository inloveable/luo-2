
#include "networkmanager.hpp"
#include "CoreFunctionality/checkbackgroundandmove.hpp"
#include"DataManager/datamanager.h"
#include"CoreFunctionality/cutpyramid.hpp"
#include"CoreFunctionality/medicaldetector.hpp"
#include "qdiriterator.h"
#include "qthreadpool.h"
#include "qtimer.h"
#include "vipswrapper.hpp"
#include<QThreadPool>
#include<glog/logging.h>
NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{
    DataManager::GetInstance()->setStages(5);
    connect(this,&NetworkManager::movingToNextStage,DataManager::GetInstance(),&DataManager::moveToNextStage);
}

void NetworkManager::login(QString account,QString password)
{


    bool result=DataManager::checkIfUserInDataBase(account,password);

    if(!result)
    {
        emit loginResult(false,"用户名或密码错误");
        return;
    }

    emit loginResult(true,"success");
}

//datamanager 已经hash了
void NetworkManager::startOpen(QString file,QString dst)
{
    currentStatus=ANALIZE_STATUS::WAIT;
    emit movingToNextStage(true);

    preCutBuffer=dst;
    CutPyramid* cutPtramid=new CutPyramid{file,dst,1024,0,
          [manager=this](int sucess)
            {
                if(sucess==0)
                {
                //switch to the correct status when task is done.
                  manager->currentStatus=ANALIZE_STATUS::OPEN;
                }else {
                  manager->currentStatus=ANALIZE_STATUS::ERROR;
                  manager->errormessage="error when Open image(cut)";
                }
                manager->checkStatus();
                manager->cutPtramid=nullptr;
           }};

    QThreadPool::globalInstance()->start(cutPtramid);
    QThreadPool::globalInstance()->start([](){
        //删除input时会将input文件夹一起删除，所以需要重新创建
         Utility::removeDir(DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::PYRAMID_INPUT));
         QDir dir;
         dir.mkpath(DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::PYRAMID_INPUT));
    });
}

void NetworkManager::preparing()
{
    buildDirectoryInPostTiff();
    auto filter=new CheckBackgroundAndMove(preCutBuffer+"_files",
                                             QString{DataManager::GetInstance()
                                                 ->getDirectoryPath(DataManager::DirectoryPath::POST_TIFF)+"/"+
                                                                    DataManager::GetInstance()->getHashMark()+"_files"},
                                             DataManager::GetInstance()
                                                 ->getDirectoryPath(DataManager::DirectoryPath::PYRAMID_INPUT),
                                             [manager=this](int result)
                                             {
                                                 manager->currentStatus=ANALIZE_STATUS::FILITER;
                                                 manager->checkStatus();
                                                 manager->analize=nullptr;
                                             });


    QThreadPool::globalInstance()->start(filter);
}


void NetworkManager::buildDirectoryInPostTiff()
{
    auto& data=*DataManager::GetInstance();
    const QString postTiff=data.getDirectoryPath(DataManager::DirectoryPath::POST_TIFF);
    const QString pyramidDir{data.getHashMark()+"_files"};

    QDir dstDir{postTiff};

    QDir dir{preCutBuffer+"_files"};
    QVector<int> layers;
    dstDir.mkdir(pyramidDir);
    for(auto&& info:dir.entryInfoList())
    {
        if(info.fileName()=="."||info.fileName()=="..")
        {
            continue;
        }
        if(info.isDir())
        {
            layers<<info.fileName().toInt();
        }
        if(info.isFile())
        {
           //move vips-properties.xml
            QFile f{info.absoluteFilePath()};
            f.copy(data.getDirectoryPath(DataManager::DirectoryPath::POST_TIFF)
                   +"/"+pyramidDir+"/"+f.fileName());
        }
    }



    for(auto&& l:layers)
    {
        dstDir.mkdir(pyramidDir+"/"+QString::number(l));
    }

    dir.cd("..");

    const QString dziFileName=data.getHashMark()+".dzi";
    QFile dziFile{dir.absolutePath()+"/"+dziFileName};
    dziFile.copy(data.getDirectoryPath(DataManager::DirectoryPath::POST_TIFF)+"/"+dziFileName);

}

void NetworkManager::checkStatus()
{
    if(aboutStatus==ANALIZE_STATUS::ANALIZE
        &&currentStatus==ANALIZE_STATUS::FILITER)
    {
        analizing();
        emit movingToNextStage();
        emit endStartOpen();
        aboutStatus=ANALIZE_STATUS::WAIT;
    }
    else if(currentStatus==ANALIZE_STATUS::OPEN)
    {
        emit letUiGeneratePreHtml();
        preparing();//filter
        LOG(INFO)<<"filtering...";
        emit movingToNextStage();
    }
    else if(currentStatus==ANALIZE_STATUS::ANALIZE)
    {
        LOG(INFO)<<"anlize finished";
        movingPost();
        emit movingToNextStage();
    }
    else if(currentStatus==ANALIZE_STATUS::RECOVER_PYRAMID)
    {
        LOG(INFO)<<"to post tiff finished";
        emit movingToNextStage();
        emit letUiGeneratePostHtml();
    }
    else if(currentStatus==ANALIZE_STATUS::ERROR)
    {
        LOG(WARNING)<<"BACKEND ERROR,error message:"<<errormessage.toStdString();
        emit backEndError(errormessage);
    }
}

void NetworkManager::analizing()
{
    LOG(INFO)<<"anlizing。。。";
    MedicalDetector* analize;

#if !ENABLE_MEDICAL_DETECTOR
    analize=new MedicalDetector(
        [manager=this](int)
        {
           manager->currentStatus=ANALIZE_STATUS::ANALIZE;
           manager->checkStatus();
        });
#elif
    analize=new RealMedicalDetector([manager=this](int){
        manager->currentStatus=ANALIZE_STATUS::ANALIZE;
        manager->checkStatus();
    });
#endif

    QThreadPool::globalInstance()->start(analize);
}

void NetworkManager::movingPost()
{
    const QString src=DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::PYRAMID_OUTPUT);
    const QString postTiff=DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::POST_TIFF);
    auto generate=new GeneratePostImage(src,postTiff,[manager=this](int){
        manager->currentStatus=ANALIZE_STATUS::RECOVER_PYRAMID;
        manager->checkStatus();//17@16_22.jpg
    });

    QThreadPool::globalInstance()->start(generate);
}

void NetworkManager::clearCache()
{
    LOG(INFO)<<"NetworkManager clearning cache";
    QDir dir1{"appCache"};

    dir1.removeRecursively();
    auto& Data=*DataManager::GetInstance();

    QDir dir;
    dir.mkdir("appCache");
    dir.mkdir("appCache/pyramidBuffer");

    dir.mkdir(Data.getDirectoryPath(DataManager::DirectoryPath::LOG));//all should be held constant as DataManager;
    dir.mkdir(Data.getDirectoryPath(DataManager::DirectoryPath::PYRAMID_INPUT));
    dir.mkdir(Data.getDirectoryPath(DataManager::DirectoryPath::PYRAMID_OUTPUT));
    dir.mkdir(Data.getDirectoryPath(DataManager::DirectoryPath::POST_TIFF));
    dir.mkdir(Data.getDirectoryPath(DataManager::DirectoryPath::DEFAULT_DOCUMENTS));
    dir.mkdir(Data.getDirectoryPath(DataManager::DirectoryPath::PRE_TIFF));
}

NetworkManager::~NetworkManager()
{
    if(analize!=nullptr)
    {
        analize->setGeneralFlag();
    }
    if(cutPtramid!=nullptr)
    {
        cutPtramid->setGeneralFlag();
    }
}




