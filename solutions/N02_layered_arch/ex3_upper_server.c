/*
 * ex3_upper_server.c  --  N2 homework REFERENCE SOLUTION (challenge task)
 *
 * Same as networks/N02_layered_arch/examples/ex1_echo_server.c, but the echoed
 * text is uppercased:   ACK <n>: <UPPERCASE_TEXT>     (n unchanged)
 *
 * Build:  gcc -Wall -Wextra -O2 -o ex3_upper_server.exe ex3_upper_server.c -lws2_32
 * Run:    .\ex3_upper_server.exe 9001
 *
 * Verified expectations:
 *   in "tcp"       -> ACK 4: TCP
 *   in "AbC 123!"  -> ACK 9: ABC 123!
 *   in "hello 408" -> ACK 10: HELLO 408
 *
 * Key point: the response prefix is "ACK <n>: " -- changing the payload's
 * case must NOT change n (ASCII case conversion preserves length). n counts
 * the line's bytes INCLUDING the terminating '\n', so "tcp" -> 4, not 3.
 */

#define _WIN32_WINNT 0x0600      /* required by MinGW-w64 so inet_ntop/inet_pton are declared */
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 4096

static int recv_line(SOCKET sd, char *out, int cap);
static int send_all(SOCKET sd, const char *buf, int len);

int main(int argc, char **argv)
{
    WSADATA wsa;
    SOCKET  listen_sock = INVALID_SOCKET, conn = INVALID_SOCKET;
    struct sockaddr_in addr;
    int     port;
    char    buf[MAXLINE + 1];
    char    reply[MAXLINE + 64];
    int     n, i;

    if (argc != 2) {
        fprintf(stderr, "usage: server_echo_upper.exe <port>\n");
        return 2;
    }
    port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "FATAL: port must be 1-65535\n");
        return 2;
    }
    setvbuf(stdout, NULL, _IONBF, 0);

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "FATAL: WSAStartup failed\n");
        return 1;
    }

    listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_sock == INVALID_SOCKET) {
        fprintf(stderr, "FATAL: socket() failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    {
        BOOL yes = TRUE;
        setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons((unsigned short)port);

    if (bind(listen_sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        fprintf(stderr, "FATAL: bind() failed on port %d: %d\n", port, WSAGetLastError());
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }
    if (listen(listen_sock, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(stderr, "FATAL: listen() failed: %d\n", WSAGetLastError());
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    printf("UPPER ECHO SERVER READY on port %d\n", port);

    for (;;) {
        struct sockaddr_in peer;
        int plen = (int)sizeof(peer);
        char peerip[INET_ADDRSTRLEN] = {0};

        conn = accept(listen_sock, (struct sockaddr *)&peer, &plen);
        if (conn == INVALID_SOCKET) {
            fprintf(stderr, "WARN: accept() failed: %d\n", WSAGetLastError());
            continue;
        }
        inet_ntop(AF_INET, &peer.sin_addr, peerip, sizeof(peerip));
        printf("CONNECTED from %s:%d\n", peerip, ntohs(peer.sin_port));

        for (;;) {
            n = recv_line(conn, buf, MAXLINE);
            if (n < 0) break;
            n += 1;                                  /* count the '\n' delimiter too */

            /* --- the ONLY real change vs server_echo.c: uppercase the payload --- */
            for (i = 0; i < n; i++) {
                buf[i] = (char)toupper((unsigned char)buf[i]);
            }
            buf[n] = '\0';

            _snprintf(reply, sizeof(reply), "ACK %d: %s\n", n, buf);
            reply[sizeof(reply) - 1] = '\0';
            if (send_all(conn, reply, (int)strlen(reply)) < 0) break;
            printf("RECV %d bytes -> %s\n", n, buf);
        }

        closesocket(conn);
        conn = INVALID_SOCKET;
        printf("DISCONNECTED\n");
    }

    closesocket(listen_sock);
    WSACleanup();
    return 0;
}

static int recv_line(SOCKET sd, char *out, int cap)
{
    int  total = 0;
    char ch;

    for (;;) {
        int r = recv(sd, &ch, 1, 0);
        if (r == 0) return (total == 0) ? -1 : total;
        if (r == SOCKET_ERROR) return -1;
        if (ch == '\n') break;
        if (ch == '\r') continue;
        if (total < cap) out[total++] = ch;
    }
    out[total] = '\0';
    return total;
}

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
