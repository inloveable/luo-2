
#pragma once



#include <QObject>

//All ProgressBar under same operation should be mannered under this Class(Object)
//In this case,analize operation
class QProgressBar;
class ProgressController : public QObject
{
    Q_OBJECT
public:
    explicit ProgressController(QObject *parent = nullptr);


    void adoptProgressBar(QProgressBar* bar);
    bool moveToNextStage();

    void setProgressStage(int n){stages=n;};
    void start();


signals:
    void updateProgressBar(int val);
private:
    QVector<QProgressBar*> progressBars;


    //阶段:切割，分析,复原，后切割
    int stages=4;
    int currentStage=0;

    QTimer* timer=nullptr;

    int currentVal=0;
    int maxVal=0;

};

