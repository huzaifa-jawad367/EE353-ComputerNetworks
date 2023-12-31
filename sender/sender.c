#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PACKET_SIZE 500
#define WINDOW_SIZE 10  // Example window size
#define PORT 8080

// Packet structure definition
struct packet {
    int32_t seq_num;
    char data[PACKET_SIZE];
    int32_t len;
};

// Function to serialize the packet
void serialize_packet(struct packet *pkt, char *buffer) {
    int32_t seq_num_net = htonl(pkt->seq_num); // Convert to network byte order
    int32_t len_net = htonl(pkt->len); // Convert to network byte order

    memcpy(buffer, &seq_num_net, sizeof(seq_num_net));
    memcpy(buffer + sizeof(seq_num_net), pkt->data, PACKET_SIZE);
    memcpy(buffer + sizeof(seq_num_net) + PACKET_SIZE, &len_net, sizeof(len_net));
}

// Function to check if the window is full
int is_window_full(int base, int next_seq_num) {
    return next_seq_num >= base + WINDOW_SIZE;
}

int main() {
    // Initialize socket
    int sockfd;
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_port = htons(PORT); // Port number
    servaddr.sin_addr.s_addr = INADDR_ANY; // For localhost

    // Open the file
    char *filename = "input.mp4";
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    int seq_num = 0;
    int base = 0;
    int next_seq_num = 0;

    // Main loop to read from the file and send packets
    while (!feof(file)) {
        if (!is_window_full(base, next_seq_num)) {
            struct packet pkt;
            pkt.seq_num = seq_num;
            pkt.len = fread(pkt.data, 1, PACKET_SIZE, file);

            if (pkt.len < 0) {
                perror("Failed to read file");
                exit(EXIT_FAILURE);
            }

            // TODO: Implement packet serialization if necessary
            char serialized_pkt[sizeof(struct packet)];
            serialize_packet(&pkt, serialized_pkt);

            ssize_t sent_bytes = sendto(sockfd, serialized_pkt, sizeof(serialized_pkt), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
            if (sent_bytes < 0) {
                perror("Failed to send packet");
                exit(EXIT_FAILURE);
            }

            // TODO: Send the packet
            ssize_t sent_bytes = sendto(sockfd, (const char *)&pkt, sizeof(pkt), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
            if (sent_bytes < 0) {
                perror("Failed to send packet");
                exit(EXIT_FAILURE);
            }

            // TODO: Add packet to unacknowledged window
            // ...

            next_seq_num++;
        }

        // TODO: Process any received acknowledgments
        // ...

        // TODO: Handle retransmission if needed
        // ...

        // TODO: Update window and check for transmission completion
        // ...
    }

    // Close file and socket
    fclose(file);
    close(sockfd);

    // Terminate sender
    return 0;
}

