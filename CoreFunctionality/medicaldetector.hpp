
#pragma once


#include"cutpyramid.hpp"
#include<functional>

//subclass this,and rewrite run;
class MedicalDetector : public Runnables<std::function<void(int)>>
{
public:
    MedicalDetector(std::function<void(int)>&& callback);

    void run() override;
};

class GeneratePostImage:public Runnables<std::function<void(int)>>
{
public:
    GeneratePostImage(const QString& src,const QString& dst,std::function<void(int)>&& callback);

    void run() override;

private:
    QString src;
    QString dst;
};

