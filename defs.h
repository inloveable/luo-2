#ifndef DEFS_H
#define DEFS_H

#include "qurl.h"
#include<QString>
#include <vector>
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

//只是为了打印而存在
struct PrintInfo{
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

struct CheckInfo{
    QString checkDepartment;
    QString checkPositon;
    QString checkDate;
    QString illAbstract;
    QString CheckResult;
    bool extra=false;
};

struct PatientInfo
{
    PatientInfo():m_patientName(""),m_patientAge(""),
        m_patientSex(""),m_patientPhone(""),
        m_patientId(""),
        m_patientState1(""),
        m_patientState2(""),
        loginDate(""){}
    void addCheckInfo(const CheckInfo& info){
        checkInfos.push_back(info);
    }

    QString m_patientName;
    QString m_patientAge;
    QString m_patientSex;
    QString m_patientPhone;
    QString m_patientId;
    QString m_patientState1;
    QString m_patientState2;  
    QString loginDate;
    std::vector<CheckInfo> checkInfos;

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

#define PROFILE_BEGIN     auto begin=std::chrono::high_resolution_clock::now()
#define PROGILE_END       auto end=std::chrono::high_resolution_clock::now()
#define PROFILE_RESULT    std::chrono::duration_cast\
<std::chrono::milliseconds>(end-begin).count()



#endif // DEFS_H
