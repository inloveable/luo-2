#include "inputcheckinfowidget.hpp"

InputCheckInfoWidget::InputCheckInfoWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
}


QString InputCheckInfoWidget::getCheckResult()
{
    return ui.textEdit_2->toPlainText();
}

QString InputCheckInfoWidget::getIllAbstract()
{
    return ui.textEdit->toPlainText();
}

void InputCheckInfoWidget::resetInput()
{
    ui.textEdit->clear();
    ui.textEdit_2->clear();
}
