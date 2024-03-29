

#include"vips/vips.h"

#include "vipswrapper.hpp"
#include <QDir>
#include <chrono>
#include <iostream>
#include <qDebug>
#include<QCryptographicHash>

std::chrono::time_point<std::chrono::high_resolution_clock> Utility::begin;
std::chrono::time_point<std::chrono::high_resolution_clock> Utility::end;

VipsWrapper::VipsWrapper(QObject *parent)
    : QObject{parent}
{

}

int VipsWrapper::dzSave(QString picture,QString dst,int tilesize,int overlap)
{
    VipsImage *image = vips_image_new_from_file(picture.toStdString().c_str(), NULL);
  //  const double background[3]{255,255,255};
  //  auto array=vips_array_double_new(background,3);//potential memory leak
    if(image==NULL)
    {
        return -2;
    }
    std::cerr<<"vip error:"<<vips_error_buffer();
    std::cerr<<"imageFileLength: "<<image->length<<std::endl;
    auto ret=vips_dzsave(image,
                           dst.toStdString().c_str(),"tile_size",tilesize,
                           //"background",*array,
                           "overlap",overlap,
                           "suffix",".jpg[background=255,Q=90]",
                           NULL);
    std::cerr<<"vip error:"<<vips_error_buffer();

    g_object_unref(image);


    return ret;
}

qint64 Utility::removeDir(const QString &dir)
{

    QDir directory{dir};
    if (!directory.exists()) {
        return 0;
    }  
    auto begin=std::chrono::high_resolution_clock::now();
    directory.removeRecursively();
    auto end=std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast
        <std::chrono::milliseconds>(end-begin).count();
}


void Utility::calcuBegin()
{
    begin=std::chrono::high_resolution_clock::now();
}

qint64 Utility::calcuEnd()
{
    auto end=std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast
                      <std::chrono::milliseconds>(end-begin).count();
}


QString Utility::hashStringMd5(const QString& hashed)
{
    QByteArray hash = QCryptographicHash::hash(hashed.toUtf8(), QCryptographicHash::Md5);
    return hash;
}

bool VipsWrapper::isBackground(const QString& picture)
{

    VipsImage *image = vips_image_new_from_file(picture.toStdString().c_str(), NULL);
    std::cerr<<"vip error:"<<vips_error_buffer();
    if (image == NULL) {

    }
    VipsImage *gray = vips_image_new ();
    std::cerr<<"vip error:"<<vips_error_buffer();
    qDebug()<<"color trans:"<< vips_colourspace(image, &gray, VIPS_INTERPRETATION_B_W, NULL);
    std::cerr<<"vip error:"<<vips_error_buffer();

    double mean=0, stddev=0;



        qDebug()<<"avg:"<<vips_avg(gray,&mean);
        std::cerr<<"vip error:"<<vips_error_buffer();
        qDebug()<<"dev:"<<vips_deviate(gray,&stddev);
        std::cerr<<"vip error:"<<vips_error_buffer();

    g_object_unref(image);


    qDebug()<<"图像标准差："<<stddev<<" 平均值:"<<mean;


    g_object_unref(gray);
    // 判断灰度图像是否是背景图像
    if (stddev < 10.0 && mean > 200.0) {
        return true;
    } else {
        return false;
    }

}
