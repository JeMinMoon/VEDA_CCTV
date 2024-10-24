#include <gst/video/videooverlay.h>
#include <gst/base/gstbaseparse.h>
#include "gstthread.h"

static void on_pad_added(GstElement *src, GstPad *pad, gpointer data) {
    GstElement *depayloader = (GstElement *)data;
    GstPad *sink_pad = gst_element_get_static_pad(depayloader, "sink");
    if (gst_pad_is_linked(sink_pad)) {
        g_print("Pad already linked. Ignoring.\n");
        g_object_unref(sink_pad);
        return;
    }
    if (gst_pad_link(pad, sink_pad) != GST_PAD_LINK_OK) {
        g_printerr("Failed to link pads.\n");
    } else {
        g_print("Pad linked successfully.\n");
    }
    g_object_unref(sink_pad);
}

GstThread::GstThread(WId windowId, QString rtspUrl)
    : windowId(windowId), rtspUrl(rtspUrl), stopped(false) {

    loop = g_main_loop_new(NULL, FALSE);

    // Create pipeline elements
    pipeline = gst_pipeline_new("video-player");
    source = gst_element_factory_make("rtspsrc", "source");
    depay = gst_element_factory_make("rtph264depay", "depay");
    parse1 = gst_element_factory_make("h264parse", "parse1");
    tee = gst_element_factory_make("tee", "tee");

    // Display branch
    queue_display = gst_element_factory_make("queue", "queue_display");
    decoder = gst_element_factory_make("openh264dec", "decoder");
    videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    displaysink = gst_element_factory_make("glimagesink", "displaysink");

    // Recording branch
    queue_record = gst_element_factory_make("queue", "queue_record");
    parse2 = gst_element_factory_make("h264parse", "parse2");
    timestamper = gst_element_factory_make("h264timestamper", "timestamper");
    mux = gst_element_factory_make("mp4mux", "mux");
    filesink = gst_element_factory_make("filesink", "filesink");

    // Check all elements were created successfully
    if (!pipeline || !source || !depay || !parse1 || !tee ||
        !queue_display || !decoder || !videoconvert || !displaysink ||
        !queue_record || !parse2 || !timestamper || !mux || !filesink) {
        g_printerr("One or more elements could not be created. Exiting.\n");
        return;
    }

    // Configure source
    g_object_set(G_OBJECT(source), "location", rtspUrl.toUtf8().constData(), NULL);
    g_object_set(G_OBJECT(source), "protocols", 0x00000004, NULL);  // Force TCP (4)
    g_object_set(G_OBJECT(filesink), "location", "C:/Users/change11/Desktop/video/Output.mp4", NULL);

    // Configure muxer for faster finalization
    g_object_set(G_OBJECT(mux),
                 "faststart", TRUE,           // 빠른 시작 활성화
                 "movie-timescale", 90000,    // 높은 타임스케일로 정밀도 향상
                 "fragment-duration", 1000,    // 1초마다 프래그먼트 생성
                 NULL);

    // Add elements to pipeline
    gst_bin_add_many(GST_BIN(pipeline),
                     source, depay, parse1, tee,
                     queue_display, decoder, videoconvert, displaysink,
                     queue_record, parse2, timestamper, mux, filesink,
                     NULL);

    // Link elements (except source which will be linked dynamically)
    if (!gst_element_link_many(depay, parse1, tee, NULL)) {
        g_printerr("Failed to link depay -> parse1 -> tee\n");
        return;
    }

    // Link display branch
    if (!gst_element_link_many(queue_display, decoder, videoconvert, displaysink, NULL)) {
        g_printerr("Failed to link display branch\n");
        return;
    }

    // Link recording branch
    if (!gst_element_link_many(queue_record, parse2, timestamper, mux, filesink, NULL)) {
        g_printerr("Failed to link recording branch\n");
        return;
    }

    // Link tee pads
    GstPad *tee_display_pad = gst_element_request_pad_simple(tee, "src_%u");
    GstPad *queue_display_pad = gst_element_get_static_pad(queue_display, "sink");
    if (GST_PAD_LINK_FAILED(gst_pad_link(tee_display_pad, queue_display_pad))) {
        g_printerr("Failed to link tee -> display queue\n");
    }
    gst_object_unref(queue_display_pad);
    gst_object_unref(tee_display_pad);

    GstPad *tee_record_pad = gst_element_request_pad_simple(tee, "src_%u");
    GstPad *queue_record_pad = gst_element_get_static_pad(queue_record, "sink");
    if (GST_PAD_LINK_FAILED(gst_pad_link(tee_record_pad, queue_record_pad))) {
        g_printerr("Failed to link tee -> record queue\n");
    }
    gst_object_unref(queue_record_pad);
    gst_object_unref(tee_record_pad);

    // Set up video overlay
    if (GST_IS_VIDEO_OVERLAY(displaysink)) {
        gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(displaysink), windowId);
    }
}

static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data) {
    GMainLoop *loop = (GMainLoop *)data;

    switch (GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_EOS:
        g_print("End of stream\n");
        g_main_loop_quit(loop);
        break;

    case GST_MESSAGE_ERROR: {
        gchar *debug;
        GError *error;
        gst_message_parse_error(msg, &error, &debug);
        g_free(debug);
        g_printerr("Error: %s\n", error->message);
        g_error_free(error);
        g_main_loop_quit(loop);
        break;
    }
    default:
        break;
    }
    return TRUE;
}

static gboolean check_stopped(gpointer data) {
    GstThread *thread = (GstThread *)data;
    if (thread->stopped) {
        g_print("Stop signal received\n");
        g_main_loop_quit(thread->loop);
        return FALSE;
    }
    return TRUE;
}

void GstThread::run() {
    // Connect pad-added signal
    g_signal_connect(source, "pad-added", G_CALLBACK(on_pad_added), depay);

    // Add bus watch
    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_call, loop);
    gst_object_unref(bus);

    // Add stopped check timer
    g_timeout_add(100, check_stopped, this);

    // Start playing
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        return;
    }

    g_main_loop_run(loop);

    // Clean up
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_main_loop_unref(loop);
}

void GstThread::stop() {
    stopped = true;
}

void GstThread::SetRtspUrl(QString rtspUrl) {
    this->rtspUrl = rtspUrl;
}

void GstThread::SetWindowId(WId windowId) {
    this->windowId = windowId;
}
