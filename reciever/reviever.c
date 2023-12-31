#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PACKET_SIZE 500
#define WINDOW_SIZE 10  // Example window size

// Function to process packets and write to file
// ...

int main() {
    // Initialize socket
    // ...

    // Open the file for writing
    // ...

    // Main loop to receive packets
    while (1) {
        char buffer[PACKET_SIZE];

        ////////////////////////// Your code for Receiving Packet here /////////////////////////////////
        // ...

        ////////////////////////// Your code for Sequence Number Checking here /////////////////////////
        // ...

        ////////////////////////// Your code for Acknowledgment Sending here ///////////////////////////
        // ...

        ////////////////////////// Your code for Re-ordering here //////////////////////////////////////
        // ...

        // Write the processed packet to the file
        // ...

        // Check for end of file transfer
        // ...
    }

    // Close file and socket
    // ...

    // Terminate receiver
    return 0;
}
