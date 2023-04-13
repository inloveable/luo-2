
#pragma once

#include"cutpyramid.hpp"
#include <functional>
#include <mutex>
#include <queue>


class CheckBackgroundAndMove : public Runnables<std::function<void(int)>>
{
public:
    CheckBackgroundAndMove(const QString& src,
                           const QString&backgroundDst,const QString& imageDst,
                           std::function<void(int)>&& callback);

    void run() override;

private:
    QString src;
    QString imageDst;
    QString backgroundDst;



};

