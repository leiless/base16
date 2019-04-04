/*
 * Created 190403 lynnl
 */

#include <stdio.h>
#include <limits.h>
#include <arpa/inet.h>

#include "src/base16.h"

void generate_u32_table(void)
{
    int i;
    char buff[5];

    for (i = 0; i <= USHRT_MAX; i++) {
        base16_encode(buff, &i, sizeof(uint16_t));
        //printf("%s,%c", buff, i % 10 != 9 ? ' ' : '\n');
        /* Endianness dependent */
        //printf("%#x,%c", *((uint32_t *) buff), i % 10 != 9 ? ' ' : '\n');
        /* Big endian(human intuitive) representation */
        printf("%#x,%c", htonl(*((uint32_t *) buff)), i % 10 != 9 ? ' ' : '\n');
    }

    if (USHRT_MAX % 10 != 9) putchar('\n');
}

int main(void)
{
    int i;
    char buff[3];

    for (i = 0; i <= UCHAR_MAX; i++) {
        base16_encode(buff, &i, sizeof(uint8_t));
        printf("%s,%c", buff, i % 16 != 15 ? ' ' : '\n');
    }
    puts("");

    for (i = 0; i <= UCHAR_MAX; i++) {
        base16_encode_baseline(buff, &i, sizeof(uint8_t));
        printf("%s,%c", buff, i % 16 != 15 ? ' ' : '\n');
    }
    puts("");

    //generate_u32_table();

    return 0;
}

