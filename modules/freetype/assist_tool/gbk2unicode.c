////////////////////////////////////////////////////////////////////////////////////
//                        版权所有，作者保留一切权利
// 在保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播。
// 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途
////////////////////////////////////////////////////////////////////////////////////
// 文 件 名: gbk2unicode.c
// 作    者: apostle --- apostle9891@foxmail.com
// 版    本: version 1.0
// 日    期: 2014-03-17 15:56
// 描    述: 由于要gb2312需要转换为unicode。所以遍历gb2312制作映射表。
//           由于中文在unicode里只是转换为两位。所以为了节省空间，所有unicode为两位。
// 历史描述:
////////////////////////////////////////////////////////////////////////////////////
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
