#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 256

int main() {
    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char audio_buffer[MAX_BUFFER_SIZE];
    
    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(sockfd, 10) == -1) {
        perror("Error listening");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);

    // Open a new audio file for writing
    FILE* audio_file = fopen("received_audio.wav", "wb");
    if (audio_file == NULL) {
        perror("Error opening audio file for writing");
        exit(1);
    }

    while (1) {
        int bytes_received = recv(new_sock, audio_buffer, MAX_BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            // End of file or error
            break;
        }

        // Write received data to the audio file
        fwrite(audio_buffer, 1, bytes_received, audio_file);
    }

    // Close the file and socket
    fclose(audio_file);
    close(new_sock);
    close(sockfd);

    return 0;
}
