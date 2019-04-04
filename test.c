/*
 * Created 190403 lynnl
 */

#include <stdio.h>
#include <limits.h>
#include <arpa/inet.h>

#include "src/base16.h"

#define M       6
#define N       (M - 1)

void generate_u32_table(void)
{
    int i;
    uint32_t t;
    char buff[5];

    for (i = 0; i <= USHRT_MAX; i++) {
        base16_encode(buff, &i, sizeof(uint16_t));
        //printf("%s,%c", buff, i % M != N ? ' ' : '\n');
        /* Machine endianness */
        //printf("%#x,%c", *((uint32_t *) buff), i % M != N ? ' ' : '\n');

        /* Big endian(human intuitive) representation from big endian */
        //printf("%#x,%c", htonl(*((uint32_t *) buff)), i % M != N ? ' ' : '\n');

        /* Big endian(human intuitive) representation from little endian */
        t = *((uint32_t *) buff);
        t = (t << 16) | (t >> 16);
        printf("%#x,%c", htonl(t), i % M != N ? ' ' : '\n');
    }

    if (USHRT_MAX % M != N) putchar('\n');
}

int main(void)
{
    int i;
    char buff[3];
    char buff2[5];

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
        base16_encode2(buff2, &i, sizeof(uint16_t));
        printf("%s,%c", buff2, i % 16 != 15 ? ' ' : '\n');
    }
    puts("");

    for (i = 0; i <= UCHAR_MAX; i++) {
        base16_encode2(buff, &i, sizeof(uint8_t));
        printf("%s,%c", buff, i % 16 != 15 ? ' ' : '\n');
    }
    puts("");

    //generate_u32_table();

    return 0;
}

