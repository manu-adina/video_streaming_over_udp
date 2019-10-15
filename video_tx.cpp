#include <gst/gst.h> // G-Streamer
#include <cstdio>


#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720


#define GROUND_STATION_IP "192.168.0.124"
#define GROUND_STATION_PORT 5100

GstElement *pipeline;
GstElement *source;
GstElement *encoder_caps_filter;
GstElement *encoder;
GstElement *payloader;
GstElement *sink;
GstBus *bus;
GstMessage *msg;

int create_pipeline(void) {
    pipeline = gst_pipeline_new("video_stream"); // Creates a pipeline with a given name.
    source = gst_element_factory_make("v4l2src", "vidsrc"); // Captures video.
    encoder_caps_filter = gst_element_factory_make("capsfilter", NULL);
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
            "framerate", G_TYPE_INT, 30/1, 
            "width", G_TYPE_INT, 640, 
            "height", G_TYPE_INT, 480, NULL);

    g_object_set(G_OBJECT(payloader), "config-interval", 1, "pt", 96, NULL);
    g_object_set(G_OBJECT(sink), "host", GROUND_STATION_IP, "port", 5100, NULL); // Do we need "QOS-DSCP?"

    gst_bin_add_many(GST_BIN(pipeline), source, encoder_caps_filter, encoder, payloader, sink, NULL);

    return 0;
}

int destroy_pipeline(void) {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    g_object_unref(pipeline);
    return 0;
}

int main(int argc, char *argv[]) {

    gst_init(&argc, &argv);

    if(create_pipeline() < 0) {
        printf("Error with creating the pipeline");
        return -1;
    }
    
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    bus = gst_element_get_bus (pipeline);


    return 0;
}




