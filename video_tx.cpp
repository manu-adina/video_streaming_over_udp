#include <gst/gst.h> // G-Streamer
#include <cstdio>

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480
#define GROUND_STATION_IP "192.168.0.124"
#define GROUND_STATION_PORT 5801

GstElement *pipeline;
GstElement *source;
GstElement *encoder_caps_filter;
GstElement *encoder;
GstElement *payloader;
GstElement *sink;
GstBus *bus;
GstMessage *msg;
GstStateChangeReturn ret;
GMainLoop *loop;

int create_pipeline(void) {
    pipeline = gst_pipeline_new("video_stream"); // Creates a pipeline with a given name.
    source = gst_element_factory_make("v4l2src", "vidsrc"); // Captures video.
    encoder_caps_filter = gst_element_factory_make("capsfilter", NULL); // Specifies framerate, W and H
    encoder = gst_element_factory_make("omxh264enc", "encoder"); // Converts to h264.
    payloader = gst_element_factory_make("rtph264pay", "payloader"); // Encapsulates in RTP.
    sink = gst_element_factory_make("udpsink", "netsink"); // Sends UDP packets to port.

    // All created fine
    if (!pipeline || !source || !encoder_caps_filter || !encoder || !payloader || !sink) {
        printf("Error creating the pipeline\n");
        printf("Encoder initiated? : %d\n", source);
        return -1;
    }

    // TODO: Above this line, need to create a pipeline that takes in OpenCV frames.

    
    // Setting sink params.
    g_object_set(G_OBJECT(source), "device", "/dev/video0", NULL);

    GstCaps *encoder_caps;
    encoder_caps = gst_caps_new_simple("video/x-raw", 
            "framerate", GST_TYPE_FRACTION, 30, 1, 
            "width", G_TYPE_INT, DEFAULT_WIDTH, 
            "height", G_TYPE_INT, DEFAULT_HEIGHT, NULL);

    g_object_set(G_OBJECT(payloader), "config-interval", 1, "pt", 96, NULL);
    g_object_set(encoder_caps_filter, "caps", encoder_caps, NULL);
    g_object_set(G_OBJECT(sink), "host", GROUND_STATION_IP, "port", GROUND_STATION_PORT, NULL); // Do we need "QOS-DSCP?"

    gst_caps_unref(encoder_caps);

    gst_bin_add_many(GST_BIN(pipeline), source, encoder_caps_filter, encoder, payloader, sink, NULL);
    
    if(gst_element_link_many(source, encoder_caps_filter, encoder, payloader, sink, NULL) != TRUE) {
        printf("Failed to link!\n"); 
        return -1;
    }

    return 0;
}

void destroy_pipeline(void) {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    g_object_unref(pipeline);
}

int main(int argc, char *argv[]) {

    gst_init(&argc, &argv);
    loop = g_main_loop_new(NULL, FALSE);

    if(create_pipeline() < 0) {
        printf("Error with creating the pipeline\n");
        return -1;
    }
    
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_main_loop_run(loop);

    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
        return -1;              
    }

    destroy_pipeline();
    g_main_loop_unref(loop);
 
    printf("Did it even play?\n");

    return 0;
}
