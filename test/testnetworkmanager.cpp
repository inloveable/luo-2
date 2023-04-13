
#include "testnetworkmanager.hpp"
#include"../CoreFunctionality/cutpyramid.hpp"
#include"../CoreFunctionality/checkbackgroundandmove.hpp"
#include "qdebug.h"
#include "qthreadpool.h"
#include "gtest/gtest.h"

TestNetworkManager::TestNetworkManager()
{

}




TEST_F(TestNetworkManager,testTemplate)
{
/*
    CutPyramid* pyramid=new CutPyramid("./testFiles/test1.svs","buffer/22222222",1024,0,[=](int val)
                                        {
                                             qDebug()<<"called back:"<<val;
                                         });

    QThreadPool::globalInstance()->start(pyramid);
    ASSERT_NO_THROW(QThreadPool::globalInstance()->waitForDone());
*/
    auto background=new CheckBackgroundAndMove("./buffer/22222222_files/",
                                                 "./buffer/postTiff/22222222_files","./buffer/moved/input",[](int val)
                                                 {
                                                     qDebug()<<val;
                                                 });
    QThreadPool::globalInstance()->start(background);
    ASSERT_EQ(true,QThreadPool::globalInstance()->waitForDone());
}


TEST_F(TestNetworkManager,testBackground_Outter)
{

}


