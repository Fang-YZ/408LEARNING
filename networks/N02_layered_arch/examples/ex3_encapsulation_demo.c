/*
 * ex3_encapsulation_demo.c  --  N2 classroom example: encapsulation, step by step
 *
 * Pure computation (no sockets): it shows how the SAME user data is wrapped
 * with a new header at every layer on the way down, and unwrapped on the way up.
 * This is the appendix of the exam question "describe data encapsulation".
 *
 * Build:  gcc -Wall -Wextra -O2 -o ex3_encapsulation_demo.exe ex3_encapsulation_demo.c
 * Run:    .\ex3_encapsulation_demo.exe
 *         .\ex3_encapsulation_demo.exe "GET /index.html HTTP/1.1"
 *
 * All output is English on purpose (Windows console + Chinese = mojibake).
 * All numbers are 408 textbook values: TCP 20 + IP 20 + frame 14 + FCS 4.
 */

#include <stdio.h>
#include <string.h>

#define DEF_PAYLOAD "GET /index.html HTTP/1.1"

#define TCP_HDR   20      /* TCP header, no options                */
#define IP_HDR    20      /* IPv4 header, no options               */
#define FRAME_HDR 14      /* dst MAC 6 + src MAC 6 + type 2         */
#define FCS_LEN    4      /* frame check sequence (trailer)         */

static void repeat(char c, int n)
{
    int i;
    for (i = 0; i < n; i++) putchar(c);
}

static void banner(const char *title)
{
    printf("\n");
    repeat('=', 62);
    printf("\n  %s\n", title);
    repeat('=', 62);
    printf("\n");
}

/* Print one layer's data unit, drawn on top of the layer below. */
static void show_unit(const char *layer, const char *unit, int payload_len,
                      const char *left, int left_len,
                      const char *right, int right_len)
{
    printf("  %-14s %-22s ", layer, unit);
    if (left_len > 0)  printf("[%s:%d]", left, left_len);
    printf("[DATA:%d]", payload_len);
    if (right_len > 0) printf("[%s:%d]", right, right_len);
    printf("\n");
}

/* The frame row: the only unit that carries both a header and a trailer. */
static void show_frame(int payload_len)
{
    printf("  %-14s %-22s [FRAME:%d][IP:%d][TCP:%d][DATA:%d][FCS:%d]\n",
           "data link", "frame", FRAME_HDR, IP_HDR, TCP_HDR, payload_len, FCS_LEN);
}

int main(int argc, char **argv)
{
    const char *payload = (argc > 1) ? argv[1] : DEF_PAYLOAD;
    int payload_len = (int)strlen(payload);

    int segment = payload_len + TCP_HDR;
    int packet  = segment     + IP_HDR;
    int frame   = packet      + FRAME_HDR + FCS_LEN;
    int bits    = frame * 8;

    banner("ENCAPSULATION DEMO -- one user payload, five layers");

    printf("  payload (application data): \"%s\"\n", payload);
    printf("  payload length            : %d bytes\n", payload_len);

    printf("\n  --- sending side: top -> bottom, a header is ADDED each step ---\n\n");
    show_unit("application", "message", payload_len, "", 0, "", 0);
    show_unit("transport", "segment", payload_len, "TCP", TCP_HDR, "", 0);
    show_unit("network", "packet", payload_len, "IP", IP_HDR, "", 0);
    show_frame(payload_len);
    printf("  %-14s %-22s %d bytes -> %d bits on the wire\n",
           "physical", "bit", frame, bits);

    printf("\n  --- received side: bottom -> top, that header is STRIPPED again ---\n\n");
    printf("  physical    : %d bits arrive\n", bits);
    printf("  data link   : check FCS (%d bytes), strip FRAME header (%d bytes) -> %d bytes\n",
           FCS_LEN, FRAME_HDR, packet);
    printf("  network     : strip IP header (%d bytes), look up the route      -> %d bytes\n",
           IP_HDR, segment);
    printf("  transport   : strip TCP header (%d bytes), demux by port number  -> %d bytes\n",
           TCP_HDR, payload_len);
    printf("  application : gets exactly \"%s\"\n", payload);

    banner("ONLY THE DATA LINK LAYER ADDS A TRAILER");
    printf("  headers added  : TCP %d + IP %d + frame %d = %d bytes of overhead\n",
           TCP_HDR, IP_HDR, FRAME_HDR, TCP_HDR + IP_HDR + FRAME_HDR);
    printf("  trailer added  : FCS %d bytes (frame check sequence)\n", FCS_LEN);
    printf("  total on wire  : %d bytes (%d bits)\n", frame, bits);
    printf("  efficiency     : %d / %d = %.2f%%\n",
           payload_len, frame, 100.0 * payload_len / frame);

    banner("CHECK YOURSELF (compute these by hand first)");
    printf("  1. Why does the frame need a trailer but the segment does not?\n");
    printf("  2. If the payload were 1 byte, how many bytes go on the wire?  ... %d\n",
           1 + TCP_HDR + IP_HDR + FRAME_HDR + FCS_LEN);
    printf("  3. Which device stops at the network layer?  ... a router\n");
    printf("  4. Which addresses change at every hop?      ... MAC addresses\n");

    return 0;
}
