/* switch_compare.c -- Lesson N3: circuit / message / packet switching timing
 * (408 Computer Networks, Chapter 2: three switching paradigms)
 *
 * Build & run (Windows, from this file's folder):
 *     gcc -Wall -Wextra switch_compare.c -o switch_compare
 *     switch_compare.exe                                    (uses the defaults below)
 *     switch_compare.exe <L_bit> <rate_bps> <hops> <pkt_bit>
 *
 * Inputs:
 *     1) total message length L in bit
 *     2) link bandwidth R in bps (each hop has the same R)
 *     3) number of hops k (host -> router -> ... -> host; k = links on the path)
 *     4) packet size P in bit (only used by packet switching)
 *
 * Model (textbook values, ignoring processing / queueing / propagation):
 *     circuit switching : setup S seconds first, then the bits flow through
 *                         => S + L/R                      (no store-and-forward)
 *     message switching : store-and-forward the WHOLE message at every hop
 *                         => k * (L/R)
 *     packet switching  : pipeline the P-bit packets through k hops
 *                         => (k + p - 1) * (P/R),  p = L/P packets
 *
 * The point of this program: watch packet switching beat message switching,
 * and see the tradeoff against circuit switching's fixed setup cost.
 *
 * Output is ASCII English on purpose (Windows console + Chinese = mojibake).
 */
#include <stdio.h>
#include <stdlib.h>

#define CIRCUIT_SETUP_S 0.500    /* fixed call setup time of circuit switching */

int main(int argc, char **argv)
{
    double L = 8000.0;      /* bit   */
    double R = 1000.0;      /* bps   */
    int    k = 3;           /* hops  */
    double P = 1000.0;      /* bit   */
    long   p;
    double circuit, message, packet;

    if (argc == 5) {
        L = atof(argv[1]);
        R = atof(argv[2]);
        k = atoi(argv[3]);
        P = atof(argv[4]);
    } else if (argc != 1) {
        fprintf(stderr, "usage: switch_compare.exe [<L_bit> <rate_bps> <hops> <pkt_bit>]\n");
        return 2;
    }

    if (L <= 0 || R <= 0 || k <= 0 || P <= 0) {
        fprintf(stderr, "FATAL: L, R, hops and packet size must all be > 0\n");
        return 2;
    }

    p       = (long)(L / P);          /* number of packets (whole packets only) */
    if (p < 1) p = 1;
    if (p * P < L) p += 1;            /* last partial packet still must be sent */

    circuit = CIRCUIT_SETUP_S + L / R;
    message = (double)k * (L / R);
    packet  = ((double)k + (double)p - 1.0) * (P / R);

    printf("message_bit        = %.0f bit\n", L);
    printf("bandwidth_bps      = %.0f bps\n", R);
    printf("hops               = %d\n", k);
    printf("packet_bit         = %.0f bit\n", P);
    printf("packet_count       = %ld\n", p);
    printf("one_hop_time_s     = %.6f s\n", L / R);
    printf("circuit_total_s    = %.6f s\n", circuit);
    printf("message_total_s    = %.6f s\n", message);
    printf("packet_total_s     = %.6f s\n", packet);
    printf("packet_vs_message  = %.6f s saved\n", message - packet);
    printf("faster_than_msg    = %s\n", (packet < message) ? "packet" : "message");
    printf("circuit_vs_packet  = %s wins by %.6f s\n",
           (circuit < packet) ? "circuit" : "packet",
           (circuit < packet) ? (packet - circuit) : (circuit - packet));
    return 0;
}
