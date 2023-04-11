
#pragma once

#include"vips/vips.h"
#include"../vipswrapper.hpp"
#include "qdir.h"

#include <QObject>
#include<gtest/gtest.h>

class TestNetworkManager : public ::testing::Test
{
public:
    TestNetworkManager();

    virtual void SetUp() override{
        QDir dir{"buffer"};
        Utility::calcuBegin();
        dir.removeRecursively();
        qDebug()<<"Qt remove dir cost:"<<Utility::calcuEnd();
        vips_init("TestDzSave");
    };
    virtual void TearDown() override{
        vips_shutdown();
    };



};

