#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PACKET_SIZE 500
#define PORT 8080

// Packet structure definition
struct packet {
    int32_t seq_num;
    char data[PACKET_SIZE];
    int32_t len;
};

// Function to deserialize the packet
void deserialize_packet(char *buffer, struct packet *pkt) {
    int32_t seq_num_net, len_net;

    memcpy(&seq_num_net, buffer, sizeof(seq_num_net));
    pkt->seq_num = ntohl(seq_num_net); // Convert from network byte order

    memcpy(pkt->data, buffer + sizeof(seq_num_net), PACKET_SIZE);

    memcpy(&len_net, buffer + sizeof(seq_num_net) + PACKET_SIZE, sizeof(len_net));
    pkt->len = ntohl(len_net); // Convert from network byte order
}

// Function to send acknowledgment
void send_acknowledgment(int sockfd, struct sockaddr_in *sender_addr, int32_t ack_seq_num) {
    int32_t ack_seq_num_net = htonl(ack_seq_num);
    sendto(sockfd, &ack_seq_num_net, sizeof(ack_seq_num_net), 0, (struct sockaddr *)sender_addr, sizeof(*sender_addr));
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&sender_addr, 0, sizeof(sender_addr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    char buffer[sizeof(struct packet)];
    struct packet pkt;
    
    // Open the file for writing
    FILE *file = fopen("output.mp4", "wb");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    while (1) {
        ssize_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&sender_addr, &sender_addr_len);
        if (len > 0) {
            deserialize_packet(buffer, &pkt);

            // TODO: Process packet (Reorder based on sequence number, write to file, etc.)
            // ...

            // Send acknowledgment
            send_acknowledgment(sockfd, &sender_addr, pkt.seq_num);
        }

        // TODO: Check for end of transmission and break the loop
        // ...
    }

    // Close file and socket
    fclose(file);
    close(sockfd);

    return 0;
}