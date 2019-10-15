#include <gst/gst.h> // G-Streamer


#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT


#define GROUND_STATION_IP "192.168.0.124"
#define GROUND_STATION_PORT 5100

int createPipeline(void) {
    pipeline = gst_pipeline_new("video_stream"); // Creates a pipeline with a given name.
    source = gst_element_factory_make("v4lsrc", "vidsrc"); // Captures video.
    encoder_caps_filter = gst_element_factory_make("encoder_caps_filter", NULL);
    encoder = gst_element_factory_make("omxh264enc", "encoder"); // Converts to h264.
    payloader = gst_element_factory_make("rtph264pay", "payloader"); // Encapsulates in RTP.
    sink = gst_element_factroy_make("udpsink", "netsink"); // Sends UDP packets to port.

    // All created fine
    if (!pipeline || !source || !encoder || !payloader || !sink) {
        std::cout << "Pipeline creation was unsuccessful" << std::endl;
        return -1;
    }

    // TODO: Above this line, need to create a pipeline that takes in OpenCV frames.

    
    // Setting sink params.
    g_object_set(G_OBJECT(source), "device", "/dev/video0", NULL);

    GstCaps *encoder_caps_filter;
    encoder_caps_filter = gst_caps_new_simple("video/x-raw", "framerate", G_TYPE_INT, 30/1, 
            "width", G_TYPE_INT, 640, "height", G_TYPE_INT, 480, NULL);

    g_object_set(G_OBJECT(payloader), "config-interval", 1, "pt", 96, NULL);
    g_object_set(G_OBJECT(sink), "host", GROUND_STATION_IP, "port", 5100, NULL); // Do we need "QOS-DSCP?"






    

        

    // All the params are then set.


    return 0;
}

int main(int argc, char *argv[]) {

    // mmal needs to capture a frame here.



    
}




