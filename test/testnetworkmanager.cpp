
#include "testnetworkmanager.hpp"
#include"../CoreFunctionality/cutpyramid.hpp"
#include "qdebug.h"
#include "qthreadpool.h"
#include "gtest/gtest.h"
TestNetworkManager::TestNetworkManager()
{

}

TEST_F(TestNetworkManager,testTemplate)
{
    CutPyramid* pyramid=new CutPyramid("./test.svs","buffer/22222222",1024,0,[=](int val)
                                         {
                                             qDebug()<<"called back:"<<val;
                                         });

    QThreadPool::globalInstance()->start(pyramid);
    ASSERT_NO_THROW(QThreadPool::globalInstance()->waitForDone());
}

