/*
    Simple UDP Server Example
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFLEN 512
#define PORT 8888

void die(const char *s) {
    perror(s);
    exit(1);
}

int main(void) {
    struct sockaddr_in si_me, si_other;

    int s, recv_len;
    socklen_t slen = sizeof(si_other);
    char buf[BUFLEN];

    // Create a UDP socket
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }

    // Zero out the data structure
    memset((char *) &si_me, 0, sizeof(si_me));

    // Set necessary parameters
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind socket to port
    if (bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1) {
        die("bind");
    }

    // Keep listening for data
    while (1) {
        printf("Waiting for data on port %d ...", PORT);
        fflush(stdout);

        // Try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1) {
            die("recvfrom()");
        }

        // Print the details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);

        // Now, reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1) {
            die("sendto()");
        }
    }

    close(s);
    return 0;
}
