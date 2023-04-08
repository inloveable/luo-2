#ifndef DEFS_H
#define DEFS_H

#include "qurl.h"
#include<QString>
//类中添加声明
#define SINGLETON_DECLARE(Class) \
public:\
    static Class *GetInstance();\
    void Destory();\
private:\
    static Class *m_pInstance;

//cpp中添加实现
#define SINGLETON_IMPLEMENT(Class) \
    Class * Class::m_pInstance = new Class;\
    Class * Class::GetInstance()\
{\
    return m_pInstance;\
}\
 \


struct CheckInfo{
    QString name;
    QString gender;
    int age;
    int id;
    QString ultrasonicCode;
    QString illAbstract;
    QString CheckResult;
    QString CheckTip;
    QUrl pic1;
    QUrl pic2;
    QString checkDevice;
    QString checkDepartment;
    QString checkPositon;
    QString checkDate;
    QString checkDoc;
    QString printDate;
    QString printDoc;
    QString title;//Header;
    QString signaturePath;
};

struct PatientInfo
{
    PatientInfo():m_patientName(""),m_patientAge(""),
        m_patientSex(""),m_patientPhone(""),
        m_patientId(""),
        m_patientState1(""),
        m_patientState2(""),
        m_DocId(""),
        CheckDate(""){}
    QString m_patientName;
    QString m_patientAge;
    QString m_patientSex;
    QString m_patientPhone;
    QString m_patientId;
    QString m_patientState1;
    QString m_patientState2;
    QString m_DocId;
    QString CheckDate;

    bool extra=false;
};

struct UserInfo{
    QString doctorName;
    QString account;
    QString password;
    QString signaturePath;
    bool extra=false;//mark for newly added data
    //more to came;
};

#endif // DEFS_H
