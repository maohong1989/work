////////////////////////////////////////////////////////////////////////////////////
//                        版权所有，作者保留一切权利
// 但保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播。
// 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途
////////////////////////////////////////////////////////////////////////////////////
// 文 件 名: HashMemMng.c
// 作    者: apostle --- apostle9891@foxmail.com
// 版    本: version 1.0
// 日    期: 2013-12-28 08:37
// 描    述:
// 历史描述:
////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "ASLCommon.h"
#include "ASLText2Bmp_DotMatrix.h"
int main(void)
{
	HANDLE handle;
	FONTLIB_CFG_OBJ cfg = {
	.FontSize   = 16,
	.FontWidth  = 8,
	.FontHeight = 16,
	.FontType   = TYPE_ASCII,
	.FontPath   = "./font/ASC16.bin",
	};
	ASLText2Bmp_DM_Init(&handle);

	ASLText2Bmp_DM_AddFontLib(handle, &cfg);

	cfg.FontSize   = 16;
	cfg.FontWidth  = 16;
	cfg.FontHeight = 16;
	cfg.FontType   = TYPE_ZH;
	memset(cfg.FontPath, 0, 100);
	memcpy(cfg.FontPath, "./font/HZK16.bin", 100);
	ASLText2Bmp_DM_AddFontLib(handle, &cfg);


	BMP_CFG_OBJ bmpcfg = {
	.FontSize = 16,
	};

	Sint32 len;
	wchar_t data[100];
//  	CHAR *Str = "一二三四五六日";
 	CHAR *Str = "一二";
//  	CHAR Str[12] = {0xd0,0xc7};
//  	CHAR *Str = "12";
	setlocale(LC_ALL, "zh_CN.utf8");
	len = mbstowcs(data, Str, 100);
	printf("get to unicode len:%d\n", len);
	setlocale(LC_ALL, "zh_CN.gbk");
	len = wcstombs(bmpcfg.Str, data, 400);

	BMP_INFO_OBJ BmpInfo;

	ASLText2Bmp_DM_Convert(handle, &bmpcfg, &BmpInfo);

	ASLText2Bmp_DM_Exit(&handle);


}
