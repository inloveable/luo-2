


#include"vips/vips.h"
#include "mainwindow.hpp"
#include "qapplication.h"
#include <QGuiApplication>
#include<QDir>

#include<glog/logging.h>
#include"datamanager.h"

void createDirectories(QApplication& a);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setWindowIcon(QIcon{"qrc:/resources/images/logo.ico"});

    auto& Data=*DataManager::GetInstance();


    google::InitGoogleLogging(argv[0]);
    google::EnableLogCleaner(3);
    google::SetLogFilenameExtension(".txt");
    createDirectories(a);

    FLAGS_alsologtostderr = 1;
    FLAGS_log_dir = Data.getDirectoryPath(DataManager::DirectoryPath::LOG).toStdString();
    FLAGS_colorlogtostderr = true;//是否启用不同颜色显示(如果终端支持)

    Data.init();//have to init after QApplication created;
    vips_init(argv[0]);

    LOG(INFO)<<"current log dir:"<<Data.getDirectoryPath(DataManager::DirectoryPath::LOG).toStdString();


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


void createDirectories(QApplication& a)
{

    auto& Data=*DataManager::GetInstance();

    QDir dir;

    dir.mkdir("appCache");
    dir.mkdir("appCache/pyramidBuffer");

    dir.mkdir(Data.getDirectoryPath(DataManager::DirectoryPath::LOG));//all should be held constant as DataManager;
    dir.mkdir(Data.getDirectoryPath(DataManager::DirectoryPath::PYRAMID_INPUT));
    dir.mkdir(Data.getDirectoryPath(DataManager::DirectoryPath::PYRAMID_OUTPUT));
    dir.mkdir(Data.getDirectoryPath(DataManager::DirectoryPath::POST_TIFF));
    dir.mkdir(Data.getDirectoryPath(DataManager::DirectoryPath::DEFAULT_DOCUMENTS));


}
