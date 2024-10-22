#include <gst/video/videooverlay.h>
#include "gstthread.h"

GstThread::GstThread(WId windowId, QString rtspUrl) {
    loop = g_main_loop_new(NULL, FALSE);

    // Create GStreamer elements
    pipeline = gst_pipeline_new("video player");
    source = gst_element_factory_make("rtspsrc", "source");
    demux = gst_element_factory_make("rtph264depay", "depay");
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


    // RTSP 소스 설정
    g_object_set(source, "location", rtspUrl.toUtf8().constData(), NULL);

    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), windowId);
}


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


// GStreamer 버스 메시지 핸들러 함수
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
    GstThread *gstThread = (GstThread *)data;

    // stopped 변수가 true일 경우 루프 종료
    if (gstThread->stopped) {
        g_print("Stopped signal received. Quitting the loop.\n");
        g_main_loop_quit(gstThread->loop);
        return FALSE;  // 타이머를 중지
    }

    return TRUE;  // 계속 체크
}

void GstThread::run() {
    stopped = false;
    // `pad-added` 신호를 받아 동적으로 depayloader에 연결
    g_signal_connect(source, "pad-added", G_CALLBACK(on_pad_added), demux);

    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_call, loop);
    gst_object_unref(bus);

    // stopped 변수를 주기적으로 확인
    g_timeout_add(100, check_stopped, this);  // 100ms마다 상태 확인

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    g_main_loop_run(loop);

    // 메인 루프가 종료된 후 파이프라인 정리
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
