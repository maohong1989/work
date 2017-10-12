#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
int main(void)
{
	short ch = 0x31;
	wchar_t data;
	char a[2];
// 	char aa[4];
	unsigned char *aa;
	int i, len,ret;
	unsigned int value = 0;

	FILE *Fd = NULL;
	Fd = fopen("gb2unicode.bin", "wb");
	if (Fd == NULL)
		printf("error!\n");

	setlocale(LC_ALL, "zh_CN.gbk");
	for (i = 0; i < 65535; i++)
	{
		ch   = i;
		a[0] = (ch & 0xff);
		a[1] = ((ch & 0xff00) >> 8);
		len  = mbstowcs(&data, a, 4);
// 		printf("len [%d] data[0x%x]\n", len, data);

		if (len == -1)
		{
			value = 0xa;
		}
		else
		{
			value = data;
		}


		ret = fwrite(&value, 1, 2, Fd);
		if (ret != 2)
			printf("error!\n");
	}
	fclose(Fd);
	return 0;
}
