
#pragma once


#include <QObject>


class MainWindow;
class QmlCommunicator:public QObject
{
    Q_OBJECT
public:
    QmlCommunicator(MainWindow* m,QObject* parent=nullptr):QObject{parent}{mainwindow=m;};
    Q_INVOKABLE void login(QString account,QString password);
    void onLoginResult(bool result,QString message);

    Q_INVOKABLE QString getCurrentDoctorName();

    Q_INVOKABLE int tryRegister(QString account,QString password,QString doctorName);
    Q_INVOKABLE int intoTCTAnalize();
    Q_INVOKABLE void setCurrentPatient(QString identity);
    //PatientInfo has a list of CheckInfos,to avoid clustering
    Q_INVOKABLE void addPatient(QString name,QString age,QString sex,
                                QString checkTime,
                                QString identity
                                ,QString phoneNum,
                                QString hasCance,
                                QString hasHPV);

    Q_INVOKABLE void populatePatients();//INIT_FUNC
    Q_INVOKABLE void showMassageBox(QString message);

signals:
    void loginResult(bool result,QString message);
    void addPatientResult(bool result,QString message);
    void tryLogin(QString account,QString password);

    int askRegister(const QString& account,const QString& password
                    ,const QString& docId,const QString& picturePath);

    //reactivates LoginWindow
    void showMainArea();

    //
    //Critical:Should only be used for populating patients
    //Should not be mixed in other circumstances
    //TODO:Refactoring????
    void sendPatient(QString name,QString age,QString sex,//INIT_FUNC
                     QString checkTime,
                     QString identity
                     ,QString phoneNum,
                     QString hasCance,
                     QString hasHPV);
private:

    MainWindow* mainwindow;
};

