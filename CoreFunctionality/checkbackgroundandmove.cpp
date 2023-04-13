
#include "checkbackgroundandmove.hpp"
#include <mutex>
#include<opencv2/opencv.hpp>
#include"../vipswrapper.hpp"
#include "qdiriterator.h"
#include<QDirIterator>
#include"glog/logging.h"
#include"../defs.h"
using namespace cv;
CheckBackgroundAndMove::CheckBackgroundAndMove(const QString& src,
                                               const QString&backgroundDst,const QString& imageDst,
                                               std::function<void(int)>&& callback)
    :Runnables{std::forward<std::function<void(int)>>(callback)},src{src},imageDst{imageDst},
    backgroundDst{backgroundDst}

{

}

static bool checkIfPictureIsbackground(const cv::Mat& srcImage)
{
    Mat srcHsv;
    cvtColor(srcImage, srcHsv, COLOR_RGB2HSV);

    Mat white_mask;
    inRange(srcHsv, Scalar(0, 0, 221), Scalar(180, 30, 255), white_mask);

    int morph_size = 5;
    Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1,
                                                         2 * morph_size + 1),
                                        Point(morph_size, morph_size));
    Mat dst;
    morphologyEx(white_mask, dst, MORPH_OPEN, element, Point(-1, -1), 2);

    Mat white;
    morphologyEx(dst, white, MORPH_CLOSE, element, Point(-1, -1), 2);

    int temp = countNonZero(white);
    int resultValue = temp * 100 / (white.rows * white.cols);


    if(resultValue>92)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CheckBackgroundAndMove::run()
{

    std::mutex mu;
    std::condition_variable condition;
    std::vector<std::tuple<Mat,QString,bool>> data;


    bool flag=false;


    auto judge=[&]()
    {

        QDirIterator dirIter{src,
                             //QDir::NoDotAndDotDot,
                             QDirIterator::IteratorFlag::Subdirectories};
        QString next;
        PROFILE_BEGIN;
        while(dirIter.hasNext())
               //&&!generalFlag)
        {
            next=dirIter.next();
            if(!next.endsWith(".jpg"))
            {
                continue;
            }
            Mat srcImage=imread(next.toStdString());
            if(srcImage.empty())
            {
                LOG(WARNING)<<"null image:"<<next.toStdString();
            }

            bool result=checkIfPictureIsbackground(srcImage);
                std::unique_lock<std::mutex> guard{mu};

                data.push_back(std::tuple<Mat,QString,bool>{srcImage,
                                                              next,result});

            if(data.size()>100)
                condition.notify_one();

        }
        condition.notify_one();
        flag=true;
        PROGILE_END;
        LOG(INFO)<<"judge thread exit COST:"<<PROFILE_RESULT;
    };

    auto write=[&]()
    {
        std::vector<std::tuple<Mat,QString,bool>> local;
        local.reserve(200);
        for(;;)
        {
            if(flag&&data.size()==0)
            {
                LOG(INFO)<<"write thread exit";
                return;
            }
            {
                std::unique_lock<std::mutex> guard{mu};
                condition.wait(guard,[&](){return data.size()>0;});
                local=std::move(data);
            }
            for(auto&& l:local)
            {
                auto fileOrigin=std::get<1>(l);
                auto list=fileOrigin.split("/");
                QString picture=list[list.size()-1];
                QString layer=list[list.size()-2];
                if(std::get<2>(l)==false)
                {
                    QString dst{this->imageDst+"/"+layer+"@"+picture};
                    imwrite(dst.toStdString(),std::get<0>(l));
                }
                else
                {
                    QString dst{this->backgroundDst+"/"+layer+"/"+picture};
                    imwrite(dst.toStdString(),std::get<0>(l));
                }

            }
            local.clear();


        }        
    };

    std::thread t1{judge};
    std::thread t2{write};

    t1.join();
    t2.join();

    Runnables::takeArgs(true);
}

