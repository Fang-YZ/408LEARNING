/*
 * ex2_echo_client.c  --  N2 classroom example (layered architecture)
 *
 * A TCP client that talks to ex1_echo_server.exe.
 * One-shot mode:  .\ex2_echo_client.exe <host> <port> <text>
 * Interactive:    .\ex2_echo_client.exe <host> <port>     (type lines, Ctrl+Z then Enter to quit)
 *
 * Build:  gcc -Wall -Wextra -O2 -o ex2_echo_client.exe ex2_echo_client.c -lws2_32
 *
 * Required output (English, line by line):
 *   CONNECTED <host>:<port>
 *   SENT <n> bytes        <- n = strlen(text) + 1, the '\n' included
 *   REPLY: ACK <n>: <text>
 *   CLOSED
 *
 * Layering note: "SENT n bytes" (application view) equals the server's
 * "ACK n" -- both count application bytes. Everything below (segment, packet,
 * frame, bits) adds its own headers and is invisible at this level.
 */

#define _WIN32_WINNT 0x0600      /* required by MinGW-w64 so inet_ntop/inet_pton are declared */
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 4096

static int send_all(SOCKET sd, const char *buf, int len);

int main(int argc, char **argv)
{
    WSADATA wsa;
    SOCKET  sd = INVALID_SOCKET;
    struct sockaddr_in addr;
    char    line[MAXLINE + 1];
    char    reply[MAXLINE + 1];
    int     port;
    int     interactive;

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "usage: ex2_echo_client.exe <host> <port> [text]\n");
        return 2;
    }
    port        = atoi(argv[2]);
    interactive = (argc == 3);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "FATAL: port must be 1-65535\n");
        return 2;
    }
    setvbuf(stdout, NULL, _IONBF, 0);

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "FATAL: WSAStartup failed\n");
        return 1;
    }

    /* step 2 of the TCP skeleton: create the socket */
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == INVALID_SOCKET) {
        fprintf(stderr, "FATAL: socket() failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port   = htons((unsigned short)port);
    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) != 1) {
        fprintf(stderr, "FATAL: bad address: %s\n", argv[1]);
        closesocket(sd);
        WSACleanup();
        return 1;
    }

    /* the TCP three-way handshake happens inside connect() */
    if (connect(sd, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        fprintf(stderr, "FATAL: connect() failed: %d\n", WSAGetLastError());
        closesocket(sd);
        WSACleanup();
        return 1;
    }
    printf("CONNECTED %s:%d\n", argv[1], port);

    do {
        int len;

        if (interactive) {
            if (fgets(line, sizeof(line), stdin) == NULL) break;   /* EOF */
        } else {
            strncpy(line, argv[3], MAXLINE);
            line[MAXLINE] = '\0';
            strcat(line, "\n");
        }

        len = (int)strlen(line);
        if (send_all(sd, line, len) < 0) {
            fprintf(stderr, "FATAL: send() failed: %d\n", WSAGetLastError());
            break;
        }
        printf("SENT %d bytes\n", len);

        {
            int total = 0;
            for (;;) {
                char ch;
                int r = recv(sd, &ch, 1, 0);
                if (r <= 0) break;
                if (ch == '\n') break;
                if (ch == '\r') continue;
                if (total < MAXLINE) reply[total++] = ch;
            }
            reply[total] = '\0';
            printf("REPLY: %s\n", reply);
        }
    } while (interactive);

    closesocket(sd);
    WSACleanup();
    printf("CLOSED\n");
    return 0;
}

/* send() may transmit fewer bytes than requested: loop until all are sent. */
static int send_all(SOCKET sd, const char *buf, int len)
{
    int sent = 0;
    while (sent < len) {
        int r = send(sd, buf + sent, len - sent, 0);
        if (r == SOCKET_ERROR) return -1;
        sent += r;
    }
    return sent;
}
