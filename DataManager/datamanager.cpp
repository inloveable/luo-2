#include "datamanager.h"
#include"libqsql.h"
#include "progresscontroller.hpp"
#include "qapplication.h"
#include "qdatetime.h"
#include "qprogressbar.h"
#include "qsqldatabase.h"


#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include"../vipswrapper.hpp"
#include<glog/logging.h>
#include <memory>
SINGLETON_IMPLEMENT(DataManager)

DataManager::DataManager(QObject *parent)
    : QObject{parent}
{

}

void DataManager::init()
{

    initializeDatabase();
    readAllUserInfoFromDatabase();
    readAllPatientInfoFromDatabase();

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
    this->writeAllPatientInfoToDatabase();
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

    for(auto&& i: m_pInstance->users)
    {
        if(i.account==account)
        {
            if(i.password==password)
            {
                m_pInstance->setCurrentUser(account,i.doctorName);
                return true;
            }
        }
    }

    return false;
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

void DataManager::makeAnalizeMarkIdentical()
{
    auto currentDateTime=QDateTime::currentDateTime().toString("MM.dd.hh.mm.ss");
    this->identicalHashMarkForAnalize=Utility::hashStringMd5(currentDateTime);
}

bool DataManager::setCurrentUser(const QString& account,const QString& docId)
{
    for(auto&& i:users)
    {
        if(i.account==account)
        {
            if(i.doctorName==docId)
            {
                currentUser=std::make_unique<UserInfo>(i);
                LOG(INFO)<<"current User updated Hello doctor:"<<docId.toStdString();
                return true;
            }
        }
    }
    return false;
}

bool DataManager::setCurrentPatient(const QString& id)
{
    for(auto&& i:patients)
    {
        if(i.m_patientId==id)
        {
                currentPatient=std::make_unique<PatientInfo>(i);
                return true;

        }
    }
    return false;
}

void DataManager::adoptProgressBar(QProgressBar* bar)
{
    if(controller==nullptr)
    {
        controller=new ProgressController(this);
    }
    controller->adoptProgressBar(bar);
}

bool DataManager::addPatient(PatientInfo& info)
{
    for(auto&& i:this->patients)
    {
        if(i.m_patientId==info.m_patientId)
        {
            LOG(INFO)<<"adding duplicate patient,returnning";
            return false;
        }
    }

    info.extra=true;

    patients.emplace_back(std::move(info));
    return true;
}

void DataManager::readAllPatientInfoFromDatabase()
{
    QSqlDatabase db=QSqlDatabase::database("masterbase.db");
    if(!db.open())
    {
        LOG(INFO)<<"database open error when first read";
        return;
    }

    QSqlQuery query(db);
    QString statement = QString("SELECT * FROM PatientInfo");
    query.exec(statement);


    PatientInfo info;

    auto record=query.record();
    int indexOfUserName=record.indexOf("id");
    int indexOfPassword=record.indexOf("name");
    int indexOfDoctorName=record.indexOf("age");
    int indexOfSignature=record.indexOf("sex");
    int indexOfPhone=record.indexOf("phone");
    int indexOfHasCancer=record.indexOf("state1");
    int indexOfHasHpv=record.indexOf("state2");
    int indexOfCheckDate=record.indexOf("checkDate");
    while(query.next())
    {

        info.m_patientId=query.value(indexOfUserName).toString();
        info.m_patientName=query.value(indexOfPassword).toString();
        info.m_patientAge=query.value(indexOfDoctorName).toString();
        info.m_patientSex=query.value(indexOfSignature).toString();
        info.m_patientPhone=query.value(indexOfPhone).toString();
        info.m_patientState1=query.value(indexOfHasCancer).toString();
        info.m_patientState2=query.value(indexOfHasHpv).toString();
        info.loginDate=query.value(indexOfCheckDate).toString();
        info.extra=false;
        patients.push_back(info);
    }

    LOG(INFO)<<"INITIAL Patient count: "<<users.size();
}

void DataManager::writeAllPatientInfoToDatabase()
{
    auto db=QSqlDatabase::database("masterbase.db");
    db.open();
    for(auto&& i:patients)
    {
        if(i.extra==true)
        {
            LOG(INFO)<<"WRITING NEW patient:"<<i.m_patientName.toStdString();

            if(db.lastError().isValid())
            {
                LOG(INFO)<<"database error: "<<db.lastError().text().toStdString();
            }
            QSqlQuery query(db);
            QString statement = QString("insert into PatientInfo "
                                        "values('%1' ,'%2' ,'%3' ,'%4','%5','%6',"
                                        "'%7','%8')")
                                    .arg(i.m_patientId).arg(i.m_patientName)
                                    .arg(i.m_patientAge).arg(i.m_patientSex)
                                    .arg(i.m_patientPhone).arg(i.m_patientState1)
                                    .arg(i.m_patientState2).arg(i.loginDate);
            query.exec(statement);

        }
    }
    db.close();
}











