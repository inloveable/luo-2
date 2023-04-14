
#include "medicaldetector.hpp"
#include "cutpyramid.hpp"
#include"datamanager.h"
#include "qdiriterator.h"
#include<glog/logging.h>
#include <utility>


#if ENABLE_MEDICAL_DETECTOR
#include "MedicalForeign.h"
#include <qmessagebox.h>
#include "detect_api.h"
#pragma comment(lib, "MedicalDetector.lib")
#endif

MedicalDetector::MedicalDetector(std::function<void(int)>&& callback):
    Runnables{std::forward<std::function<void(int)>>(callback)}
{
    Runnables::takeArgs(0);
}



void MedicalDetector::run()
{
    const QString src=DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::PYRAMID_INPUT);
    const QString dst=DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::PYRAMID_OUTPUT);

    QDirIterator iterator{src};

    while(iterator.hasNext()&&!generalFlag)
    {
        QString next=iterator.next();

        if(next.endsWith('.')||next.endsWith(".."))
        {
            continue;
        }
        QString picture{next.section("/",-1)};

        QFile file{next};

        file.copy(dst+"/"+picture);
    }
}


GeneratePostImage::GeneratePostImage(const QString& src,const QString& dst,std::function<void(int)>&& callback)
    :Runnables{std::forward<decltype(callback)>(callback)},src{src},dst{dst}
{
    Runnables::takeArgs(0);
}

void GeneratePostImage::run()
{
    QDirIterator dirIter{DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::PYRAMID_OUTPUT)};
    QString fileName;
    const QString postTiff=DataManager::GetInstance()->getDirectoryPath(DataManager::DirectoryPath::POST_TIFF);
    while(dirIter.hasNext())
    {
        fileName=dirIter.next();
        QFile file{fileName};

        QString pName=fileName.section("/",-1);

        auto list=pName.split("@");
        qDebug()<<"NEXT:"<<fileName;
        if(list.size()<2){
            LOG(WARNING)<<"something mix up in output dir continuing:"<<fileName.toStdString();
            continue;
        }
        auto layer=list[0];
        auto picture=list[1];

        qDebug()<<"COPYED FILENAME:"<<postTiff+"/"+DataManager::GetInstance()->getHashMark()+"_files/"+layer+"/"+picture;
        qDebug()<<"copy result:"<<file.copy(postTiff+"/"+DataManager::GetInstance()->getHashMark()+"_files/"+layer+"/"+picture);
        file.remove();
    }

}

RealMedicalDetector::RealMedicalDetector(std::function<void(int)>&& callback)
    :Runnables{std::forward<decltype(callback)>(callback)}

{

}

void RealMedicalDetector::run()
{
   /*
    *
    *   MedicalForeign* medicalFor = new MedicalForeign();
    QString yolomodeldir = QString("%1/data/").arg(QApplication::applicationDirPath());
    QString yolosort = QString("%1/data/coco1.names").arg(QApplication::applicationDirPath());
    QString srcdir = QString("%1/data/input").arg(QApplication::applicationDirPath());
    QString outdir = QString("%1/data/output").arg(QApplication::applicationDirPath());
    int detectpercount = 1;
    float threshold = 0.5;
    float nmsthreshold = 0.5;
    float objthreshold = 0.5;
    int trygpu = 1;
    int framerate = 1;
    int encoder = 5;
    int modelindex = 0;
    medicalFor->getParamFromClient(yolomodeldir, yolosort, srcdir, outdir, detectpercount, threshold, nmsthreshold, objthreshold, trygpu, framerate, encoder, modelindex);
    */
}


