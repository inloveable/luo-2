
#include "vips/vips.h"
#include "qapplication.h"

#include <gtest/gtest.h>
#include<QDir>
int main(int argc, char *argv[])
{
    QApplication a{argc,argv};
    vips_init(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);
    int val=RUN_ALL_TESTS();
    vips_shutdown();
    return val;
}



TEST(single,s1)
{
    const QString preCutBuffer="./buffer/22222222";
    const QString postTiff="./buffer/postTiff";


    QDir dstDir{postTiff};


    //-------------------------------------------
    QDir dir{preCutBuffer+"_files"};
    //dir.setFilter(QDir::Filter::NoDotAndDotDot);
    QVector<int> layers;

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
            //f.copy(data.getDirectoryPath(DataManager::DirectoryPath::POST_TIFF)
                   //+"/"+pyramidDir+"/"+f.fileName());
            f.copy(postTiff+"/22222222_files/"+f.fileName());
        }
    }


    for(auto&& l:layers)
    {
        dstDir.mkdir("22222222_files/"+QString::number(l));
    }

    dir.cd("..");

    const QString dziFileName="22222222.dzi";
    QFile dziFile{dir.absolutePath()+"/"+dziFileName};
    dziFile.copy(postTiff+"/"+dziFileName);
}
