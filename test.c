#include <stdio.h>
#include <limits.h>

#include "src/base16.h"

int main(void)
{
    int i;
    char buff[3];

    for (i = 0; i <= UCHAR_MAX; i++) {
        base16_encode(buff, &i, sizeof(uint8_t));
        printf("%s,%c", buff, i % 8 != 7 ? ' ' : '\n');
    }

    return 0;
}

