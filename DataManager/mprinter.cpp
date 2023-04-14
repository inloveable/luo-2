#include "mprinter.h"
#include "qnamespace.h"
#include "qpainter.h"
#include "qpdfwriter.h"
#include "qstringliteral.h"
#include<QPrinter>
#include<QFile>
#include<QDesktopServices>
#include<QDebug>
mPrinter::mPrinter(QObject *parent)
    : QObject{parent}
{
     pdfWriter=nullptr;
     printer=nullptr;

     painter=nullptr;
}


void mPrinter::printCheckPDF(const QString& url,const PrintInfo& info)
{
    QFile file(url);
    file.open(QIODevice::ReadWrite);
    pdfWriter=new QPdfWriter(&file);

    pdfWriter->setPageSize(QPageSize::A4);                  //设置纸张为A4纸
    pdfWriter->setResolution(QPrinter::ScreenResolution);           //设置分辨率 屏幕分辨率 打印机分辨率 高分辨率
    pdfWriter->setPageMargins(QMarginsF(20, 20, 20, 20));           //设置页边距 顺序是:左上右下


     painter = new QPainter(pdfWriter);

     QTextOption option(Qt::AlignCenter);                           //标题居中显示
     option.setWrapMode(QTextOption::WordWrap);                     //标题自动换行

        QFont font;
        font.setFamily("Microsoft YaHei");                            //设置字体 微软雅黑、宋体之类的
        font.setPointSize(16);                                        //设置字体大小
        font.setBold(true);                                           //加粗
        painter->setFont(font);
        //应客户要求。破坏了
        //应客户要求。破坏了泛型。
               QString titleStr1;
               QString titleStr2;
               int left = info.title.size() - 7;
               //987654321 8765432
               for(int i=0;i<info.title.size(); ++i)
               {
                   if (i < left)
                   {
                       titleStr1.append(info.title[i]);
                   }
                   else
                   {
                       titleStr2.append(info.title[i]);
                   }     
               }

               //
           painter->drawText(QRect(2400, 100, 4000, 300), titleStr1, option);
           painter->drawText(QRect(3400,400,2000,300),titleStr2,option);
        //距离左边3000,上面0,标题字宽度2100，标题字高450

        option.setAlignment(Qt::AlignLeft);
        font.setPointSize(12);                                       //字体大小14
        painter->setFont(font);


        painter->setPen(QPen(QBrush(Qt::black), 8));
        painter->drawLine(500,950,8000,950);

        font.setBold(false);
        painter->setFont(font);

        painter->drawText(QRect(1000, 1500, 1000, 300),QStringLiteral("姓名: "),option);
        painter->drawText(QRect(4000,1500,1000,300),QStringLiteral("性别: "),option);
        painter->drawText(QRect(6500,1500,1000,300),QStringLiteral("年龄: "),option);
        painter->drawText(QRect(1000, 2000, 1000, 300),QStringLiteral("申请科室: "),option);
        painter->drawText(QRect(4000, 2000, 1000, 300),QStringLiteral("检查设备: "),option);
        painter->drawText(QRect(1000, 2500, 1000, 300),QStringLiteral("检查部位: "),option);

        option.setAlignment(Qt::AlignLeft);
        painter->drawText(QRect(7500,1500,1200,300),QString("%1").arg(info.age),option);
        painter->drawText(QRect(5000,2000,1200,300),info.checkDevice,option);
        painter->drawText(QRect(2000,2500,1200,300),info.checkPositon,option);
        painter->drawText(QRect(2000,2000,1200,300),info.checkDepartment,option);
        painter->drawText(QRect(5000,1500,1200,300),info.gender,option);
        painter->drawText(QRect(2000,1500,700,300),info.name,option);

        painter->drawLine(500,3050,8000,3050);

        option.setAlignment(Qt::AlignLeft);
        painter->drawText(QRect(4000, 2500, 1000, 300),QStringLiteral("超声号: "),option);

        painter->drawText(QRect(1000,3750,1000,300),QStringLiteral("病史摘要: "),option);

        option.setAlignment(Qt::AlignLeft);
        painter->drawText(QRect(5000,2500,3000,300),info.ultrasonicCode,option);

        option.setWrapMode(QTextOption::WrapAnywhere);
        painter->drawText(QRect(1500,4050,6000,1300),QString("%1").arg(info.illAbstract),option);


        QPixmap map;
        map.load(info.pic1.path());
        qDebug()<<"pictue 1 isNull:"<<map.isNull();
        painter->drawPixmap(QRect(1000,5500,2200,1650),map);

        map.load(info.pic2.path());
        qDebug()<<"pictue 2 isNull:"<<map.isNull();
        painter->drawPixmap(QRect(4500,5500,2200,1650),map);

        option.setAlignment(Qt::AlignRight);
        painter->drawText(QRect(1000,7800,1000,300),QStringLiteral("检查结果: "),option);
        option.setAlignment(Qt::AlignLeft);
        painter->drawText(QRect(1500,8100,6000,1300),info.CheckResult,option);

        option.setAlignment(Qt::AlignRight);
       // painter->drawText(QRect(1000,9500,1000,300),QStringLiteral("检查提示: "),option);
        option.setAlignment(Qt::AlignLeft);
        painter->drawText(QRect(2000,9800,6000,800),info.CheckTip,option);

        option.setAlignment(Qt::AlignRight);
        painter->drawText(QRect(1000, 10700, 1000, 300),QStringLiteral("报告医生: "),option);
        painter->drawText(QRect(5000,10700,1000,300),QStringLiteral("审核医生: "),option);
        painter->drawText(QRect(1000, 11100, 1000, 300),QStringLiteral("检查日期: "),option);
        painter->drawText(QRect(5000,11100,1000,300),QStringLiteral("打印日期: "),option);


        option.setAlignment(Qt::AlignLeft);

        QPixmap docSignature(info.signaturePath);
        painter->drawPixmap(QRect(6000,10700,1200,300),docSignature);
        painter->drawText(QRect(2000,10700,1200,300),info.checkDoc,option);
        painter->drawText(QRect(6000,11100,1200,300),info.printDate,option);
        painter->drawText(QRect(2000,11100,1200,300),info.checkDate,option);

    delete painter;
    delete pdfWriter;
    pdfWriter=nullptr;
    painter=nullptr;

    file.close();



}
