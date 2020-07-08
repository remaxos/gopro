#include <stdio.h>
#include <string.h>
#include <stdint.h>

float dump_mempic(uint8_t *mem)
{
    uint64_t w, h; 
    uint64_t i;
    uint16_t *pixels;

    w = *mem;
    h = *(mem + sizeof(w));
    pixels = (uint16_t *)(mem + sizeof(w) + sizeof(h));

    printf("width=%lu height=%lu\n", w, h);
    for (i = 0; i < w * h; i++) {
	printf("%04X ", *(pixels + i));
    }
    printf("\n");
}

int matches(const char *cmd, const char *pattern)
{
	int len = strlen(cmd);

	if (len > strlen(pattern))
		return -1;
	return memcmp(pattern, cmd, len);
}
