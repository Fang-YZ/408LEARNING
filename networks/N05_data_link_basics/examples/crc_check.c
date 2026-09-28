/* crc_check.c -- Lesson N5: Cyclic Redundancy Check (CRC) by hand, in code
 * (408 Computer Networks, Chapter 3 data link layer: error detection)
 *
 * Build & run (Windows, from this file's folder):
 *     gcc -Wall -Wextra crc_check.c -o crc_check
 *     crc_check.exe                                  (defaults: 1101011011 10011)
 *     crc_check.exe <data_bits> <generator_bits>
 *
 * Inputs (strings of '0'/'1' only):
 *     1) data bits M          e.g. 1101011011
 *     2) generator G (>= 2)   e.g. 10011        (a.k.a. the generator polynomial)
 *
 * What it does (the exam procedure, step by step):
 *     1. let r = deg(G) = strlen(G) - 1
 *     2. append r zeros to M  ->  that is M * 2^r
 *     3. divide M*2^r by G using XOR (mod-2 division), keep the r-bit remainder R
 *     4. the transmitted code word is M followed by R
 *     5. at the receiver, dividing the whole code word by G must give remainder 0
 *
 * Verified known answers (r = 4):
 *     "1101011011" / "10011"  ->  remainder 1110,  code word 11010110111110,  length 14
 *     "101001"     / "1101"   ->  remainder 001,   code word 101001001,       length 9
 *
 * Output is ASCII English on purpose (Windows console + Chinese = mojibake).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBITS 256

/* strip everything that is not '0'/'1', then make sure the string is not empty */
static int clean_bits(const char *in, char *out, int cap)
{
    int n = 0;
    while (*in && n < cap - 1) {
        if (*in == '0' || *in == '1') out[n++] = *in;
        in++;
    }
    out[n] = '\0';
    return n;
}

/* parse a bit string into an integer (data length is bounded by the caller) */
static unsigned long long bits_to_int(const char *bits)
{
    unsigned long long v = 0;
    while (*bits) {
        v = (v << 1) | (unsigned long long)(*bits - '0');
        bits++;
    }
    return v;
}

static void print_bits(const char *label, unsigned long long value, int width)
{
    int i;
    printf("%s", label);
    for (i = width - 1; i >= 0; i--) putchar(((value >> i) & 1ULL) ? '1' : '0');
    putchar('\n');
}

int main(int argc, char **argv)
{
    char mraw[MAXBITS], graw[MAXBITS];
    char data[MAXBITS];
    const char *msrc, *gsrc;
    int mlen, glen, r, i;
    unsigned long long M, G, R, codeword, check, data_int;

    if (argc == 3) {
        msrc = argv[1];
        gsrc = argv[2];
    } else if (argc == 1) {
        msrc = "1101011011";
        gsrc = "10011";
    } else {
        fprintf(stderr, "usage: crc_check.exe [<data_bits> <generator_bits>]\n");
        return 2;
    }

    mlen = clean_bits(msrc, mraw, MAXBITS);
    glen = clean_bits(gsrc, graw, MAXBITS);

    if (mlen == 0 || glen < 2) {
        fprintf(stderr, "FATAL: need data bits >= 1 and generator bits >= 2 (0/1 only)\n");
        return 2;
    }
    if (mlen > 32 || glen > 32) {
        fprintf(stderr, "FATAL: keep both bit strings within 32 bits for this teaching tool\n");
        return 2;
    }

    mlen = (int)strlen(mraw);
    glen = (int)strlen(graw);
    r    = glen - 1;

    strcpy(data, mraw);
    for (i = 0; i < r; i++) data[mlen + i] = '0';
    data[mlen + r] = '\0';

    M         = bits_to_int(mraw);
    G         = bits_to_int(graw);
    data_int  = bits_to_int(data);

    /* ---- mod-2 division: shift G to align with the leading 1 of R, then XOR ---- */
    R = data_int;
    {
        int k;
        for (k = mlen + r; k >= r; k--) {
            if ((R >> k) & 1ULL) {
                R ^= (G << (k - r));
            }
        }
    }

    codeword = (M << r) | R;
    check    = codeword;
    for (i = mlen + r; i >= r; i--) {
        if ((check >> i) & 1ULL) {
            check ^= (G << (i - r));
        }
    }

    printf("data_bits          = %s\n", mraw);
    printf("data_length        = %d bit\n", mlen);
    printf("generator_bits     = %s\n", graw);
    printf("generator_degree_r = %d\n", r);
    printf("appended_zeros     = ");
    for (i = 0; i < r; i++) putchar('0');
    putchar('\n');
    printf("divided_bits       = %s\n", data);
    print_bits("remainder          = ", R, r);
    print_bits("remainder_bin      = ", R, r);
    /* NOTE: MinGW's msvcrt printf cannot do "%llX" -- cast instead of changing the specifier,
       otherwise -Wall -Wextra reports "unknown conversion type character 'l'".       */
    printf("remainder_hex      = 0x%X\n", (unsigned int)R);
    print_bits("codeword           = ", codeword, mlen + r);
    printf("codeword_length    = %d bit\n", mlen + r);
    printf("receiver_remainder = ");
    for (i = r - 1; i >= 0; i--) putchar(((check >> i) & 1ULL) ? '1' : '0');
    putchar('\n');
    printf("verdict            = %s\n", (check == 0ULL) ? "OK (remainder is zero)" : "ERROR DETECTED");
    return 0;
}
