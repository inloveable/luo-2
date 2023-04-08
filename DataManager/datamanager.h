#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include<QMap>
#include <memory>
#include <mutex>
#include"../defs.h"
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
    std::unique_ptr<CheckInfo>& getPatientInfo(){return currentPatient;};
signals:

 private:

    void initializeDatabase();
    void saveConfigurationWhenExit();


    QMap<DirectoryPath,QString> directories;

    std::unique_ptr<UserInfo> currentUser=nullptr;
    std::unique_ptr<CheckInfo> currentPatient=nullptr;

    void readAllUserInfoFromDatabase();
    void writeAllUserInfoToDatabase();

    std::vector<UserInfo> users;
    std::vector<CheckInfo> records;
};

#endif // DATAMANAGER_H
