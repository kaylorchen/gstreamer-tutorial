//
// Created by kaylor on 2022/1/30.
//

#include "gst/gst.h"
typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *filter;
    GstElement *queue;
    GstElement *convert;
    GstElement *encode;
    GstElement *rtph264pay;
    GstElement *sink;
} CustomData;
// send: gst-launch-1.0 v4l2src device=/dev/video0 ! \
// 'video/x-raw, width=640, height=480, framerate=30/1'!\
// queue ! videoconvert ! x264enc tune=zerolatency speed-preset=ultrafast !\
// rtph264pay config-interval=1 pt=35 ! udpsink host=127.0.0.1 port=5022

// receive: gst-launch-1.0 udpsrc port=5022 caps='application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264' !\
// rtph264depay ! avdec_h264 ! videoconvert ! clockoverlay valignment=bottom ! autovideosink
int main(int argc, char *argv[]){
    CustomData data;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    /* Initialize GStreamer */
    gst_init (&argc, &argv);

    /* Create the elements */
    g_print("Create elements\n");
    data.source = gst_element_factory_make ("v4l2src", "source");
    data.filter = gst_element_factory_make("capsfilter", "filter");
    data.convert = gst_element_factory_make ("videoconvert", "convert");
    data.encode = gst_element_factory_make("x264enc", "encode");
    data.rtph264pay = gst_element_factory_make("rtph264pay", "rtph264pay");
    data.sink = gst_element_factory_make ("udpsink", "sink");

    /* Create the empty pipeline */
    g_print("Create pipeline\n");
    data.pipeline = gst_pipeline_new ("test-pipeline");
    if (!data.pipeline || !data.source || !data.filter || !data.convert || !data.encode || !data.rtph264pay || !data.sink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }
    g_print("Link elements\n");
    gst_bin_add_many (GST_BIN (data.pipeline), data.source, data.filter, data.convert, data.encode, data.rtph264pay,data.sink, NULL);
    if (!gst_element_link_many (data.source, data.filter, data.convert, data.encode, data.rtph264pay,data.sink, NULL)) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (data.pipeline);
        return -1;
    }
    g_print("Setting parameters\n");
    g_object_set(data.source, "device", "/dev/video0", "io-mode", 2, NULL);
    GstCaps *caps;
    caps = gst_caps_from_string("video/x-raw, width=640, height=480, framerate=30/1");
    g_object_set(data.filter, "caps", caps, NULL);
    g_object_set(data.encode, "tune", 4, "speed-preset", 1, NULL);
    g_object_set(data.rtph264pay, "pt", 35, "config-interval", 1, NULL);
    g_object_set(data.sink, "host", "127.0.0.1", "port", 5600, NULL);

    g_print("Change pipeline state\n");
    ret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (data.pipeline);
        return -1;
    }
    g_print("starting...\r\n");

    /* Wait until error or EOS */
    bus = gst_element_get_bus (data.pipeline);
    msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* Parse message */
    if (msg != NULL) {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE (msg)) {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error (msg, &err, &debug_info);
                g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
                g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
                g_clear_error (&err);
                g_free (debug_info);
                break;
            case GST_MESSAGE_EOS:
                g_print ("End-Of-Stream reached.\n");
                break;
            default:
                /* We should not reach here */
                g_printerr ("Unexpected message received.\n");
                break;
        }
        gst_message_unref (msg);
    }
    /* Free resources */
    g_print("Free resources\n");
    gst_object_unref (bus);
    gst_element_set_state (data.pipeline, GST_STATE_NULL);
    gst_object_unref (data.pipeline);
    return 0;
}