#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef C_ASSERT
// Straight copy from Win32 headers, but not including Windows.h makes this portable
#define C_ASSERT(e) typedef char __C_ASSERT__[(e) ? 1 : -1]
#endif

typedef unsigned char byte;
#define BYTES_PER_ROW 16
C_ASSERT(!(BYTES_PER_ROW & 1));
#define REPRESENT_BYTE(b) (((b) >= (0x20)) && ((b) <= (0x7E))) ? ((char)(b)) : ('.')

int main(int argc, char *argv[])
{
	FILE *fp;
	errno_t err;
	size_t filesize, i, i2, i3, paddedsize;
	byte *buffer = NULL, print[BYTES_PER_ROW];
	
	
	if(argc < 2)
	{
		puts("Usage: bintype filename");
		return 0;
	}
	
	fp = fopen(argv[1], "rb");	
	if(!fp)
	{
		printf("Could not open %s\n", argv[1]);
		return ENOENT;
	}
	
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	rewind(fp);
	if(0 == filesize)
	{
		puts("File is empty");
		return 0;
	}
	
	paddedsize = filesize;
	while((paddedsize++) % BYTES_PER_ROW);

	buffer = malloc(paddedsize + 1);
	if(NULL == buffer)
	{
		perror("malloc");
		return ENOMEM;
	}
	
	memset(buffer, 0, paddedsize);
	fread(buffer, 1, filesize, fp);
	fclose(fp);
	
	for(i = 0; i <= paddedsize; i++)
	{
		if((i % BYTES_PER_ROW) == 0)
		{
			if(i > 0)
			{
				putchar(' ');
				for(i3 = 0; i3 < BYTES_PER_ROW; i3++)
				{
					if ((i + i3 - BYTES_PER_ROW) >= filesize)
					{
						putchar(' ');
					}
					else
					{
						putchar(REPRESENT_BYTE(print[i3]));
					}
					
				}
				if (i == paddedsize - 1)
				{
					break;
				}
				memset(print, 0, BYTES_PER_ROW);
				putchar('\n');
			}
			i2 = 0;
			
			printf("%.8X: ", i);

		}
		if(i > 0 && (i % (BYTES_PER_ROW / 2)) == 0 && (i % BYTES_PER_ROW) != 0)
		{
			putchar(' ');
		}
		if (i >= filesize)
		{
			printf("   ");
		}
		else
		{
			printf("%.2hhX ", buffer[i]);
		}
		print[i2++] = buffer[i];
	}
	
	free(buffer);
	buffer = NULL;

	putchar('\n');
	
	return 0;
}
