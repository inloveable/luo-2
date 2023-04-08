#include "datamanager.h"
#include"libqsql.h"
#include "qapplication.h"
#include "qsqldatabase.h"


#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>

#include<glog/logging.h>
SINGLETON_IMPLEMENT(DataManager)

DataManager::DataManager(QObject *parent)
    : QObject{parent}
{

}

void DataManager::init()
{

    initializeDatabase();
    readAllUserInfoFromDatabase();
    const QString currentBin=QApplication::applicationDirPath();
    directories.insert(DirectoryPath::LOG,QString{currentBin+"/log"});
    directories.insert(DirectoryPath::POST_TIFF,QString{currentBin+"/appCache/postPyramidBuffer"});
    directories.insert(DirectoryPath::PYRAMID_INPUT,QString{currentBin+"/appCache/pyramidBuffer/input"});
    directories.insert(DirectoryPath::PYRAMID_OUTPUT,QString{currentBin+"/appCache/pyramidBuffer/output"});
    directories.insert(DirectoryPath::DEFAULT_DOCUMENTS,QString{currentBin+"/appCache/documents"});
    directories.insert(DirectoryPath::HTML_PATH,QString{"qrc:/resources/html/preHtml/Newtest.html"});
    //doubted:js file?
    directories.insert(DirectoryPath::JavaScript_PATH,QString{"qrc:/resources/html/preHtml/openseadragon.min.js"});


}

DataManager::~DataManager()
{

}
//should be called only once;
void DataManager::Destory()
{
   saveConfigurationWhenExit();
}
void DataManager::saveConfigurationWhenExit()
{
    this->writeAllUserInfoToDatabase();
}
void DataManager::initializeDatabase()
{


    L_library::LibQSQL dbManager("QSQLITE","masterbase.db");

    if(!dbManager.empty())
    {
        return;
    }

    QString strType{};
    int     intType=0;

    QVector<QString> columns{"userName","password","doctorName","signature"};

    dbManager.createTable("UserInfo",columns,strType,strType,strType,strType);
    dbManager.insertRowIntoTable_allString("UserInfo",QStringList{"userName","password","doctorName","signature"},
                                           {"admin","123456","赵善","NULL"});
    dbManager.printLastError();

    columns.clear();
    columns<<"id"<<"name"<<"age"
            <<"sex"
            <<"phone"<<"state1"<<"state2"<<"checkDate";
    dbManager.createTable("PatientInfo",columns,strType,strType,intType,strType,strType,strType,strType,strType);
    dbManager.printLastError();
}

int DataManager::checkIfUserInDataBase(const QString& account,const QString& password)
{

    return -1;
}

void DataManager::readAllUserInfoFromDatabase()
{
    QSqlDatabase db=QSqlDatabase::database("masterbase.db");
    if(!db.open())
    {
        LOG(INFO)<<"database open error when first read";
        return;
    }

    QSqlQuery query(db);
    QString statement = QString("SELECT * FROM UserInfo");
    query.exec(statement);


    UserInfo info;

    auto record=query.record();
    int indexOfUserName=record.indexOf("userName");
    int indexOfPassword=record.indexOf("password");
    int indexOfDoctorName=record.indexOf("doctorName");
    int indexOfSignature=record.indexOf("signature");
        while(query.next())
        {

        info.account=query.value(indexOfUserName).toString();
        info.doctorName=query.value(indexOfDoctorName).toString();
        info.password=query.value(indexOfPassword).toString();
        info.signaturePath=query.value(indexOfSignature).toString();
        users.push_back(info);
        }

    LOG(INFO)<<"INITIAL user count: "<<users.size();
}

void DataManager::writeAllUserInfoToDatabase()
{

    for(auto&& i:users)
    {
        if(i.extra==true)
        {
            LOG(INFO)<<"WRITING NEW USER:"<<i.account.toStdString();
            auto db=QSqlDatabase::database("masterbase.db");
            db.open();
            if(db.lastError().isValid())
            {
                LOG(INFO)<<"database error: "<<db.lastError().text().toStdString();
            }
            QSqlQuery query(db);
            QString statement = QString("insert into UserInfo values('%1' ,'%2' ,'%3' ,'%4')")
                                    .arg(i.account).arg(i.password)
                                    .arg(i.doctorName).arg(i.signaturePath);
            query.exec(statement);
            db.close();
        }
    }


}
//actually write to cache
int DataManager::writeAccountInfoToDatabase(const QString& account,const QString& password
                                            ,const QString& docId,const QString& picturePath)
{

    LOG(INFO)<<"checking account exists?";
    for(auto&& i:users)
    {
        if(i.account==account)
        {
            LOG(INFO)<<"found same account";
            return -2;
        }else if(i.doctorName==docId)
        {
            LOG(INFO)<<"found same doctorid";
            return -3;
        }else
        {
            continue;
        }
    }

    UserInfo info;
    info.account=account;
    info.doctorName=docId;
    info.password=password;
    info.signaturePath=picturePath;
    info.extra=true;

    users.emplace_back(info);

    return 0;
}
