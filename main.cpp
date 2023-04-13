


#include"vips/vips.h"
#include "mainwindow.hpp"
#include "qapplication.h"
#include <QGuiApplication>
#include<QDir>

#include <cstddef>
#include<glog/logging.h>
#include"datamanager.h"
#include<iostream>
void SinalHandle(const char*,std::size_t len);
void createDirectories(QApplication& a);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setWindowIcon(QIcon{"qrc:/resources/images/logo.ico"});

    auto& Data=*DataManager::GetInstance();

    Data.init();//have to init after QApplication created;
    createDirectories(a);
    google::InitGoogleLogging(argv[0]);

    google::EnableLogCleaner(3);
    google::SetLogFilenameExtension(".txt");


    FLAGS_alsologtostderr = 1;
    FLAGS_minloglevel = google::GLOG_INFO;
    FLAGS_log_dir = Data.getDirectoryPath(DataManager::DirectoryPath::LOG).toStdString();
    FLAGS_colorlogtostderr = true;//是否启用不同颜色显示(如果终端支持)
    google::InstallFailureSignalHandler();
    google::InstallFailureWriter(&SinalHandle);


    vips_init(argv[0]);
    MainWindow w;
    QObject::connect(&a,&QApplication::aboutToQuit,[&](){
        Data.Destory();

    });
    //w.show();
    a.exec();

    vips_shutdown();
    google::ShutdownGoogleLogging();

    //exit;


    return 0;
}

void SinalHandle(const char* message,std::size_t len)
{
    std::cerr<<"glog error:";
    std::cerr.write(message,len);
    std::cerr<<"\n";
}
void createDirectories(QApplication& a)
{
    LOG(INFO)<<"entering create dir";
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
    LOG(INFO)<<"LEAing create dir";


}
