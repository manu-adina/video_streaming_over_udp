#include <sys/socket.h>
#include <poll>

#define UDP_PORT 5000
#define UDP_IP "192.168.0.124"


static const unsigned int max_packet_bytes = 2000;
static const unsigned int max_frame_packets = 128;

struct pkt {
    char buf[max_packet_bytes];
    uint16_t len;
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

    // TODO: Don't really understands what's happening under this block. Need to figure it out. 
    // Set socket to send as well. 
    if(setsockopt(fd, IPPROTO_IP, IP_TOS, &tos, sizeof(tos)) != 0) {
        std::cout << "Error setting socket options" << std::endl;
        return -1;
    }

    return fd;
}


int main(int argc, const char *argv[]) {

    bool frame_complete = false;
    unsigned int frame_pkt_idx = 0;
    struct pkt frame_pkts[max_frame_packets];
    int small_packet_count = 0;


    // For drop packet information.
    int seq = 0;
    int last_seq = 0;
    int drops = 0;

    // Create a socket for receiving RTP video packets.
    int fd = create_socket(app_video_tos);
    if(fd < 0) {
        return 1;
    }

    // Setting up polling information.
    const int num_fds = 1;
    struct pollfd fd_set[num_fds];
    memset(fd_set, 0, sizeof(fd_set));
    fd_set[0].fd = fd;
    fd_set[0].event = POLLIN; // Setting an event that there is data to read.

    // Handle downlink video packets.
    while(1) {

        // TODO: Need to understand this time stuff. Why even need it
        //uint64_t now_us = clock_gettime_us(CLOCK_MONOTONIC);
        int timeout_ms = 100;

        //if(log_time_us < app_check_us) timeout_ms = (log_time_us - now_us) / 1000 + 1;
        //else timeout_ms = (app_check_us - now_us) / 1000 + 1;

        //if(timeout_ms < 0) timeout_ms = 0;

        // if poll returns with an error. fd_set is unmodified so putting revents as 0
        // to prevent false-detection.
        fd_set[0].revents = 0;
        int pollrc = poll(fd_set, num_fds, timeout_ms);
        
        // Poll info is returned and that there is data to be read, receive packets.
        if((pollrc > 0) && (fd_set[0].revents & POLLIN)) {
            char pkt_buf[max_packet_bytes];
            ssize_t pkt_bytes = recv(fd, pkt_buf, sizeof(pkt_buf),0);

            //now_us = clock_gettime_us(CLOCK_MONOTONIC);

            if(pkt_bytes <= 0) {
                std::cout << "Error receiving" << std::endl;
                sleep(1)
            } else {
                // Process the packet if it was received properly.

                // Extracting the sequency number from the header.
                seq = pkt_buf[2] << 8 | pkt_buf[3];
                if(seq > (last_seq + 1) && last_seq && seq > last_seq)
                    drops += (seq - last_seq - 1);
                
                last_seq = seq;

                memcpy(frame_pkts[frame_pkt_idx].buf, pkt_buf, pkt_bytes);
                frame_pkts[frame_pkt_idx].len = pkt_bytes;

                // If this is a full frame, send it. Or if out of memory, send it.
                // Note 0x5c indicates that it's the last packet.
                // and 0x81 indicates some FEC information. Not sure why 0x81 is used.
                if((pkt_buf[12] == 0x5c && pkt_buf[13] == 0x81) || frame_pkt_idx == max_frame_packets - 1)
                    frame_complete = true;
                
                if (pkt_bytes < 128) {
                    if(small_pkt_count >= 2)
                        frame_complete = true;
                    else 
                        ++small_pkt_count;
                } else if(small_pkt_count > 0) 
                    small_pkt_count = 0;
                
                packet_count++;
                bytes_count += pkt_bytes;

                if(frame_complete) {
                    // Send full frame packets.
                    
                } else {
                    ++frame_pkt_idx;
                    if(frame_pkt_idx >= max_frame_packets) {
                        frame_pkt_idx = 0;
                        continue;
                    }
                }
            }
        
        } else {
            //now_us = clock_gettime_us(CLOCK_MONOTONIC);
        }
    } // end while
    return 0;
}
