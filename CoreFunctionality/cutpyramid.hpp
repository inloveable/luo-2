
#pragma once

#include "qobject.h"
#include <QRunnable>

#include <QObject>


class CutPyramid : public QRunnable,QObject
{
    Q_OBJECT
public:
    CutPyramid(QString src,QString dst,int tileSize,int overlap);

    void run() override;

signals:
    void runningFinished(bool sucesss);
private:
    QString src;
    QString dst;
    int tileSize;
    int overlap;
};

