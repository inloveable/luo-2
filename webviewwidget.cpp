
#include "webviewwidget.hpp"
#include "qgridlayout.h"
#include "qnamespace.h"
#include "qwebengineview.h"
#include<QWebEngineView>
#include<QGridLayout>
#include<glog/logging.h>
#include<QFile>



WebViewWidget::WebViewWidget(QWidget *parent)
    : QWidget{parent}
{
    QGridLayout* layout=new QGridLayout;
    webView=new QWebEngineView(this);
    layout->addWidget(webView);

    this->setLayout(layout);

    webView->page()->setBackgroundColor(Qt::gray);
}

WebViewWidget::~WebViewWidget()
{
    webView->deleteLater();
}

void WebViewWidget::setUrl(const QUrl& url)
{
    webView->setUrl(url);
}

QImage WebViewWidget::screenShot()
{
    return webView->grab().toImage();
}

void WebViewWidget::setFullScreen(bool val)
{
    if(val)
        webView->showFullScreen();
    else
        webView->showNormal();
}

