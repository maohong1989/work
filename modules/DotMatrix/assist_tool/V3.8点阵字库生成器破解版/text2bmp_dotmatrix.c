#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "dotmatrix_private.h"

FILE *HZK16fp = NULL;
FILE *ASC16fp = NULL;

OSD_S32 dotmatrix_init(TEXT2BMP_PATH_FIFL_HANDLE handle)
{
	if (NULL == (handle->HZK16fp = fopen(PATH_HZK16, "rb")))
	{
		OSD_ERROR_INFO("can't open the HZK16\n");
		return OSD_ERROR;
	}
	if (NULL == (handle->ASC16fp = fopen(PATH_ASC16, "rb")))
	{
		OSD_ERROR_INFO("can't open the ASC16\n");
		return OSD_ERROR;
	}
	if (NULL == (handle->HZK24fp = fopen(PATH_HZK24, "rb")))
	{
		OSD_ERROR_INFO("can't open the HZK24\n");
		return OSD_ERROR;
	}
	if (NULL == (handle->ASC24fp = fopen(PATH_ASC24, "rb")))
	{
		OSD_ERROR_INFO("can't open the ASC24\n");
		return OSD_ERROR;
	}
	return 0;
}
OSD_S32 dotmatrix_exit(TEXT2BMP_PATH_FIFL_HANDLE handle)
{
	if (NULL != handle->HZK16fp)
	{
		fclose(handle->HZK16fp);
		handle->HZK16fp = NULL;
	}
	if (NULL != handle->ASC16fp)
	{
		fclose(handle->ASC16fp);
		handle->ASC16fp = NULL;
	}
	if (NULL != handle->HZK24fp)
	{
		fclose(handle->HZK24fp);
		handle->HZK24fp = NULL;
	}
	if (NULL != handle->ASC24fp)
	{
		fclose(handle->ASC24fp);
		handle->ASC24fp = NULL;
	}
	return OSD_OK;
}

OSD_S32 dotmatrix_get_datas(TEXT2BMP_OSD_ATTR_HANDLE handle, TEXT2BMP_CFG_HANDLE cfg)
{
	OSD_U32 FontSize       = cfg->FontSize;
	OSD_U8  *Str           = cfg->Str;
	FILE    *AscFd         = NULL;
	FILE    *HzkFd         = NULL;
	OSD_U32 HzkCharBufSize = 0;
	OSD_U32 AscCharBufSize = 0;
	OSD_U32 Offset         = 0;
	OSD_U64 location       = 0;
	OSD_S32 ret            = 0;
	OSD_U32 CharNum        = 0;

	TEXT2BMP_POS_ATTR_HANDLE BufInfo;
	BufInfo = &(handle->PosInfo);
	BufInfo->FontWidth = FontSize;

	switch(FontSize)
	{
		case 16:
			{
				AscFd          = handle->PathFp.ASC16fp;
				HzkFd          = handle->PathFp.HZK16fp;
				HzkCharBufSize = FontSize * ALIGN_UP(FontSize, 8) >> 3;
				AscCharBufSize = FontSize * ALIGN_UP(FontSize / 2, 8) >> 3;
				break;
			}
		case 24:
			{
				AscFd          = handle->PathFp.ASC24fp;
				HzkFd          = handle->PathFp.HZK24fp;
				HzkCharBufSize = FontSize * ALIGN_UP(FontSize, 8) >> 3;
				AscCharBufSize = FontSize * ALIGN_UP(FontSize / 2, 8) >> 3;
				break;
			}
		default:
			break;
	}

	while(*Str)
	{
		if (*Str & 0x80)
		{
			location = ((Str[0] - 0xa1) * 94 + (Str[1] - 0xa1)) * HzkCharBufSize;
			fseek(HzkFd, location, SEEK_SET);
			ret = fread(BufInfo->buf + Offset, 1, HzkCharBufSize, HzkFd);
			if (0 == ret)
			{
				OSD_ERROR_INFO("read error!\n");
				return OSD_ERROR;
			}
			Offset += HzkCharBufSize;
			Str    += 2;
// 			BufInfo->CharWidth[BufInfo->CharNum] = FontSize;
			BufInfo->CharWidth[BufInfo->CharNum] = ALIGN_UP(FontSize, 8);
			BufInfo->Offset[BufInfo->CharNum]    = HzkCharBufSize;
			BufInfo->CharNum++;
		}
		else
		{
			location = AscCharBufSize * Str[0];
			fseek(AscFd, location, SEEK_SET);
			ret = fread(BufInfo->buf + Offset, 1, AscCharBufSize, AscFd);
			if (0 == ret)
			{
				OSD_ERROR_INFO("read error!\n");
				return OSD_ERROR;
			}
			Offset += AscCharBufSize;
			Str    += 1;
// 			BufInfo->CharWidth[BufInfo->CharNum] = FontSize / 2;
			BufInfo->CharWidth[BufInfo->CharNum] = ALIGN_UP(FontSize/2, 8);
			BufInfo->Offset[BufInfo->CharNum]    = AscCharBufSize;
			BufInfo->CharNum++;
		}
		if (Offset + HzkCharBufSize > FONT_BUF_SIZE)
		{
			OSD_ERROR_INFO("the buffer too short!\n");
			return OSD_ERROR;
		}
	}
	return OSD_OK;
}

