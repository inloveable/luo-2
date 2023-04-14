#pragma once

#include "ui_inputcheckinfowidget.h"

class InputCheckInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InputCheckInfoWidget(QWidget *parent = nullptr);

    QString getIllAbstract();
    QString getCheckResult();

    void resetInput();

private:
    Ui::InputCheckInfoWidget ui;
};

