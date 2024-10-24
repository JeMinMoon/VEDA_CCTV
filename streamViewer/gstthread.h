#ifndef GSTTHREAD_H
#define GSTTHREAD_H

#include <QThread>
#include <QWidget>
#include <gst/gst.h>

class GstThread : public QThread {
    Q_OBJECT

public:
    GstThread(WId windowId, QString rtspUrl);
    void cleanup();
    void run() override;
    void stop();
    void SetWindowId(WId);
    void SetRtspUrl(QString);
    bool stopped;
    GMainLoop *loop;

private:
    WId windowId;
    QString rtspUrl;
    GstElement *pipeline, *source, *depay, *parse1, *parse2, *decoder, *displaysink, *filesink,
        *tee, *queue_display, *queue_record, *encoder, *mux, *videoconvert, *timestamper;
};


#endif // GSTTHREAD_H
