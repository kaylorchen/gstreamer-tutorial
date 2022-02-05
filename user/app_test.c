//
// Created by kaylor on 2022/2/1.
//
#include "gst/gst.h"
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *queue;
    GstElement *convert;
    GstElement *sink;
} CustomData;
CustomData _customdata;
int initRTPPipeline() {
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    /* Initialize GStreamer */
    gst_init (NULL, NULL);

    /* Create the elements */
    g_print("Create elements\n");
    _customdata.source = gst_element_factory_make ("appsrc", "source");
    g_assert (_customdata.source);
    _customdata.queue = gst_element_factory_make("queue", "queue");
    _customdata.convert = gst_element_factory_make ("videoconvert", "convert");
    _customdata.sink = gst_element_factory_make ("autovideosink", "sink");

    /* Create the empty pipeline */
    g_print("Create pipeline\n");
    _customdata.pipeline = gst_pipeline_new ("test-pipeline");
    if (!_customdata.pipeline || !_customdata.source || !_customdata.queue || !_customdata.convert || !_customdata.sink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }
    g_print("Link elements\n");
    gst_bin_add_many (GST_BIN (_customdata.pipeline), _customdata.source,  _customdata.convert, _customdata.sink, NULL);
    if (!gst_element_link_many (_customdata.source,  _customdata.convert, _customdata.sink, NULL)) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (_customdata.pipeline);
        return -1;
    }
    g_print("Setting parameters\n");
//    GstCaps *caps;
//    caps = gst_caps_from_string("video/x-raw, format=I420 width=1920, height=1080, framerate=1/1");
//    g_object_set(_customdata.source, "caps", caps, NULL);
        g_object_set (G_OBJECT (_customdata.source), "caps",
                      gst_caps_new_simple ("video/x-raw",
                                       "format", G_TYPE_STRING, "RGB",
                                       "width", G_TYPE_INT, 1920,
                                       "height", G_TYPE_INT, 1080,
                                       "framerate", GST_TYPE_FRACTION, 30, 1,
                                       NULL), NULL);

    g_print("Change pipeline state\n");
    ret = gst_element_set_state (_customdata.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (_customdata.pipeline);
        return -1;
    }
    g_print("starting...\r\n");
}

void pushRTPStream(unsigned char *data, int dataLen) {
    static unsigned char white = 0;
    if (dataLen == 0){
        return;
    }
    GstBuffer *buf;
    GstMapInfo map;
    static GstClockTime timestamp = 0;
    g_print("_customdata length = %d\n", dataLen);
    buf = gst_buffer_new_and_alloc (dataLen);
    gst_buffer_map (buf, &map, GST_MAP_WRITE);
//    memcpy(map._customdata, _customdata, dataLen);
    memset (map.data, white ? 0xff : 0x0, dataLen);
    white =! white;
    gst_buffer_unmap (buf, &map);
//    CustomData *app = (CustomData *)&data;
    gst_app_src_push_buffer (GST_APP_SRC (_customdata.source), buf);
//        buf = gst_buffer_new_allocate (NULL, dataLen, NULL);
//    gst_buffer_map (buf, &map, GST_MAP_WRITE);
////    memcpy( (guchar *)map._customdata, data1,  gst_buffer_get_size( buffer ) );
//    /* this makes the image black/white */
//    gst_buffer_memset (buf, 0, white ? 0xff : 0x0, dataLen);
//
//    white = !white;
//
//    GST_BUFFER_PTS (buf) = timestamp;
//    GST_BUFFER_DURATION (buf) = gst_util_uint64_scale_int (1, GST_SECOND, 2);
//
//    timestamp += GST_BUFFER_DURATION (buf);
//
//    g_signal_emit_by_name (appsrc, "push-buffer", buffer, &ret);
}

int main()
{
    initRTPPipeline();
    unsigned char data[10];
    while (1){
        pushRTPStream(data, 1920*1080*3);
        g_usleep(1000000);
    }
}

//#include <gst/gst.h>
//
//static GMainLoop *loop;
//
//static void
//cb_need_data (GstElement *appsrc,
//              guint       unused_size,
//              gpointer    user_data)
//{
//    static gboolean white = FALSE;
//    static GstClockTime timestamp = 0;
//    GstBuffer *buffer;
//    guint size,depth,height,width,step,channels;
//    GstFlowReturn ret;
//    guchar *data1;
//    GstMapInfo map;
//
//    size = 1920*1080*3;
//
//    buffer = gst_buffer_new_allocate (NULL, size, NULL);
//    gst_buffer_map (buffer, &map, GST_MAP_WRITE);
////    memcpy( (guchar *)map._customdata, data1,  gst_buffer_get_size( buffer ) );
//    /* this makes the image black/white */
//    gst_buffer_memset (buffer, 0, white ? 0xff : 0x0, size);
//
//    white = !white;
//
//    GST_BUFFER_PTS (buffer) = timestamp;
//    GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale_int (1, GST_SECOND, 2);
//
//    timestamp += GST_BUFFER_DURATION (buffer);
//
//    g_signal_emit_by_name (appsrc, "push-buffer", buffer, &ret);
//
//    if (ret != GST_FLOW_OK) {
//        /* something wrong, stop pushing */
//        g_main_loop_quit (loop);
//    }
//}
//
//gint
//main (gint   argc,
//      gchar *argv[])
//{
//    GstElement *pipeline, *appsrc, *conv, *videosink;
//
//    /* init GStreamer */
//    gst_init (&argc, &argv);
//    loop = g_main_loop_new (NULL, FALSE);
//
//    /* setup pipeline */
//    pipeline = gst_pipeline_new ("pipeline");
//    appsrc = gst_element_factory_make ("appsrc", "source");
//    conv = gst_element_factory_make ("videoconvert", "conv");
//    videosink = gst_element_factory_make ("autovideosink", "videosink");
//
//    /* setup */
//    g_object_set (G_OBJECT (appsrc), "caps",
//                  gst_caps_new_simple ("video/x-raw",
//                                       "format", G_TYPE_STRING, "RGB",
//                                       "width", G_TYPE_INT, 640,
//                                       "height", G_TYPE_INT, 360,
//                                       "framerate", GST_TYPE_FRACTION, 1, 1,
//                                       NULL), NULL);
//    gst_bin_add_many (GST_BIN (pipeline), appsrc, conv, videosink, NULL);
//    gst_element_link_many (appsrc, conv, videosink, NULL);
//    //g_object_set (videosink, "device", "/dev/video0", NULL);
//    /* setup appsrc */
//    g_object_set (G_OBJECT (appsrc),
//                  "stream-type", 0,
//                  "format", GST_FORMAT_TIME, NULL);
//    g_signal_connect (appsrc, "need-_customdata", G_CALLBACK (cb_need_data), NULL);
//
//    /* play */
//    gst_element_set_state (pipeline, GST_STATE_PLAYING);
//    g_main_loop_run (loop);
//
//    /* clean up */
//    gst_element_set_state (pipeline, GST_STATE_NULL);
//    gst_object_unref (GST_OBJECT (pipeline));
//    g_main_loop_unref (loop);
//
//    return 0;
//}
