#include "modifycheckinfodialog.hpp"
#include"datamanager.h"
ModifyCheckInfoDialog::ModifyCheckInfoDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    auto& Data=*DataManager::GetInstance();

    ui.DepartmentLineEdit->setText(Data.checkDepartment);
    ui.DepartmentLineEdit_2->setText(Data.checkPosition);
    ui.DepartmentLineEdit_6->setText(Data.hospital);
    ui.DepartmentLineEdit_3->setText(Data.checkTitle);
    ui.DepartmentLineEdit_5->setText(Data.checkDevice);

}

void ModifyCheckInfoDialog::on_pushButton_clicked()
{
    //be sure;

    auto& Data=*DataManager::GetInstance();
    Data.checkDepartment=ui.DepartmentLineEdit->text();
    Data.checkPosition=ui.DepartmentLineEdit_2->text();
    Data.hospital=ui.DepartmentLineEdit_6->text();
    Data.checkDevice=ui.DepartmentLineEdit_5->text();
    Data.checkTitle=ui.DepartmentLineEdit_3->text();

    emit accepted();
    close();
}

