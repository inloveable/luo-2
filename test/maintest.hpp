
#pragma once

#include "vips/vips.h"
#include <gtest/gtest.h>
#include"../vipswrapper.hpp"

#include<QDir>
class MainTest : public ::testing::Test
{
public:
    MainTest();
    ~MainTest(){};
protected:
    virtual void SetUp() override{
        QDir dir{"buffer"};
        Utility::calcuBegin();
        dir.removeRecursively();
        qDebug()<<"Qt remove dir cost:"<<Utility::calcuEnd();
        vips_init("TestDzSave");
    };
    virtual void TearDown() override{
        vips_shutdown();
        QDir dir{"buffer"};
        Utility::calcuBegin();
        //dir.removeRecursively();
        qDebug()<<"Qt remove dir cost:"<<Utility::calcuEnd();
    };


private:
};

