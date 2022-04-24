//
// Created by kaylor on 2022/1/31.
//
#include "rtppipeline.h"
#include "gst/gst.h"
#include <gst/app/gstappsrc.h>

// #define GST_TEST
// #define GST_VAAPI

typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *input_filter;
    GstElement *queue;
    GstElement *convert;
    GstElement *scale;
    GstElement *filter;
    GstElement *encode;
    GstElement *rtph264pay;
    GstElement *sink;
} CustomData;
CustomData data;

int initRTPPipeline(const char *ip, int port) {
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    /* Initialize GStreamer */
    gst_init(NULL, NULL);

    /* Create the elements */
    g_print("Create elements\n");
    data.source = gst_element_factory_make("appsrc", "source");
    data.input_filter = gst_element_factory_make("capsfilter", "input_filter");
    data.queue = gst_element_factory_make("queue", "queue");
#ifndef GST_VAAPI
    data.convert = gst_element_factory_make("videoconvert", "convert");
#else
    data.convert = gst_element_factory_make("vaapipostproc", "convert");
#endif
    data.scale = gst_element_factory_make("videoscale", "scale");
    data.filter = gst_element_factory_make("capsfilter", "filter");
#ifdef GST_TEST
    data.sink = gst_element_factory_make("autovideosink", "sink");
#else
#ifndef GST_VAAPI
    data.encode = gst_element_factory_make("x264enc", "encode");
#else
    data.encode = gst_element_factory_make("vaapih264enc", "encode");
#endif
    data.rtph264pay = gst_element_factory_make("rtph264pay", "rtph264pay");
    data.sink = gst_element_factory_make("udpsink", "sink");
#endif


    /* Create the empty pipeline */
    g_print("Create pipeline\n");
    data.pipeline = gst_pipeline_new("test-pipeline");
#ifdef GST_TEST
    if (!data.pipeline || !data.source || !data.input_filter || !data.queue || !data.convert || !data.scale || !data.filter || !data.sink) {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }
#else
    if (!data.pipeline || !data.source || !data.input_filter || !data.queue || !data.convert || !data.scale ||
        !data.filter || !data.encode || !data.rtph264pay || !data.sink) {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }
#endif

    g_print("Link elements\n");
#ifdef GST_TEST
    gst_bin_add_many(GST_BIN (data.pipeline), data.source, data.input_filter, data.queue, data.convert, data.scale, data.filter, data.sink, NULL);
    if (!gst_element_link_many(data.source, data.input_filter, data.queue, data.convert, data.scale, data.filter, data.sink, NULL)) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }
#else
    gst_bin_add_many(GST_BIN (data.pipeline), data.source, data.input_filter, data.queue, data.convert, data.scale,
                     data.filter, data.encode, data.rtph264pay, data.sink, NULL);
    if (!gst_element_link_many(data.source, data.input_filter, data.queue, data.convert, data.scale, data.filter,
                               data.encode, data.rtph264pay, data.sink, NULL)) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }
#endif
    g_print("Setting parameters\n");
//    GstCaps *caps;
//    caps = gst_caps_from_string("video/x-raw, format=I420 width=1920, height=1080, framerate=30/1");
//    g_object_set(data.source, "caps", caps, NULL);
    g_object_set(G_OBJECT (data.source), "caps",
                 gst_caps_new_simple("video/x-raw",
                                     "format", G_TYPE_STRING, "I420",
                                     "width", G_TYPE_INT, 1920,
                                     "height", G_TYPE_INT, 1440,
//                                     "framerate", GST_TYPE_FRACTION, 25, 1,
                                     NULL), NULL);
    g_object_set(G_OBJECT (data.source), "block", TRUE, NULL);
    GstCaps *caps;
    caps = gst_caps_from_string("video/x-raw,format=I420,width=1920,height=1440,framerate=30/1 ");
    g_object_set(data.input_filter, "caps", caps, NULL);
    /**变换比例的时候，图像做的是最小边匹配*/
    caps = gst_caps_from_string("video/x-raw, width=1920, height=720");
    g_object_set(data.filter, "caps", caps, NULL);

#ifndef GST_TEST
#ifndef GST_VAAPI
    g_object_set(data.encode, "tune", 4, "speed-preset", 1, NULL);
#else
    g_object_set(data.encode,"bitrate", 2048, "tune", 1, "cabac", TRUE, "compliance-mode", 1,NULL );
#endif
    g_object_set(data.rtph264pay, "pt", 35, "config-interval", 1, NULL);
    g_object_set(data.sink, "host", ip, "port", port, NULL);
