
#pragma once


#include <QObject>
#include <chrono>
#include <ratio>


class VipsWrapper : public QObject
{
    Q_OBJECT
public:
    explicit VipsWrapper(QObject *parent = nullptr);

    static int dzSave(QString picture,QString dst,int tilesize=1024,int overlap=0);
    static int saveToTiff(QList<QString> files);

signals:

    void compeleteSignal(int val);

private:


};

class Utility
{
   public:

      //returns time cost
    static qint64 removeDir(const QString& dir);

    //must be called coupled
   static void calcuBegin();
   static qint64 calcuEnd();

private:


    static std::chrono::time_point<std::chrono::high_resolution_clock> begin;
    static std::chrono::time_point<std::chrono::high_resolution_clock> end;

};

