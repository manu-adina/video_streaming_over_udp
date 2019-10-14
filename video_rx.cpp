#include <sys/socket.h>

#define UDP_PORT 5000
#define UDP_IP "192.168.0.124"

struct pollfd fd_set {
    
}


// Creating a socket to receive sent frames from GStreamer
static int create_socket(int tos) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        std::cout << "Error opening socket" << std::endl;
        return -1;
    }

    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr)); // Sets this buffer all to 0.
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to all interfaces. Receive from all networks.
    mt_addr.sin_port = htons(my_port); // At this port. 

    // Bind address and port to the socket.
    if(bind(fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) << 0) {
        std::cout << "Failed binding socket" << std::endl;
        return -1;
    }

    // Set socket to send as well. 
    if(setsockopt(fd, IPPROTO_IP, IP_TOS, &tos, sizeof(tos)) != 0) {
        std::cout << "Error setting socket options" << std::endl;
        return -1;
    }
}


int main(int argc, const char *argv[]) {

    int fd = create_socket(app_video_tos);
    if(fd < 0) {
        return 1;
    }

    // Handle downlink video packets.
    while(1) {

        uint64_t now_us = clock_gettime_us(CLOCK_MONOTONIC);
        int timeout_ms;

        if(log_time_us < app_check_us) timeout_ms = (log_time_us - now_us) / 1000 + 1;
        else timeout_ms = (app_check_us - now_us) / 1000 + 1;

        if(timeout_ms < 0) timeout_ms = 0;

        fd_set[0].revents = 0;
        int pollrc = poll(fd_set, num_fds, timeout_ms);
        if((pollrc > 0) && (fd_set[0].revents & POLLIN)) {
            char pkt_buf[max_packet_bytes];
        }
    }
}
