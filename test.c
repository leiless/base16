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

#include "src/base16.h"

#define P       8
#define Q       (P - 1)

void generate_u16_table(void)
{
    int i;
    uint16_t t;
    char buff[3];

    for (i = 0; i <= UCHAR_MAX; i++) {
        base16_encode_baseline(buff, &i, sizeof(uint8_t));
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
        base16_encode(buff, &i, sizeof(uint16_t));
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

static void b16_encode(const void *src, size_t n, const char *expected)
{
    printf("b16_encode('%s', %zu, '%s');\n", src, n, expected);

    static char buff[512];

    *buff = '\1';
    base16_encode_baseline(buff, src, n);
    printf("%s\n", buff);
    assert(!strcmp(buff, expected));

    *buff = '\2';
    base16_encode(buff, src, n);
    assert(!strcmp(buff, expected));

    *buff = '\3';
    base16_encode2(buff, src, n);
    assert(!strcmp(buff, expected));
}

static void enc_bench(const void *src, size_t n)
{
    assert(n <= SSIZE_MAX);
    char *dst = malloc((n << 1) + 1);
    assert(dst != NULL);
    clock_t t;

    t = clock();
    base16_encode_baseline(dst, src, n);
    t = clock() - t;

    printf("\nBenchmarking  size: %zu\n", n);

    printf("Time elapsed: %gs  base16_encode_baseline()\n",
            (double) (t) / CLOCKS_PER_SEC);

    t = clock();
    base16_encode(dst, src, n);
    t = clock() - t;

    printf("Time elapsed: %gs  base16_encode()\n",
            (double) (t) / CLOCKS_PER_SEC);

    t = clock();
    base16_encode2(dst, src, n);
    t = clock() - t;

    printf("Time elapsed: %gs  base16_encode2()\n",
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

    b16_encode("", 0, "");
    b16_encode(NULL, 0, "");
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

