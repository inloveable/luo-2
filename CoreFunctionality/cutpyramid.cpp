
#include "cutpyramid.hpp"
#include"../defs.h"
#include"../vipswrapper.hpp"
#include <functional>
#include <utility>
#include"glog/logging.h"
CutPyramid::CutPyramid(QString src,QString dst,int tileSize,int overlap,
                       std::function<void(int)>&& parent)
    :Runnables{std::forward<std::function<void(int)>>(parent)},
    src{src},dst{dst}
    ,tileSize(tileSize),overlap{overlap}
{

}

void CutPyramid::run()
{
    PROFILE_BEGIN;
    int result=VipsWrapper::dzSave(src,dst,tileSize,overlap);
    Runnables::takeArgs(result);
    PROGILE_END;
    LOG(INFO)<<"dzsave cost:"<<PROFILE_RESULT;

    LOG(INFO)<<"run Cutpytamid finished";
}

