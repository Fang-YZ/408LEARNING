/* net_delay.c — Lesson N1: transmission / propagation delay calculator
 * (408 Computer Networks, Chapter 1 performance metrics)
 *
 * Build & run (Windows, from this file's folder):
 *     gcc net_delay.c -o net_delay
 *     net_delay.exe
 *
 * Inputs (decimal numbers, convert units BEFORE typing):
 *     1) file size in bits      (e.g. 10^7 bit = 10000000)
 *     2) bandwidth in bps       (e.g. 100 Mb/s = 100000000)
 *     3) link length in meters  (e.g. 1000 km = 1000000)
 *     4) propagation speed in m/s (copper cable ~= 200000000)
 *
 * Outputs are ASCII English on purpose (no Chinese in the terminal).
 */
#include <stdio.h>

int main(void)
{
    double bits, rate, dist, speed;

    printf("Enter file size (bit): ");
    if (scanf("%lf", &bits) != 1) { printf("bad input\n"); return 1; }
    printf("Enter bandwidth (bps): ");
    if (scanf("%lf", &rate) != 1) { printf("bad input\n"); return 1; }
    printf("Enter link length (m): ");
    if (scanf("%lf", &dist) != 1) { printf("bad input\n"); return 1; }
    printf("Enter propagation speed (m/s): ");
    if (scanf("%lf", &speed) != 1) { printf("bad input\n"); return 1; }

    double send  = bits / rate;      /* transmission delay = L / R     */
    double prop  = dist / speed;     /* propagation delay  = d / v     */
    double total = send + prop;      /* processing/queueing ignored    */
    double prod  = prop * rate;      /* delay-bandwidth product (bits) */
    double rttp  = 2.0 * prop;       /* RTT, propagation part only     */

    printf("file_size          = %.0f bit\n", bits);
    printf("bandwidth          = %.0f bps\n", rate);
    printf("send_delay         = %.9f s\n", send);
    printf("propagation_delay  = %.9f s\n", prop);
    printf("total_delay        = %.9f s\n", total);
    printf("delay_bandwidth_product = %.6f bit\n", prod);
    printf("rtt_propagation    = %.9f s\n", rttp);
    return 0;
}
