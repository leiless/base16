#include <stdio.h>
#include <limits.h>

#include "src/base16.h"

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

    return 0;
}

