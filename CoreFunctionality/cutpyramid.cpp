
#include "cutpyramid.hpp"
#include"../vipswrapper.hpp"
CutPyramid::CutPyramid(QString src,QString dst,int tileSize,int overlap):src{src},dst{dst}
    ,tileSize(tileSize),overlap{overlap}
{

}

void CutPyramid::run()
{
    int result=VipsWrapper::dzSave(src,dst,tileSize,overlap);

    if(result==0)
    {
        emit runningFinished(true);
    }
    else
    {
        emit runningFinished(false);
    }

}

