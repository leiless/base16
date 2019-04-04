/*
 * Created 190403 lynnl
 */

#include <assert.h>
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
    fprintf(stderr, "b16_encode('%s', %zu, '%s');\n", src, n, expected);

    static char buff[512];

    *buff = '\1';
    base16_encode_baseline(buff, src, n);
    fprintf(stderr, "%s\n", buff);
    assert(!strcmp(buff, expected));

    *buff = '\2';
    base16_encode(buff, src, n);
    assert(!strcmp(buff, expected));

    *buff = '\3';
    base16_encode2(buff, src, n);
    assert(!strcmp(buff, expected));
}

int main(void)
{
    int i;
    char buff[512];

    //generate_u16_table();
    //generate_u32_table();

    for (i = 0; i <= UCHAR_MAX; i++) {
        base16_encode_baseline(buff, &i, sizeof(uint8_t));
        printf("%s,%c", buff, i % 16 != 15 ? ' ' : '\n');
    }
    puts("");

    for (i = 0; i <= UCHAR_MAX; i++) {
        base16_encode(buff, &i, sizeof(uint8_t));
        printf("%s,%c", buff, i % 16 != 15 ? ' ' : '\n');
    }
    puts("");

    for (i = 0; i <= USHRT_MAX; i++) {
        base16_encode2(buff, &i, sizeof(uint16_t));
        printf("%s,%c", buff, i % 16 != 15 ? ' ' : '\n');
    }
    puts("");

    for (i = 0; i <= UCHAR_MAX; i++) {
        base16_encode2(buff, &i, sizeof(uint8_t));
        printf("%s,%c", buff, i % 16 != 15 ? ' ' : '\n');
    }
    puts("");

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
    b16_encode("0123456789ABCDEF", 10, "30313233343536373839");
    b16_encode("0123456789ABCDEF", 15, "303132333435363738394142434445");

    fprintf(stderr, "\nPASS!\n");

    return 0;
}

