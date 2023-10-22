#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 256

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char audio_buffer[MAX_BUFFER_SIZE];
    
    // Open and read the audio file
    FILE* audio_file = fopen("Calling.wav", "rb");
    if (audio_file == NULL) {
        perror("Error opening audio file");
        exit(1);
    }

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the receiver
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to the receiver");
        close(sockfd);
        exit(1);
    }

    while (1) {
        // Read a chunk of data from the audio file
        int bytes_read = fread(audio_buffer, 1, MAX_BUFFER_SIZE, audio_file);
        if (bytes_read <= 0) {
            // End of file
            break;
        }

        // Send the data over TCP
        if (send(sockfd, audio_buffer, bytes_read, 0) == -1) {
            perror("Error sending data");
            break;
        }
    }

    // Indicate the end of the file
    shutdown(sockfd, SHUT_WR);

    // Close the socket and the file
    close(sockfd);
    fclose(audio_file);

    

    return 0;
}
