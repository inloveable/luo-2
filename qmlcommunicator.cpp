
#include "qmlcommunicator.hpp"
#include"defs.h"
#include"datamanager.h"
#include<QMessageBox>
#include"mainwindow.hpp"
#include<QFileDialog>
#include<glog/logging.h>
void QmlCommunicator::setCurrentPatient(QString identity)
{
    auto& Data=*DataManager::GetInstance();
    bool result=Data.setCurrentPatient(identity);
    if(result==false)
    {
        LOG(FATAL)<<"error when set current patient:No such patient:"<<identity.toStdString();
    }
}

void QmlCommunicator::addPatient(QString name,QString age,QString sex,
                                        QString checkTime,
                                        QString identity
                                        ,QString phoneNum,
                                        QString hasCance,
                                        QString hasHPV)
{
    PatientInfo patient;
    patient.m_patientName=name;
    patient.m_patientAge=age;
    patient.m_patientId=identity;
    patient.m_patientPhone=phoneNum;
    patient.m_patientSex=sex;
    patient.m_patientState1=hasCance;
    patient.m_patientState2=hasHPV;
    patient.loginDate=checkTime;

    bool result=DataManager::GetInstance()->addPatient(patient);
    if(result==false)
    {
        emit this->addPatientResult(false,"病人已存在");
    }
    else
    {
        emit this->addPatientResult(true,"添加成功");
    }

}

void QmlCommunicator::populatePatients()
{
    auto& Data=*DataManager::GetInstance();

    const auto& patients=Data.getPatients();
    LOG(INFO)<<"initial patients count:"<<patients.size();
    for(const auto& p:patients)
    {
        emit sendPatient(p.m_patientName,p.m_patientAge,
                         p.m_patientSex,p.loginDate,
                         p.m_patientId,p.m_patientPhone,
                         p.m_patientState1,p.m_patientState2);
    }
}

QString QmlCommunicator::getCurrentDoctorName()
{
    auto& userInfo=DataManager::GetInstance()->getCurretnUser();
    if(userInfo!=nullptr)
    {
        return userInfo->doctorName;
    }
    return "";
}

void QmlCommunicator::showMassageBox(QString message)
{
    QMessageBox::information(mainwindow,"提示",message);
}

void QmlCommunicator::onLoginResult(bool result,QString message)
{
    if(result==true)
    {

    }
    emit loginResult(result,message);

}

void QmlCommunicator::login(QString account,QString password)
{
    if(account==""||password=="")
    {
        emit loginResult(false,"账号或密码为空");
        return;
    }
    emit tryLogin(account,password);
}

int QmlCommunicator::tryRegister(QString account,QString password,QString doctorId)
{
    QMessageBox::information(mainwindow,"提示","医生您好，注册账号请提供个人手写签名照片!");
    auto path=QFileDialog::getOpenFileName(mainwindow,"选择签名照片");
    if(path=="")
    {
        return -1;
    }

    int ret=DataManager::GetInstance()->writeAccountInfoToDatabase(account,password,doctorId,path);
    if(ret==-2)
    {
        QMessageBox::warning(mainwindow,"警告","账号已存在");
    }
    if(ret==-3)
    {
        QMessageBox::warning(mainwindow,"警告","该医生姓名已注册");
    }
    return ret;
}

int QmlCommunicator::intoTCTAnalize(){
    mainwindow->reEnter();
    mainwindow->show();
    return 0;
}
