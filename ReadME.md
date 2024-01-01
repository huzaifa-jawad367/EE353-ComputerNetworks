# Specifications
This project requires you to write code for a sender and receivers that implement reliable video file transfer over
UDP protocol using Linux/GNU C sockets. The sender is required to open a video file, read data chunks from the file
and write UDP segments, and send these segments on UDP. The receiver must be able to receive, reorder and
write
data to a file at the receiving end.
## Sender:
The sender will read the file specified by the filename and transfer it using UDP sockets. On completing the
transfer, the sender should terminate and exit. The sender should bind to listen port to receive
acknowledgments and another signal from the receiver. You should only use a single UDP socket for both
sending and receiving data. Note that although UDP will allow you to send large packets using IP
fragmentation, but make sure that you restrict your packets to 500 bytes (in the payload).
## Receiver:
The receiver will need to bind to the UDP port specified on the command line and receive a file from the sender
sent over the port. The file should be saved to a different filename. Note that you should make sure in
your testing that the filename used by the receiver is not the same as the one used by the sender. The receiver
should exit once the transfer is complete.
## Implementing reliability in UDP
You will be required to implement the following to make UDP reliable:
a) Sequence numbers
b) Retransmission (selective repeat)
c) Window size of 5-10 UDP segments (stop n wait)
d) Re-ordering on the receiver side
