#include "videowidget.h"
#include <QDebug>

static void on_pad_added (GstElement *element, GstPad *pad, gpointer data);

VideoWidget::VideoWidget(QWidget *parent) : QWidget(parent) {
    // Create GStreamer elements
    pipeline = gst_pipeline_new("video player");
    source = gst_element_factory_make("rtspsrc", "source");
    demux = gst_element_factory_make("rtph264depay", "depayl");
    parse = gst_element_factory_make("h264parse", "parse");
    filter = gst_element_factory_make("capsfilter", "filter");
    decodebin = gst_element_factory_make("openh264dec", "decode");
    sink = gst_element_factory_make("glimagesink", "sink");

    if (!pipeline || !source || !demux || !parse || !filter || !decodebin || !sink) {
        g_printerr("Not all elements could be created.\n");
        exit(1);
    }

    // Add elements to the pipeline, but don't link the source yet
    gst_bin_add_many(GST_BIN(pipeline), source, demux, parse, filter, decodebin, sink, NULL);

    if (gst_element_link_many(demux, parse, filter, decodebin, sink, NULL) != TRUE) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        exit(1);
    }

    // Connect the pad-added signal of the source to handle dynamic pad linking
    g_signal_connect_object(source, "pad-added", G_CALLBACK(on_pad_added), demux, G_CONNECT_AFTER);
}

VideoWidget::~VideoWidget() {
    // Set the pipeline to NULL state and unref the pipeline to clean up
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}

// Method to set the RTSP URL
void VideoWidget::setRtspUrl(const QString &rtspUrl) {
    if (source) {
        // Set the location property of the rtspsrc element
        g_object_set(G_OBJECT(source), "location", rtspUrl.toUtf8().constData(), NULL);
        qDebug() << "RTSP URL set to" << rtspUrl;
    } else {
        qCritical() << "RTSP source element is not available.";
    }

    // Start the pipeline
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        qCritical() << "Unable to set the pipeline to the playing state.";
        gst_object_unref(pipeline);
    }
}

void VideoWidget::play() {
    // Set pipeline to playing state
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

void VideoWidget::pause() {
    // Set pipeline to paused state
    gst_element_set_state(pipeline, GST_STATE_PAUSED);
}

void VideoWidget::stop() {
    // Set pipeline to null state
    gst_element_set_state(pipeline, GST_STATE_NULL);
}


static void on_pad_added (GstElement *element, GstPad *pad, gpointer data){

    GstPad *sinkpad;
    GstElement *decoder = (GstElement *) data;

    /* We can now link this pad with the rtsp-decoder sink pad */
    g_print ("Dynamic pad created, linking source/demuxer\n");

    sinkpad = gst_element_get_static_pad (decoder, "sink");

    gst_pad_link (pad, sinkpad);

    gst_object_unref (sinkpad);
}
