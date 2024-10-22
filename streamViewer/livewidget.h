#ifndef LIVEWIDGET_H
#define LIVEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <gst/video/videooverlay.h>
#include "gstthread.h"

class LiveWidget : public QWidget {
    Q_OBJECT
public:
    LiveWidget(QWidget *parent = nullptr);
    void setRtspUrl();

public slots:
    void playLive(QString);
    void pauseLive();
    void stopLive();

private:
    GstThread* gstThread;
    QLabel* window;
};

#endif
