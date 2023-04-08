
#include "maintest.hpp"
#include "gtest/gtest.h"

MainTest::MainTest()
{

}

/*
TEST_F(MainTest,test1)
{
    ASSERT_EQ(0,VipsWrapper::dzSave("./test.svs","./buffer/20210000"));
}*/

TEST_F(MainTest,test2)
{
    ASSERT_EQ(0,VipsWrapper::dzSave("./test1.svs","./buffer/test2"));
}


/*
TEST(UtilityTest,removeDir)
{
    ASSERT_LE(2,Utility::removeDir("buffer"));
}
*/

