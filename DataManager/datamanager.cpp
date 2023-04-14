#include "datamanager.h"
#include"libqsql.h"
#include "mprinter.h"
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
        controller=new ProgressController(this);
}
/*
             libvips cutting
//打开分析图片----------------->图片筛选(将空白图片移到output)---------

---->预处理(将空白图片与待分析图片分流,空白图片直接到/appCache/postPyramidBuffer(按照金字塔格式)
，待分析图片到/appCache/pyramidBuffer/input)
-------------------->MedicalDetector()---->将ouput中的分析后图片再放到金字塔文件夹下---->提示分析完成
-------------------->生成新html---->显示在结果webview
自动地将最底层的图片result.tiff---------->客户点击保存就让他等待

*/
void DataManager::init()
{

    initializeDatabase();
    readAllUserInfoFromDatabase();
    readAllPatientInfoFromDatabase();  
    readAllCheckInfoFromDatabase();



    const QString currentBin=QApplication::applicationDirPath();
    directories.insert(DirectoryPath::LOG,QString{currentBin+"/log"});
    directories.insert(DirectoryPath::PRE_TIFF,QString{currentBin+"/appCache/prePyramidBuffer"});
    directories.insert(DirectoryPath::POST_TIFF,QString{currentBin+"/appCache/postPyramidBuffer"});
    directories.insert(DirectoryPath::PYRAMID_INPUT,QString{currentBin+"/appCache/pyramidBuffer/input"});
    directories.insert(DirectoryPath::PYRAMID_OUTPUT,QString{currentBin+"/appCache/pyramidBuffer/output"});
    directories.insert(DirectoryPath::DEFAULT_DOCUMENTS,QString{currentBin+"/appCache/documents"});


    directories.insert(DirectoryPath::HTML_PATH,QString{currentBin+"/appCache/documents"});
    directories.insert(DirectoryPath::PRE_HTML_PATH,QString{currentBin+"/preHtml"});
    directories.insert(DirectoryPath::POST_HTML_PATH,QString{currentBin+"/postHtml"});
    //doubted:js file?
    //directories.insert(DirectoryPath::JavaScript_PATH,QString{"qrc:/resources/html/preHtml/openseadragon.min.js"});

    LOG(INFO)<<"leaving datamanger init";
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
    resetCurrent();
    this->writeAllUserInfoToDatabase();
    this->writeAllPatientInfoToDatabase();
    this->writeAllCheckInfoToDatabase();
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


    columns.clear();
    columns<<"checkDepartment"<<"checkPosition"<<"checkDate"
           <<"illAbstract"<<"CheckResult"<<"patientIdentity";
    dbManager.createTable("CheckInfo",columns,strType,strType,
                          strType,strType,strType,strType);
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
    //this->identicalHashMarkForAnalize=Utility::hashStringMd5(currentDateTime);
    this->identicalHashMarkForAnalize=currentDateTime;
}

