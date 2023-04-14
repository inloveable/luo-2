#pragma once

#include "ui_logwidget.h"

class LogWidget : public QDialog
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget *parent = nullptr);

private slots:
    void on_pushButton_clicked();

private:
    Ui::LogWidget ui;
};

