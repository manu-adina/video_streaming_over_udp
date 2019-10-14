#include <gst/gst.h> // G-Streamer






#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT



int createPipeline(void) {
    // Pipeline stack. TODO: Needs serious adjustment, won't work with out system
    pipeline = gst_pipeline_new("video_stream"); // Creates a pipeline with a given name
    source = gst_element_factory_make("mfw_v4lsrc", "vidsrc"); // 
    convcapsfilter = gst_element_factory_make("capsfilter", NULL);
    converter = gst_element_factory_make("mwf_ipucsc", "converter");
    encapsfilter = gst_element_factory_make("capsfilter", NULL);
    encoder = gst_element_factory_make("vpuenc", "encoder");
    payloader = gst_element_factory_make("rtph264pay", "payloader");
    sink = gst_element_factroy_make("udpsink", "netsink");



    // All created fine
    if (!pipeline || !source || !convcapsfilter || !converter || !encapsfilter || !encoder || !payloader || !sink) {
        std::cout << "Pipeline creation, unsuccessful" << std::endl;
        return -1;
    }

    // TODO: Above this line, need to create a pipeline that takes in OpenCV frames.


    // All the params are then set.


    return 0;
}

int main(int argc, char *argv[]) {

    
}




