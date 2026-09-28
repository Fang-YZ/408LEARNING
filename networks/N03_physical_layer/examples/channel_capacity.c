/* channel_capacity.c -- Lesson N3: Nyquist and Shannon channel capacity
 * (408 Computer Networks, Chapter 2 physical layer: communication basics)
 *
 * Build & run (Windows, from this file's folder):
 *     gcc -Wall -Wextra channel_capacity.c -o channel_capacity
 *     channel_capacity.exe                     (uses the three defaults below)
 *     channel_capacity.exe <B_Hz> <V> <SNR_dB>
 *
 * Inputs:
 *     1) channel bandwidth B in Hz      (e.g. 3 kHz = 3000)
 *     2) number of signal levels V      (>= 1; V = 2^b where b = bits per symbol)
 *     3) signal-to-noise ratio in dB    (e.g. 20; use -1 for an ideal channel)
 *
 * Formulas (408 textbook):
 *     Nyquist (ideal, no noise):  C = 2 * B * log2(V)      [bit/s]
 *     Shannon (noisy channel):    C = B * log2(1 + S/N)    [bit/s]
 *     SNR(dB) = 10 * log10(S/N)  =>  S/N = 10^(SNR_dB / 10)
 *
 * Output is ASCII English on purpose (Windows console + Chinese = mojibake).
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static double log2_of(double x) { return log(x) / log(2.0); }

int main(int argc, char **argv)
{
    double B    = 3000.0;    /* Hz   */
    double V    = 4.0;       /* levels */
    double snrdb = 20.0;     /* dB; -1 means "ideal channel, no noise" */
    double snr, nyquist, shannon, limit, bps_per_symbol;

    if (argc == 4) {
        B     = atof(argv[1]);
        V     = atof(argv[2]);
        snrdb = atof(argv[3]);
    } else if (argc != 1) {
        fprintf(stderr, "usage: channel_capacity.exe [<B_Hz> <V> <SNR_dB>]\n");
        return 2;
    }

    if (B <= 0 || V < 1) {
        fprintf(stderr, "FATAL: bandwidth must be > 0 and V must be >= 1\n");
        return 2;
    }

    bps_per_symbol = log2_of(V);
    nyquist        = 2.0 * B * bps_per_symbol;

    printf("bandwidth_Hz       = %.0f Hz\n", B);
    printf("signal_levels_V    = %.0f\n", V);
    printf("bits_per_symbol    = %.6f bit\n", bps_per_symbol);
    printf("nyquist_capacity   = %.0f bps\n", nyquist);

    if (snrdb < 0.0) {
        printf("snr_dB             = ideal (no noise)\n");
        printf("shannon_capacity   = (not applicable: ideal channel)\n");
        limit = nyquist;
    } else {
        snr     = pow(10.0, snrdb / 10.0);
        shannon = B * log2_of(1.0 + snr);
        printf("snr_dB             = %.1f dB\n", snrdb);
        printf("snr_linear         = %.6f\n", snr);
        printf("shannon_capacity   = %.6f bps\n", shannon);
        limit = (nyquist < shannon) ? nyquist : shannon;   /* the real ceiling */
    }

    printf("max_data_rate      = %.6f bps\n", limit);
    printf("limit_source       = %s\n",
           (snrdb < 0.0) ? "nyquist" : ((nyquist < shannon) ? "nyquist" : "shannon"));
    return 0;
}
