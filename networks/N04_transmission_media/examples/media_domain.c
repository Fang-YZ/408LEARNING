/* media_domain.c -- Lesson N4: shared vs dedicated medium, collision & broadcast domains
 * (408 Computer Networks, Chapter 2: transmission media and physical-layer devices)
 *
 * Build & run (Windows, from this file's folder):
 *     gcc -Wall -Wextra media_domain.c -o media_domain
 *     media_domain.exe                                    (defaults: 8 10 hub 1)
 *     media_domain.exe <devices> <link_bps> <hub|switch> <vlans>
 *
 * Inputs:
 *     1) number of devices attached                     (>= 1)
 *     2) the link's nominal bandwidth in b/s            (e.g. 10000000 for 10 Mb/s)
 *     3) medium type:  hub  = shared medium (all devices contend for one channel)
 *                      switch = dedicated per-port channel
 *     4) number of VLANs (switch only; ignored for a hub)   (>= 1)
 *
 * What it computes (textbook rules):
 *   HUB    : every device shares the SAME bandwidth  -> per_device = B / n
 *            collision domains = 1, broadcast domains = 1
 *   SWITCH : every port has its own bandwidth        -> per_device = B
 *            collision domains = n (one per port)
 *            broadcast domains = number of VLANs (all ports in one VLAN by default)
 *
 * The point: a hub is one big contention party; a switch gives every station its
 * own lane, and VLANs are what actually cut up the broadcast domain.
 *
 * Output is ASCII English on purpose (Windows console + Chinese = mojibake).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int    devices = 8;
    double bw      = 10000000.0;      /* 10 Mb/s */
    char   kind[16];
    int    vlans   = 1;
    int    is_switch;
    int    collision_domains, broadcast_domains;
    double per_device, total_usable;
    int    i;

    strcpy(kind, "hub");
    if (argc == 5) {
        devices = atoi(argv[1]);
        bw      = atof(argv[2]);
        strncpy(kind, argv[3], sizeof(kind) - 1);
        kind[sizeof(kind) - 1] = '\0';
        vlans   = atoi(argv[4]);
    } else if (argc != 1) {
        fprintf(stderr, "usage: media_domain.exe [<devices> <link_bps> <hub|switch> <vlans>]\n");
        return 2;
    }

    if (devices < 1) { fprintf(stderr, "FATAL: devices must be >= 1\n"); return 2; }
    if (bw <= 0)     { fprintf(stderr, "FATAL: link bandwidth must be > 0\n"); return 2; }

    /* normalise the medium keyword (case-insensitive-ish, ASCII) */
    for (i = 0; kind[i]; i++) {
        if (kind[i] >= 'A' && kind[i] <= 'Z') kind[i] = (char)(kind[i] + 32);
    }
    if (strcmp(kind, "switch") == 0)      is_switch = 1;
    else if (strcmp(kind, "hub") == 0)    is_switch = 0;
    else { fprintf(stderr, "FATAL: medium must be 'hub' or 'switch'\n"); return 2; }

    if (!is_switch) {
        vlans = 1;                                  /* a hub has exactly one broadcast domain */
        per_device     = bw / (double)devices;      /* everyone shares one channel */
        collision_domains = 1;
        broadcast_domains = 1;
        total_usable   = bw;
    } else {
        if (vlans < 1) vlans = 1;
        if (vlans > devices) vlans = devices;       /* cannot have more VLANs than ports */
        per_device     = bw;                        /* dedicated per port */
        collision_domains = devices;                /* one per switch port */
        broadcast_domains = vlans;
        total_usable   = bw * (double)devices;      /* aggregate switching capacity */
    }

    printf("devices            = %d\n", devices);
    printf("link_bps           = %.0f bps\n", bw);
    printf("medium             = %s\n", is_switch ? "switch" : "hub");
    printf("vlans              = %d\n", vlans);
    printf("per_device_bps     = %.6f bps\n", per_device);
    printf("collision_domains  = %d\n", collision_domains);
    printf("broadcast_domains  = %d\n", broadcast_domains);
    printf("total_usable_bps   = %.0f bps\n", total_usable);
    printf("sharing_penalty    = %s\n",
           (is_switch || devices == 1) ? "none" : "bandwidth is shared by all devices");
    return 0;
}