OSD_S32 dotmatrix_draw_bmp(TEXT2BMP_OSD_ATTR_HANDLE handle, TEXT2BMP_BMP_INFO_HANDLE bmp_info)
{
	TEXT2BMP_POS_ATTR_HANDLE PosInfo    = &(handle->PosInfo);
	TEXT2BMP_BMP_INFO_HANDLE BmpInfo    = bmp_info;
	TEXT2BMP_BASE_ATTR_HANDLE  BaseInfo = &(handle->PixelBaseAttr);

	OSD_U32 CharNum         = PosInfo->CharNum;
	OSD_U8  BackGroundValue = BaseInfo->PixelBackGround;
	OSD_U8 FontValue        = BaseInfo->PixelFont;
	OSD_U8 *SrcBuf          = PosInfo->buf;
	OSD_U32 i               = 0;
	OSD_U32 j               = 0;
	OSD_U32 k               = 0;
	OSD_U32 BufWidth        = 0;
	OSD_U32 BufHeight       = 0;
	OSD_U8 *DesBuf          = NULL;
	OSD_U8 *OffSetBuf       = NULL;
	OSD_U8 *TmpBuf          = NULL;
	OSD_U32 FontAlignWdith  = 0;
	OSD_U32 Offset          = 0;

	for (i = 0; i < CharNum; i++)
	{
		BufWidth += PosInfo->CharWidth[i];
	}
	BufHeight = PosInfo->FontWidth;

	DesBuf    = malloc(BufWidth*BufHeight);
	if (NULL == DesBuf)
	{
		OSD_ERROR_INFO("malloc error!\n");
		return OSD_ERROR;
	}

	memset(DesBuf, BackGroundValue, BufWidth*BufHeight);

	OffSetBuf = DesBuf;
	for (i = 0; i < CharNum; i++)
	{
		FontAlignWdith = PosInfo->CharWidth[i];
		FontAlignWdith = ALIGN_UP(FontAlignWdith, 8) >> 3;
		OffSetBuf      = DesBuf + Offset - BufWidth; //first value
		Offset        += PosInfo->CharWidth[i];

		for (j = 0; j < PosInfo->Offset[i]; j++)
		{
			if (j % FontAlignWdith == 0)
			{
				OffSetBuf = OffSetBuf + BufWidth;
				TmpBuf    = OffSetBuf;
// 				printf("\n");
			}

			for (k = 0; k < 8; k++)
			{
				if ((*SrcBuf) & (0x80 >> k))
				{
					*TmpBuf = FontValue;
// 					printf("0");
				}
				else
				{
// 					printf("-");
				}
				TmpBuf++;
			}
			SrcBuf++;
		}
	}
	BmpInfo->BufWidth  = BufWidth;
	BmpInfo->BufHeight = BufHeight;
	BmpInfo->Buf       = DesBuf;
//++==apostle++[2013-11-30]+++++++++++++++++++++++++++++++++
#if 1
	printf("BmpInfo->BufWidth  = [%d]\n", BmpInfo->BufWidth);
	printf("BmpInfo->BufHeight = [%d]\n", BmpInfo->BufHeight);
// 	printf("BmpInfo->Buf       = [%d]\n", BmpInfo->Buf);
	printf("\n");
for (i = 0; i < BufHeight; i++)
{
	for (j = 0; j < BufWidth; j++)
	{
		if (*DesBuf)
			printf("0");
		else
			printf("-");
		DesBuf++;
	}
	printf("\n");
}
	printf("\n");
#endif
//++__apostle++[2013-11-30]+++++++++++++++++++++++++++++++++
	return OSD_OK;
}
OSD_S32 dotmatrix_set_attr(TEXT2BMP_OSD_ATTR_HANDLE handle, TEXT2BMP_BASE_ATTR_HANDLE cfg)
{
	handle->PixelBaseAttr.PixelBackGround = cfg->PixelBackGround;
	handle->PixelBaseAttr.PixelFont       = cfg->PixelFont;
	handle->PixelBaseAttr.PixelOutline    = cfg->PixelOutline;
	return OSD_OK;
}

