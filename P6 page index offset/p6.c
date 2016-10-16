//Michael Mickelson
// CS3060 Program 6

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define HEX (16)
#define PAGE_SIZE (4096)

int main(int argc, char *argv[])
{
	int temp, pageIndex = 0, offset = 0;
	temp = strtol(argv[1], NULL, HEX);

	pageIndex = temp / PAGE_SIZE;
	offset = temp % PAGE_SIZE;
	
	printf("Logical Addr:0x%08x - Page Index:0x%08x - Offset:0x%08x\n", temp, pageIndex, offset);
}
