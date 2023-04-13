
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

    };
    virtual void TearDown() override{

    };


private:
};

