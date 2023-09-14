
#pragma once

#include "qobject.h"
#include <QRunnable>

#include <QObject>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

template<typename R,typename... Args>
struct function_traits;
template<typename R,typename... Args>
struct function_traits<std::function<R(Args...)>>
{
    static const size_t nargs = sizeof...(Args);

    typedef R result_type;

    using ArgumentTuple=std::tuple<Args...>;
};

template<typename Callback>
class Runnables:public QRunnable{
public:

    //using Function_type=Callback(Args...);

    using FunctionTuple=typename function_traits<Callback>::ArgumentTuple;
    Runnables(Callback&& callbeck):generalFlag(false),
        callback{std::forward<Callback>(callbeck)}{};



    void setGeneralFlag(){generalFlag=true;};
    void invoke(){
        std::apply(callback,m_args);
    }
    virtual ~Runnables(){invoke();};

    template<typename... realArgs>
    void takeArgs(realArgs&&... args)
    {
        static_assert(sizeof...(args)==
                          function_traits<Callback>::nargs,"Runnables:parameter count mismatch");
        m_args = std::make_tuple(std::forward<realArgs>(args)...);
    };

protected:
    int result=0;

    bool generalFlag=false;

private:
    Callback callback;

    FunctionTuple m_args;


};


class CutPyramid : public Runnables<std::function<void(int)>>
{

public:
    CutPyramid(QString src,QString dst,
               int tileSize,int overlap,std::function<void(int)>&& callback);
    void run() override;
private:
    QString src;
    QString dst;
    int tileSize;
    int overlap;
};

class MovePictures:public Runnables<std::function<void(int sucess,qint64,qint64)>>
{

};

class RecoverTiff:public Runnables<std::function<void(int sucess)>>
{
public:
};


