/*
 * Created 190403 lynnl
 */

#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <arpa/inet.h>
#include <ctype.h>

#define NANO_BASE16_MARK_EOS

#include "nano_base16.h"
#include "nano_base16q.h"

#define P       8
#define Q       (P - 1)

void generate_u16_table(void)
{
    int i;
    uint16_t t;
    char buff[3];

    for (i = 0; i <= UCHAR_MAX; i++) {
        nano_base16_encode_baseline(buff, &i, sizeof(uint8_t));
        t = *((uint16_t *) buff);

        /* Result string rep. */
        //printf("%s,%c", buff, i % P != Q ? ' ' : '\n');

        /* Machine endianness */
        printf("%#x,%c", t, i % P != Q ? ' ' : '\n');

        /* The other endian representation from machine endian */
        //printf("%#x,%c", htons(t), i % P != Q ? ' ' : '\n');
    }

    if (UCHAR_MAX % P != Q) putchar('\n');

    exit(EXIT_SUCCESS);
}

#define M       6
#define N       (M - 1)

void generate_u32_table(void)
{
    int i;
    uint32_t t;
    char buff[5];

    for (i = 0; i <= USHRT_MAX; i++) {
        nano_base16_encode(buff, &i, sizeof(uint16_t));
        t = *((uint32_t *) buff);

        /* Result string rep. */
        //printf("%s,%c", buff, i % M != N ? ' ' : '\n');

        /* Machine endianness */
        printf("%#x,%c", t, i % M != N ? ' ' : '\n');

        /* The other endian representation from machine endian */
        //t = (t << 16) | (t >> 16);
        //printf("%#x,%c", htonl(t), i % M != N ? ' ' : '\n');
    }

    if (USHRT_MAX % M != N) putchar('\n');

    exit(EXIT_SUCCESS);
}

void generate_dec_table(void)
{
    int i;
    int t;

    for (i = 0; i <= UCHAR_MAX; i++) {
        if (isalpha(i)) {
            t = 10 + i - (islower(i) ? 'a' : 'A');
            if (t > 15) t = 0;
        } else if (isdigit(i)) {
            t = i - '0';
        } else {
            t = 0;
        }

        printf("%2d,%c", t, i % 16 != 15 ? ' ' : '\n');
    }

    exit(EXIT_SUCCESS);
}

void generate_dec_u16_table(void)
{
    int i;
    int j;
    char enc[2];
    unsigned char output;
    ssize_t n;

    for (i = 0; i <= UCHAR_MAX; i++) {
        for (j = 0; j <= UCHAR_MAX; j++) {
            /* Little endian */
            //enc[0] = j;
            //enc[1] = i;

            /* Big endian */
            enc[0] = i;
            enc[1] = j;

            n = nano_base16_decode_baseline(&output, enc, 2);
            assert(n == 1);

            printf("%#4x,%c",
                    output,
                    (i * (UCHAR_MAX + 1) + j) % 13 != 12 ? ' ' : '\n');
        }
    }

    putchar('\n');
    exit(EXIT_SUCCESS);
}

static void b16_encode(const void *src, size_t n, const char *expected)
{
    printf("b16_encode('%s', %zu, '%s');\n", (const char *) src, n, expected);

    static char buff[512];

    *buff = '\1';
    nano_base16_encode_baseline(buff, src, n);
    printf("%s\n", buff);
    assert(!strncmp(buff, expected, n<<1));

    *buff = '\2';
    nano_base16_encode(buff, src, n);
    assert(!strncmp(buff, expected, n<<1));

    *buff = '\3';
    nano_base16_encode2(buff, src, n);
    assert(!strncmp(buff, expected, n<<1));
}

static void b16_decode(const char *src, size_t n, const void *expected)
{
    assert((n & 1) == 0);
    printf("b16_decode('%s', %zu, '%.*s');\n", src, n, (int) n>>1, (char *) expected);

    static char buff[512];
    size_t count;

    *buff = '\1';
    count = nano_base16_decode_baseline(buff, src, n);
    assert((count << 1) == n);
    printf("%.*s\n", (int) count, buff);
    assert(!memcmp(buff, expected, count));

    *buff = '\2';
    count = nano_base16_decode(buff, src, n);
    assert((count << 1) == n);
    printf("%.*s\n", (int) count, buff);
    assert(!memcmp(buff, expected, count));
}

#ifndef SSIZE_MAX
#define SSIZE_MAX   LONG_MAX
#endif

static void enc_bench(const void *src, size_t n)
{
    assert(n <= SSIZE_MAX);
    char *dst = malloc((n << 1) + 1);
    assert(dst != NULL);
    clock_t t;

    t = clock();
    nano_base16_encode_baseline(dst, src, n);
    t = clock() - t;

    printf("\nBenchmarking  size: %zu\n", n);

    printf("Time elapsed: %gs\tnano_base16_encode_baseline()\n",
            (double) (t) / CLOCKS_PER_SEC);

    t = clock();
    nano_base16_encode(dst, src, n);
    t = clock() - t;

    printf("Time elapsed: %gs\tnano_base16_encode()\n",
            (double) (t) / CLOCKS_PER_SEC);

    t = clock();
    nano_base16_encode2(dst, src, n);
    t = clock() - t;

    printf("Time elapsed: %gs\tnano_base16_encode2()\n",
            (double) (t) / CLOCKS_PER_SEC);

    free(dst);
}

