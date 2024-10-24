#include "livewidget.h"
#include <QVBoxLayout>
#include <QDebug>

LiveWidget::LiveWidget(QWidget *parent) : QWidget(parent) {

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(0);

    window = new QLabel(this);
    window->setScaledContents(true);
    // window->setPixmap(QPixmap(":/image/image.png"));
    window->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    layout->addWidget(window);
    setLayout(layout);
}

void LiveWidget::playLive(QString rtspUrl) {
    gstThread = new GstThread(window->winId(), rtspUrl);
    gstThread->start();
}

void LiveWidget::pauseLive() {
    if (gstThread) {
        gstThread->wait();
    }
}

void LiveWidget::stopLive() {
    if (gstThread) {
        gstThread->stop();
        gstThread->quit();
        gstThread->wait();
        delete gstThread;
        gstThread = NULL;
    }
}