#endif

    g_print("Change pipeline state\n");
    ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }
    g_print("starting...\r\n");
}

void pushRTPStreamRGB(unsigned char *streamdata, int dataLen) {
//    if (dataLen == 0){
//        return;
//    }
    GstBuffer *buf;
    GstMapInfo map;
    g_print("streamdata length = %d\n", dataLen);
    buf = gst_buffer_new_and_alloc (dataLen);
    gst_buffer_map(buf, &map, GST_MAP_WRITE);
    memcpy(map.data, streamdata, dataLen);
//    static unsigned char white = 0;
//    memset (map.data, white ? 0xff : 0x0, dataLen);
//    white =! white;

    gst_buffer_unmap(buf, &map);
    gst_app_src_push_buffer(GST_APP_SRC (data.source), buf);
}

void pushRTPStreamI420(AVFrame *pFrameYUV, int size) {
//    if (dataLen == 0){
//        return;
//    }
    GstBuffer *buf;
    GstMapInfo map;
//    g_print("size = %d\n", size);
    buf = gst_buffer_new_and_alloc (size * 3 / 2);
    gst_buffer_map(buf, &map, GST_MAP_WRITE);
    memcpy(map.data, pFrameYUV->data[0], size);
    memcpy(map.data + size, pFrameYUV->data[1], size / 4);
    memcpy(map.data + size + size / 4, pFrameYUV->data[2], size / 4);

    gst_buffer_unmap(buf, &map);
    gst_app_src_push_buffer(GST_APP_SRC (data.source), buf);
}

int testPipeline(const char *ip, int port, int iwidth, int iheight, int owidth, int oheight) {
    g_print("Create a pipeline\n");
    unsigned char cmd[1024] = {0};
#ifndef GST_VAAPI
    sprintf(cmd, "appsrc name=appsrc_element caps=\"video/x-raw,format=I420,width=%d,height=%d,framerate=30/1 \" ! \
          video/x-raw,format=I420,width=%d,height=%d,framerate=30/1 ! \
          queue ! videoconvert ! videoscale ! \
          video/x-raw,format=I420,width=%d,height=%d,framerate=30/1 ! \
          identity check-imperfect-timestamp=true ! x264enc tune=fastdecode speed-preset=ultrafast ! \
          rtph264pay config-interval=1 pt=35 ! udpsink host=%s port=%d",
            iwidth, iheight, iwidth, iheight, owidth, oheight, ip, port);
#else
    sprintf(cmd, "appsrc name=appsrc_element caps=\"video/x-raw,format=I420,width=%d,height=%d,framerate=30/1 \" ! \
          video/x-raw,format=I420,width=%d,height=%d,framerate=30/1 ! \
          queue ! vaapipostproc ! videoscale ! \
          video/x-raw,format=I420,width=%d,height=%d,framerate=30/1 ! \
          identity check-imperfect-timestamp=true ! vaapih264enc tune=high-compression bitrate=2048 cabac=true compliance-mode=restrict-buf-alloc ! \
          rtph264pay config-interval=1 pt=35 ! udpsink host=%s port=%d",
            iwidth, iheight, iwidth, iheight, owidth, oheight, ip, port);
#endif
    printf("%s", cmd);
    gst_init(NULL, NULL);
    data.pipeline =
            gst_parse_launch(cmd, NULL);
    // data.pipeline =
    //     gst_parse_launch("appsrc name=appsrc_element block=true !\
    //       video/x-raw,format=I420,width=1920,height=1440,framerate=30/1 ! \
    //       identity check-imperfect-timestamp=true ! videoconvert ! x264enc ! \
    //       video/x-h264,profile=\"high-4:4:4\" ! decodebin ! videoconvert ! autovideosink",
    //                      NULL);
    data.source = gst_bin_get_by_name(GST_BIN(data.pipeline), "appsrc_element");
    g_assert(data.source);
    g_assert(GST_IS_APP_SRC(data.source));
    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
}

void pushTestI420Data(unsigned char *I420data, int size) {
    GstBuffer *buf;
    GstMapInfo map;
    //    g_print("size = %d\n", size);
    buf = gst_buffer_new_and_alloc(size * 3 / 2);
    gst_buffer_map(buf, &map, GST_MAP_WRITE);
    memcpy(map.data, I420data, size * 3 / 2);
    gst_buffer_unmap(buf, &map);
    gst_app_src_push_buffer(GST_APP_SRC(data.source), buf);
}
