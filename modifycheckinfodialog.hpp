#pragma once

#include "ui_modifycheckinfodialog.h"
#include<QDialog>
class ModifyCheckInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyCheckInfoDialog(QWidget *parent = nullptr);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ModifyCheckInfoDialog ui;
};