bool DataManager::setCurrentUser(const QString& account,const QString& docId)
{

    std::unique_ptr<UserInfo> lastUser=nullptr;
    if(currentUser!=nullptr)
    {
        currentUser.swap(lastUser);
    }
    for(auto&& i:users)
    {
        if(lastUser!=nullptr)
        {
            if(i.account==lastUser->account)
            {
                i=*lastUser;//stack object automatically destory
            }
        }
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
    std::unique_ptr<PatientInfo> lastUser=nullptr;
    if(currentPatient!=nullptr)
    {
        currentPatient.swap(lastUser);
    }
    for(auto&& i:patients)
    {
        if(lastUser!=nullptr)
        {
            if(i.m_patientId==lastUser->m_patientId)
            {
                i=*lastUser;
            }
        }
        if(i.m_patientId==id)
        {
                currentPatient=std::make_unique<PatientInfo>(i);
                return true;

        }
    }
    LOG(WARNING)<<"patient id:"<<id.toStdString()<<" not found";
    return false;
}

void DataManager::adoptProgressBar(QProgressBar* bar)
{

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

void DataManager::generateCheckInfo(const QString& illAbstract,const QString& checkStatus)
{
    if(currentPatient==nullptr)
    {
        LOG(WARNING)<<"tring to generate checkinfo on null currentPatient";
        return;
    }

    CheckInfo info;
    info.extra=true;
    info.CheckResult=checkStatus;
    info.checkDate=QDateTime::currentDateTime().toString("yyyy.MM.dd:hh.mm.ss");
    info.illAbstract=illAbstract;
    info.checkDepartment=checkDepartment;
    info.checkPositon=checkPosition;
    currentPatient->checkInfos.emplace_back(std::move(info));
    LOG(INFO)<<"Done generating checkInfo";

}

void DataManager::writeAllCheckInfoToDatabase()
{
    auto db=QSqlDatabase::database("masterbase.db");
    db.open();
    for(auto&& p:patients)
    {
        for(auto&& i:p.checkInfos)
        {
            if(i.extra==true)
            {
                LOG(INFO)<<"WRITING NEW CheckInfo identity:"<<p.m_patientId.toStdString();
                QSqlQuery query(db);
                QString statement = QString("insert into CheckInfo "
                                            "values('%1' ,'%2' ,'%3' ,'%4','%5','%6'"
                                            ")")
                                        .arg(i.checkDepartment).arg(i.checkPositon)
                                        .arg(i.checkDate).arg(i.illAbstract)
                                        .arg(i.CheckResult).arg(p.m_patientId);

                query.exec(statement);

            }
        }

    }
    db.close();
}

void DataManager::readAllCheckInfoFromDatabase()
{
    QSqlDatabase db=QSqlDatabase::database("masterbase.db");
    if(!db.open())
    {
        LOG(INFO)<<"database open error when first read";
        return;
    }
    for(auto&& p:patients)
    {
        QSqlQuery query(db);
        QString statement = QString("SELECT * FROM CheckInfo "
                                    "WHERE patientIdentity='%1'").arg(p.m_patientId);
        query.exec(statement);


        CheckInfo info;

        auto record=query.record();

        int indexOfDepartment=record.indexOf("checkDepartment");
        int indexOfpostion=record.indexOf("checkPosition");
        int indexOfcheckDate=record.indexOf("checkDate");
        int indexOfillAbstract=record.indexOf("illAbstract");
        int indexOfcheckResult=record.indexOf("CheckResult");
        while(query.next())
        {
            info.checkDepartment=query.value(indexOfDepartment).toString();
            info.checkPositon=query.value(indexOfpostion).toString();
            info.checkDate=query.value(indexOfcheckDate).toString();
            info.illAbstract=query.value(indexOfillAbstract).toString();
            info.CheckResult=query.value(indexOfcheckResult).toString();


            info.extra=false;
            p.checkInfos.push_back(info);

        }
        LOG(INFO)<<"Patient:"<<p.m_patientName.toStdString()
                  <<" checkinfo count"<<p.checkInfos.size();
    }


}

//reset resource into buffer when exit
void DataManager::resetCurrent()
{
    if(currentPatient!=nullptr)
    {
        for(auto&& p:patients)
        {
            if(p.m_patientId==currentPatient->m_patientId)
            {
                p=*currentPatient;
                break;
            }
        }
    }
    if(currentUser!=nullptr)
    {
        for(auto&& u:users)
        {
            if(u.account==currentUser->account)
            {
                u=*currentUser;
                break;
            }
        }
    }

}

bool DataManager::addCheckInfo(CheckInfo& info)
{

    info.extra=true;
    info.checkDepartment=this->checkDepartment;
    info.checkDate=QDateTime::currentDateTime().toString("yyyy.MM.dd hh.mm");
    info.checkPositon=this->checkPosition;
    this->currentPatient->checkInfos.push_back(info);
    return true;
}


void DataManager::printReport(const QString& pic1,const QString& pic2,
                 const QString& illAbstract,const QString& checkStatus)
{
    PrintInfo info;
    info.name=currentPatient->m_patientName;
    info.gender=currentPatient->m_patientSex;
    info.age=currentPatient->m_patientAge;
    info.id=currentPatient->m_patientId;
    //超声号我忘了,是身份证号吗？
    info.ultrasonicCode=currentPatient->m_patientId;
    info.illAbstract=illAbstract;
    info.CheckResult=checkStatus;
    //info.checkTip=?;
    info.pic1=pic1;
    info.pic2=pic2;

    info.checkDevice=this->checkDevice;
    info.checkDepartment=this->checkDepartment;
    info.checkPositon=this->checkPosition;
    info.checkDate=QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh:mm");
    info.checkDoc=currentUser->doctorName;
    //printDate;
    //printDoc;
    info.title=this->checkTitle;

    mPrinter printer;
    const QString path=this->getDirectoryPath(DirectoryPath::DEFAULT_DOCUMENTS)+"/"+
        info.name+"_"+QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm")+".pdf";
    printer.printCheckPDF(path,info);

    LOG(INFO)<<"print complete";

}


/*
void DataManager::writeAllCheckInfoToDatabase()
{

    for(auto&& p:patients)
    {
        for(auto&& c:p.checkInfos)
        {
            if(c.extra==true)
            {
                QSqlQuery query(QSqlDatabase::database());
                // 准备 SQL 语句
                QString sql = "INSERT INTO CheckInfo (checkDepartment, checkPosition, checkDate, illAbstract, CheckResult, patientIdentity)"
                              "VALUES (:checkDepartment, :checkPosition, :checkDate, :illAbstract, :CheckResult, :patientIdentity)";

                // 绑定参数
                query.prepare(sql);
                query.bindValue(":checkDepartment",c.checkDepartment);
                query.bindValue(":checkPosition", c.checkPositon);
                query.bindValue(":checkDate", c.checkDate);
                query.bindValue(":illAbstract", c.illAbstract);
                query.bindValue(":CheckResult", c.CheckResult);
                query.bindValue(":patientIdentity",p.m_patientId);
                // 执行 SQL 语句
                if (!query.exec()) {
                    qDebug() << "Failed to insert row:" << query.lastError().text();
                }
            }
        }
    }


}
*/

const std::vector<PatientInfo>& DataManager::getPatients()
{
    if(currentPatient!=nullptr)
    {
        for(auto&& p:patients)
        {
            if(p.m_patientId==currentPatient->m_patientId)
            {
                p=*currentPatient;
                break;
            }
        }
    }

    return patients;
}











