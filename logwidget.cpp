#include "logwidget.hpp"
#include"datamanager.h"
#include "qtreewidget.h"
#include<QTreeWidgetItem>
LogWidget::LogWidget(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    setWindowTitle("日志");

    auto& Data=*DataManager::GetInstance();

    auto patients=Data.getPatients();

    for(auto&& p:patients)
    {
        QTreeWidgetItem* rootItem=new QTreeWidgetItem{ui.treeWidget};

        rootItem->setText(0,p.m_patientName);
        QTreeWidgetItem* checkItem=nullptr;
        for(auto&& c:p.checkInfos)
        {
            checkItem=new QTreeWidgetItem(rootItem);
            checkItem->setText(1,c.checkDate);
            checkItem->setText(2,c.checkDepartment);
            checkItem->setText(3,c.checkPositon);
            checkItem->setText(4,c.CheckResult);
        }
    }

    ui.treeWidget->expandAll();
}

void LogWidget::on_pushButton_clicked()
{
    close();
}

