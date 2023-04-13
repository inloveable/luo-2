
#include "medicaldetector.hpp"
#include "cutpyramid.hpp"
#include"datamanager.h"
#include "qdiriterator.h"
#include<glog/logging.h>
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


