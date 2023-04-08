#ifndef MPRINTER_H
#define MPRINTER_H

#include "qpdfwriter.h"
#include "qurl.h"
#include <QObject>
#include<qprinter.h>
#include"../defs.h"


class mPrinter : public QObject
{
    Q_OBJECT
public:
    explicit mPrinter(QObject *parent = nullptr);

    void printCheckPDF(const QString& path,const PrintInfo& info);

signals:

private:
    QPdfWriter* pdfWriter;
    QPrinter* printer;
    QPainter* painter;


};

#endif // MPRINTER_H
