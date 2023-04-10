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
                             POST_TIFF,
                             PYRAMID_INPUT,
                             PYRAMID_OUTPUT,
                             DEFAULT_DOCUMENTS,
                             HTML_PATH,
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

    const auto& getPatients(){return patients;};




   //let all ui progressbars come here
   void adoptProgressBar(QProgressBar* bar);

   //patient relative operations
    bool addPatient(PatientInfo& info);
    bool addCheckInfo(CheckInfo&,QString patientIdentity);
    std::unique_ptr<PatientInfo>& getPatientInfo(){return currentPatient;};
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
    void writeAllCheckInfoFromDatabase();

    std::vector<UserInfo> users;
    std::vector<PatientInfo> patients;

    //this string is based on the open time of a test file,and used MD5 cyrpto.
    QString identicalHashMarkForAnalize="";
    void makeAnalizeMarkIdentical();

    ProgressController* controller=nullptr;

    QString checkDepartment="内科";
    QString checkPosition="";

    void resetCurrent();


};

#endif // DATAMANAGER_H
