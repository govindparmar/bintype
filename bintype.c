#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef unsigned char byte;

#define PRINTABLEBYTE(b) (((b) >= (0x20)) && ((b) <= (0x7E))) ? ((char)(b)) : ('.')

int __cdecl main(int argc, char *argv[])
{
	FILE *fp;
	errno_t err;
	size_t filesize, i, i2, i3, paddedsize;
	byte *buffer = NULL, print[16];
	
	if(argc<2)
	{
		puts("Usage: bintype filename");
		return 0;
	}
	
	fp = fopen(argv[1], "rb");	
	
	if(!fp)
	{
		printf("Could not open %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	rewind(fp);
	
	paddedsize = filesize;

	while((paddedsize++)%16 != 0);
	

	buffer = malloc(paddedsize + 1);
	if(buffer == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(buffer, 0, paddedsize);
	fread(buffer, 1, filesize, fp);
	fclose(fp);
	
	for(i = 0; i <= paddedsize; i++)
	{
		if((i%16) == 0)
		{
			if(i > 0)
			{
				putchar(' ');
				for(i3 = 0; i3 < 16; i3++)
				{
					if ((i + i3 - 16) >= filesize)
					{
						putchar(' ');
					}
					else
					{
						putchar(PRINTABLEBYTE(print[i3]));
					}
					
				}
				if (i == paddedsize - 1)
				{
					break;
				}
				memset(print, 0, 16);
				putchar('\n');
			}
			i2 = 0;
			
			printf("%.8X: ", i);

		}
		if((i%8) == 0 && i > 0 && (i%16)!=0)
		{
			putchar(' ');
		}
		if (i >= filesize)
		{
			printf("   ");
		}
		else
		{
			printf("%.2X ", buffer[i]);
		}
		print[i2++] = buffer[i];
	}
	
	free(buffer);
	buffer = NULL;
	putchar('\n');
	return 0;
}