static const char *base16_table = "0123456789ABCDEF";

int main(void)
{
    int i;
    uint16_t t;
    char input[3];
    char expected[5];

    //generate_u16_table();
    //generate_u32_table();
    //generate_dec_table();
    //generate_dec_u16_table();

    b16_encode("", 0, "");
    b16_encode(NULL, 0, "");
    b16_encode("", 0, NULL);
    b16_encode("", 1, "00");
    b16_encode("f", 1, "66");
    b16_encode("fo", 2, "666F");
    b16_encode("foo", 3, "666F6F");
    b16_encode("foob", 4, "666F6F62");
    b16_encode("fooba", 5, "666F6F6261");
    b16_encode("foobar", 6, "666F6F626172");
    b16_encode("foobar!", 7, "666F6F62617221");
    b16_encode("foobar!!!", 9, "666F6F626172212121");
    b16_encode("0123456789", 10, "30313233343536373839");
    b16_encode("0123456789#", 10, "30313233343536373839");
    b16_encode("0123456789ABCDEF", 10, "30313233343536373839");
    b16_encode("0123456789ABCDEF", 15, "303132333435363738394142434445");

    b16_decode("", 0, "");
    b16_decode(NULL, 0, "");
    b16_decode("", 0, NULL);
    b16_decode("00", 2, "");
    b16_decode("66", 2, "f");
    b16_decode("666f", 4, "fo");
    b16_decode("666f6F", 6, "foo");
    b16_decode("666F6f", 6, "foo");
    b16_decode("666F6f62", 8, "foob");
    b16_decode("666f6f6261", 10, "fooba");
    b16_decode("666F6F626172", 12, "foobar");
    b16_decode("666f6F62617221", 12, "foobar!");
    b16_decode("666F6f6261722139", 12, "foobar!#");
    b16_decode("30313233343536373839", 20, "0123456789ABCDEF");
    b16_decode("303132333435363738394142434445", 30, "0123456789ABCDEF");

    for (i = 0; i <= UCHAR_MAX; i++) {
        input[0] = (uint8_t) i;
        input[1] = '\0';

        expected[0] = base16_table[i >> 4];
        expected[1] = base16_table[i & 0xf];
        expected[2] = '\0';

        b16_encode(input, 1, expected);
    }

    for (i = 0; i <= USHRT_MAX; i++) {
        t = (uint16_t) i;
        *((uint16_t *) input) = t;
        input[2] = '\0';

        expected[0] = base16_table[(t & 0xff) >> 4];
        expected[1] = base16_table[(t & 0xff) & 0xf];
        expected[2] = base16_table[(t >> 8) >> 4];
        expected[3] = base16_table[(t >> 8) & 0xf];
        expected[4] = '\0';

        b16_encode(input, 2, expected);
    }

    //exit(EXIT_SUCCESS);

    size_t n;
    char *src;

    n = 0;
    src = NULL;
    enc_bench(src, n);
    free(src);

    n = 1;
    src = (char *) malloc(n);
    assert(src != NULL);
    enc_bench(src, n);
    free(src);

    n = 512;
    src = (char *) malloc(n);
    assert(src != NULL);
    enc_bench(src, n);
    free(src);

    n = 4096;   /* 4k */
    src = (char *) malloc(n);
    assert(src != NULL);
    enc_bench(src, n);
    free(src);

    n = 4096;   /* 8k */
    src = (char *) malloc(n);
    assert(src != NULL);
    enc_bench(src, n);
    free(src);

    n = 16384;  /* 16k */
    src = (char *) malloc(n);
    assert(src != NULL);
    enc_bench(src, n);
    free(src);

    n = 1UL << 20;  /* 1m */
    src = (char *) malloc(n);
    assert(src != NULL);
    enc_bench(src, n);
    free(src);

    n = 1UL << 22;  /* 4m */
    src = (char *) malloc(n);
    assert(src != NULL);
    enc_bench(src, n);
    free(src);

    n = 1UL << 24;  /* 16m */
    src = (char *) malloc(n);
    assert(src != NULL);
    enc_bench(src, n);
    free(src);

    n = 1UL << 27;  /* 128m */
    src = (char *) malloc(n);
    assert(src != NULL);
    enc_bench(src, n);
    free(src);

    n = 1UL << 30;  /* 1024m */
    src = (char *) malloc(n);
    assert(src != NULL);
    enc_bench(src, n);
    free(src);

    n = (1UL << 30) + (1UL << 31);  /* 3072m */
    src = (char *) malloc(n);
    assert(src != NULL);
    enc_bench(src, n);
    free(src);

    printf("\nPASS!\n");

    return 0;
}

