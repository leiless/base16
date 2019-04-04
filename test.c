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
        //printf("%#x,%c", t, i % M != N ? ' ' : '\n');

        /* The other endian representation from machine endian */
        //t = (t << 16) | (t >> 16);
        //printf("%#x,%c", htonl(t), i % M != N ? ' ' : '\n');
    }

    if (USHRT_MAX % M != N) putchar('\n');

    exit(EXIT_SUCCESS);
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

    base16_encode2(buff, NULL, 0);
    assert(!strcmp(buff, ""));

    base16_encode2(buff, "", 0);
    assert(!strcmp(buff, ""));

    base16_encode2(buff, "f", 1);
    assert(!strcmp(buff, "66"));

    base16_encode2(buff, "fo", 2);
    assert(!strcmp(buff, "666F"));

    base16_encode2(buff, "foo", 3);
    assert(!strcmp(buff, "666F6F"));

    base16_encode2(buff, "foob", 4);
    assert(!strcmp(buff, "666F6F62"));

    base16_encode2(buff, "fooba", 5);
    assert(!strcmp(buff, "666F6F6261"));

    base16_encode2(buff, "foobar", 6);
    assert(!strcmp(buff, "666F6F626172"));

    base16_encode2(buff, "foobar!", 7);
    assert(!strcmp(buff, "666F6F62617221"));

    base16_encode(buff, "foobar!", 7);
    assert(!strcmp(buff, "666F6F62617221"));

    base16_encode_baseline(buff, "foobar!", 7);
    assert(!strcmp(buff, "666F6F62617221"));

    fprintf(stderr, "PASS!\n");

    return 0;
}

