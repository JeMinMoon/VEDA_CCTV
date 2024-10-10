#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <gst/gst.h>

class VideoWidget : public QWidget {
    Q_OBJECT
public:
    VideoWidget(QWidget *parent = nullptr);
    ~VideoWidget();
    void setRtspUrl(const QString &rtspUrl);

public slots:
    void play();
    void pause();
    void stop();

private:
    GstElement *pipeline, *source, *demux, *parse, *filter, *decodebin, *sink;
};

#endif
