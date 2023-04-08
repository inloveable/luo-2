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

    void init();//for some qt dependent operations;

    enum class DirectoryPath{LOG,
                             POST_TIFF,
                             PYRAMID_INPUT,
                             PYRAMID_OUTPUT,
                             DEFAULT_DOCUMENTS,
                             HTML_PATH,
                             JavaScript_PATH};
    const QString& getDirectoryPath(DirectoryPath mark){return directories[mark];};


    static int checkIfUserInDataBase(const QString& account,const QString& password);
    int writeAccountInfoToDatabase(const QString& account,const QString& password
                                          ,const QString& docId,const QString& picturePath);

    std::unique_ptr<UserInfo>&  getCurretnUser(){return currentUser;};
    std::unique_ptr<PatientInfo>& getPatientInfo(){return currentPatient;};


    void adoptProgressBar(QProgressBar* bar);

public slots:
    bool setCurrentUser(const QString& account,const QString& docId);
signals:

 private:

    void initializeDatabase();
    void saveConfigurationWhenExit();


    QMap<DirectoryPath,QString> directories;

    std::unique_ptr<UserInfo> currentUser=nullptr;
    std::unique_ptr<PatientInfo> currentPatient=nullptr;

    void readAllUserInfoFromDatabase();
    void writeAllUserInfoToDatabase();

    std::vector<UserInfo> users;
    std::vector<PatientInfo> patients;

    //this string is based on the open time of a test file,and used MD5 cyrpto.
    QString identicalHashMarkForAnalize="";
    void makeAnalizeMarkIdentical();

    ProgressController* controller=nullptr;


};

#endif // DATAMANAGER_H
