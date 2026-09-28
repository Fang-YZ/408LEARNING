/* sliding_window.c -- Lesson N6: stop-and-wait / GBN / SR window arithmetic
 * (408 Computer Networks, Chapter 3 data link layer: flow control and reliable transfer)
 *
 * Build & run (Windows, from this file's folder):
 *     gcc -Wall -Wextra sliding_window.c -o sliding_window
 *     sliding_window.exe                                  (defaults below)
 *     sliding_window.exe <n_bits> <Td_ms> <RTT_ms> <lost_index>
 *
 * Inputs:
 *     1) n_bits    : number of bits in the frame sequence number (>= 2)
 *     2) Td_ms     : frame transmission time Td, in milliseconds  ( = L / R )
 *     3) RTT_ms    : round-trip time, in milliseconds
 *     4) lost_index: which frame in the current window gets lost, 1-based
 *
 * Model (the textbook results):
 *     a  = RTT / Td                          "how many frames fit in one round trip"
 *     W_need = 1 + 2a                        window needed to keep the link busy
 *     GBN : max window = 2^n - 1             receive window is always 1
 *     SR  : max window = 2^(n-1)             send window == receive window
 *     utilization U = min(1, W_used / (1 + 2a))     (stop-and-wait is W_used = 1)
 *     loss at position k inside the window:
 *         GBN retransmits frames k .. end of window   -> (W - k + 1) frames
 *         SR  retransmits only the lost frame         -> 1 frame
 *
 * Verified known answers:
 *     n=3, Td=1, RTT=4, k=1  -> a=2, W_need=5, GBN 7 / SR 4, U_gbn=1.0, U_sr=0.8,
 *                               GBN resends 7, SR resends 1
 *     n=2, Td=1, RTT=4, k=4  -> W_need=5, GBN 3 / SR 2, U_gbn=0.75, U_sr=0.5,
 *                               GBN resends 0 (k=4 is outside a 3-frame window), SR resends 1
 *
 * Output is ASCII English on purpose (Windows console + Chinese = mojibake).
 */
#include <stdio.h>
#include <stdlib.h>

#define MAXW 65536     /* sequence space is capped at 16 bits for this teaching tool */

int main(int argc, char **argv)
{
    int    n_bits = 3;
    double Td_ms  = 1.0;
    double RTT_ms = 4.0;
    int    lost_index = 1;               /* 1-based position inside the window */

    unsigned long space, gbn_max, sr_max, need;
    double a, denom, u_sw, u_gbn, u_sr;
    unsigned long w_gbn, w_sr, resend_gbn, resend_sr;

    if (argc == 5) {
        n_bits     = atoi(argv[1]);
        Td_ms      = atof(argv[2]);
        RTT_ms     = atof(argv[3]);
        lost_index = atoi(argv[4]);
    } else if (argc != 1) {
        fprintf(stderr, "usage: sliding_window.exe [<n_bits> <Td_ms> <RTT_ms> <lost_index>]\n");
        return 2;
    }

    if (n_bits < 1 || n_bits > 16) { fprintf(stderr, "FATAL: n_bits must be 1..16\n"); return 2; }
    if (Td_ms <= 0 || RTT_ms < 0)  { fprintf(stderr, "FATAL: Td must be > 0 and RTT must be >= 0\n"); return 2; }
    if (lost_index < 1)            { fprintf(stderr, "FATAL: lost_index must be >= 1\n"); return 2; }

    space = 1UL << n_bits;               /* 2^n possible sequence numbers */
    gbn_max = space - 1;                 /* GBN: send window at most 2^n - 1      */
    sr_max  = (n_bits >= 2) ? (1UL << (n_bits - 1)) : 1UL;   /* SR: at most 2^(n-1) */

    a = RTT_ms / Td_ms;                  /* frames that fit in one RTT            */
    /* W_need is the smallest integer W with W * Td >= Td + RTT  <=>  W >= 1 + a */
    need = (unsigned long)1;
    while ((double)need * Td_ms < (Td_ms + RTT_ms)) need++;

    /* ---- utilization ---------------------------------------------------- */
    w_gbn = gbn_max;
    w_sr  = sr_max;

    u_sw  = 1.0 / (1.0 + a);
    denom = 1.0 + a;

    u_gbn = (double)w_gbn / denom;
    if (u_gbn > 1.0) u_gbn = 1.0;

    u_sr  = (double)w_sr / denom;
    if (u_sr > 1.0) u_sr = 1.0;

    /* ---- what happens when ONE frame in the window is lost -------------- */
    if ((unsigned long)lost_index > w_gbn) {
        resend_gbn = 0;                  /* the lost frame is outside the window */
    } else {
        resend_gbn = w_gbn - (unsigned long)lost_index + 1;
    }
    resend_sr = 1;                       /* SR acknowledges out-of-order frames  */

    printf("n_bits             = %d\n", n_bits);
    printf("seq_space_2n       = %lu\n", space);
    printf("Td_ms              = %.6f ms\n", Td_ms);
    printf("RTT_ms             = %.6f ms\n", RTT_ms);
    printf("a_ratio_RTT_over_Td= %.6f\n", a);
    printf("window_needed      = %lu\n", need);
    printf("gbn_max_window     = %lu\n", gbn_max);
    printf("sr_max_window      = %lu\n", sr_max);
    printf("util_stop_and_wait = %.6f\n", u_sw);
    printf("util_gbn           = %.6f\n", u_gbn);
    printf("util_sr            = %.6f\n", u_sr);
    printf("resend_on_loss_gbn = %lu\n", resend_gbn);
    printf("resend_on_loss_sr  = %lu\n", resend_sr);
    printf("note               = ");

    if (gbn_max >= need && sr_max >= need) {
        printf("both GBN and SR can keep the link busy\n");
    } else if (gbn_max < need && sr_max < need) {
        printf("neither window is big enough: the link idles\n");
    } else if (gbn_max >= need) {
        printf("GBN is enough, SR window is too small\n");
    } else {
        printf("SR is enough, GBN window is too small\n");
    }

    return 0;
}
