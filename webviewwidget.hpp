
#pragma once


#include <QObject>
#include <QWidget>

class QWebEngineView;
class WebViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebViewWidget(QWidget *parent = nullptr);
    ~WebViewWidget();

    void setUrl(const QUrl& url);

    QImage screenShot();

    void setFullScreen(bool val);

signals:


private:
    QWebEngineView* webView=nullptr;
};

