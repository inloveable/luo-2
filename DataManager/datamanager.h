#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include<QMap>
#include <memory>
#include <mutex>
#include"../defs.h"
#include "progresscontroller.hpp"
//同一个病人可以对应多个检查信息
//即PatientInfo->multi->CheckInfo
class  DataManager : public QObject
{
    Q_OBJECT
    SINGLETON_DECLARE(DataManager)
public:


    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();

    void init();//INIT_FUNC
    //for some qt dependent operations;

    enum class DirectoryPath{LOG,
                             PRE_TIFF,
                             POST_TIFF,
                             PYRAMID_INPUT,
                             PYRAMID_OUTPUT,
                             DEFAULT_DOCUMENTS,
                             HTML_PATH,
                             PRE_HTML_PATH,
                             POST_HTML_PATH,
                             JavaScript_PATH};
    const QString& getDirectoryPath(DirectoryPath mark){return directories[mark];};


    //user(Doctor related operations)
    static int                  checkIfUserInDataBase(const QString& account,const QString& password);
    int                         writeAccountInfoToDatabase(
                                        const QString& account,const QString& password
                                       ,const QString& docId,const QString& picturePath);
    std::unique_ptr<UserInfo>&  getCurretnUser(){return currentUser;};
    bool                        setCurrentUser(const QString& account,const QString& docId);


    std::unique_ptr<PatientInfo>&  getCurretnPatient(){return currentPatient;};
    bool                        setCurrentPatient(const QString& identity);


    void generateCheckInfo(const QString& illAbstract,const QString& checkStatus);

    const std::vector<PatientInfo>& getPatients();
    void makeAnalizeMarkIdentical();


    void moveToNextStage(bool reset=false){
        if(reset==false)
        {
            controller->moveToNextStage();
        }
        else
        {

        }
    };

   //let all ui progressbars come here
   void adoptProgressBar(QProgressBar* bar);

   //patient relative operations
    bool addPatient(PatientInfo& info);
    bool addCheckInfo(CheckInfo&);
    std::unique_ptr<PatientInfo>& getPatientInfo(){return currentPatient;};


    void setStages(int s){controller->setProgressStage(s);};

    QString getHashMark()const {return identicalHashMarkForAnalize;};
    ProgressController* controller=nullptr;


    void printReport(const QString& pic,const QString& pic2,
                     const QString& illAbstract,const QString& checkStatus);


    //要修改打印报告，replace here;
    QString checkDepartment="内科";
    QString checkPosition="肺部";
    QString checkTitle="标题";
    QString checkDevice="001";
    QString hospital="社区医院";

public slots:


 private:

    void initializeDatabase();//INIT_FUNC
    void saveConfigurationWhenExit();


    QMap<DirectoryPath,QString> directories;

    std::unique_ptr<UserInfo> currentUser=nullptr;
    std::unique_ptr<PatientInfo> currentPatient=nullptr;

    void readAllUserInfoFromDatabase();//INIT_FUNC
    void writeAllUserInfoToDatabase();

    void readAllPatientInfoFromDatabase();//INIT_FUNC
    void writeAllPatientInfoToDatabase();

    //must be called after init patient info
    void readAllCheckInfoFromDatabase();//INIT_FUNC
    void writeAllCheckInfoToDatabase();

    std::vector<UserInfo> users;
    std::vector<PatientInfo> patients;

    //this string is based on the open time of a test file,and used MD5 cyrpto.
    QString identicalHashMarkForAnalize="";






    void resetCurrent();


};

#endif // DATAMANAGER_H
