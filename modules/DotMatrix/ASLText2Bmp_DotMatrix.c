#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ASLCommon.h"
#include "ASLHashMemMng.h"
#include "ASLText2Bmp_DotMatrix.h"


static Sint32 ASLText2Bmp_OpenFile(FILE **Fd, CHAR *FontPath);
static Sint32 ASLText2Bmp_CloseFile(FILE **Fd);
static Sint32 ASLText2Bmp_LibMngAddLib(TEXT2BMP_FONTLIB_MNG_HANDLE Handle, \
		TEXT2BMP_FONTLIB_ATTR_HANDLE Cfg);
static Sint32 ASLText2Bmp_LibMngInit(TEXT2BMP_FONTLIB_MNG_HANDLE *Handle);
static Sint32 ASLText2Bmp_LibMngExit(TEXT2BMP_FONTLIB_MNG_HANDLE *Handle);
static Sint32 ASLText2Bmp_LibMngGetLib(TEXT2BMP_FONTLIB_MNG_HANDLE Handle, \
		Sint32 FontSize, Sint32 FontType, TEXT2BMP_FONTLIB_ATTR_HANDLE Cfg);
static Sint32 ASLText2Bmp_DM_DrawBmpTwoPixel(TEXT2BMP_OSD_ATTR_HANDLE handle, BMP_INFO_HANDLE bmp_info);
static Sint32 ASLText2Bmp_DM_GetLibData(CHAR *InStr, Sint32 InFontSize, \
		FILE *InAscFd,  FILE *InHzkFd, TEXT2BMP_POS_ATTR_HANDLE OutPosInfo);

Sint32 ASLText2Bmp_DM_Init(HANDLE *Handle)
{
	TEXT2BMP_OSD_ATTR_HANDLE    OsdAttrHandle     = NULL;
	TEXT2BMP_BASE_ATTR_HANDLE   BaseAttrHandle    = NULL;
	TEXT2BMP_ADVC_ATTR_HANDLE   AdvAttrHandle     = NULL;
	TEXT2BMP_FONTLIB_MNG_HANDLE FontLibAttrHandle = NULL;
	UCHAR PixelBackground = 0xff;
	UCHAR PixelFont       = 0x80;
	Sint32 Ret            = 0;

	OsdAttrHandle = malloc(sizeof(TEXT2BMP_OSD_ATTR_OBJ));
	COM_VERFY_NULL(NULL == OsdAttrHandle);

	BaseAttrHandle            = &OsdAttrHandle->PixelBaseAttr;
	AdvAttrHandle             = &OsdAttrHandle->AdvanceAttr;
	OsdAttrHandle->FontLibMng = NULL;
// 	FontLibAttrHandle         = OsdAttrHandle->FontLibMng;

	memset(OsdAttrHandle,  0, sizeof(TEXT2BMP_OSD_ATTR_OBJ));
	memset(BaseAttrHandle, 0, sizeof(TEXT2BMP_BASE_ATTR_OBJ));
	memset(AdvAttrHandle,  0, sizeof(TEXT2BMP_ADVC_ATTR_OBJ));

	BaseAttrHandle->PixelFormat     = TWO_PIXEL_FORMAT_RGB;
	BaseAttrHandle->PixelBackGround = 0x0000;
	BaseAttrHandle->PixelFont       = 0xffff;

	AdvAttrHandle->HoriScale = 0;
	AdvAttrHandle->VertScale = 0;

	Ret = ASLText2Bmp_LibMngInit(&FontLibAttrHandle);
	if (Ret != COM_SUCCESS)
	{
		COM_ERROR_INFO("the font lib manage error!\n");
		return COM_ERROR;
	}

	OsdAttrHandle->FontLibMng = FontLibAttrHandle;
	*Handle                   = OsdAttrHandle;
	return COM_SUCCESS;
}

Sint32 ASLText2Bmp_DM_AddFontLib(HANDLE handle, FONTLIB_CFG_HANDLE Cfg)
{
	TEXT2BMP_OSD_ATTR_HANDLE     OsdAttrHandle = handle;
	TEXT2BMP_FONTLIB_MNG_HANDLE  FontLibAttrHandle;
	TEXT2BMP_FONTLIB_ATTR_OBJ    FontLibCfg;
	Sint32 Ret = 0;

	memset(&FontLibCfg, 0, sizeof(TEXT2BMP_FONTLIB_ATTR_OBJ));
	FontLibAttrHandle = OsdAttrHandle->FontLibMng;

	FontLibCfg.FontSize   = Cfg->FontSize;
	FontLibCfg.FontWidth  = Cfg->FontWidth;
	FontLibCfg.FontHeight = Cfg->FontHeight;
	FontLibCfg.FontType   = Cfg->FontType;

	Ret = ASLText2Bmp_OpenFile(&FontLibCfg.FontLibFp, Cfg->FontPath);
	if (Ret != COM_SUCCESS)
	{
		COM_ERROR_INFO("open file error!\n");
		return COM_ERROR;
	}
	Ret = ASLText2Bmp_LibMngAddLib(FontLibAttrHandle, &FontLibCfg);
	if (Ret != COM_SUCCESS)
	{
		COM_ERROR_INFO("add font lib error!\n");
		return COM_ERROR;
	}
	return COM_SUCCESS;
}

Sint32 ASLText2Bmp_DM_Exit(HANDLE *Handle)
{
	TEXT2BMP_OSD_ATTR_HANDLE     OsdAttrHandle = *Handle;
	TEXT2BMP_FONTLIB_MNG_HANDLE  FontLibAttrHandle;
	Sint32 Ret = 0;

	FontLibAttrHandle = OsdAttrHandle->FontLibMng;
	Ret = ASLText2Bmp_LibMngExit(&FontLibAttrHandle);
	if (Ret != COM_SUCCESS)
	{
		COM_ERROR_INFO("exit error!\n");
		return COM_ERROR;
	}

	if (OsdAttrHandle != NULL)
	{
		free(OsdAttrHandle);
		OsdAttrHandle = NULL;
	}
	*Handle = OsdAttrHandle;
	return COM_SUCCESS;
}
Sint32 ASLText2Bmp_SetBaseAttr(HANDLE Handle, BASE_ATTR_CFG_HANDLE Cfg)
{
	TEXT2BMP_OSD_ATTR_HANDLE  OsdAttrHandle  = Handle;
	TEXT2BMP_BASE_ATTR_HANDLE BaseAttrHandle = NULL;

	BaseAttrHandle = &OsdAttrHandle->PixelBaseAttr;
	BaseAttrHandle->PixelFormat     = Cfg->PixelFormat;
	BaseAttrHandle->PixelBackGround = Cfg->PixelBackGround;
	BaseAttrHandle->PixelFont       = Cfg->PixelFont;
	return COM_SUCCESS;
}
Sint32 ASLText2Bmp_SetAdvanceAttr(HANDLE Handle, ADVC_CFG_HANDLE Cfg)
{
	TEXT2BMP_OSD_ATTR_HANDLE  OsdAttrHandle = Handle;
	TEXT2BMP_ADVC_ATTR_HANDLE AdvAttrHandle = NULL;

	AdvAttrHandle = &OsdAttrHandle->AdvanceAttr;
	AdvAttrHandle->HoriScale = Cfg->HoriScale + 1;
	AdvAttrHandle->VertScale = Cfg->VertScale + 1;

	return COM_SUCCESS;
}
Sint32 ASLText2Bmp_DM_Convert(HANDLE Handle, BMP_CFG_HANDLE BmpCfg, BMP_INFO_HANDLE BmpInfo)
{
	TEXT2BMP_OSD_ATTR_HANDLE    OsdAttrHandle     = Handle;
	TEXT2BMP_FONTLIB_MNG_HANDLE FontLibAttrHandle = OsdAttrHandle->FontLibMng;
	TEXT2BMP_FONTLIB_MNG_HANDLE PosAttrHandle     = &OsdAttrHandle->PosAttr;
	TEXT2BMP_FONTLIB_ATTR_OBJ   FontLibAttr;
	Sint32 Ret = 0;

	Sint32 FontSize    = BmpCfg->FontSize;
	CHAR   *ConvertStr = BmpCfg->Str;
	Sint32 i           = 0;
	FILE *AscFd        = NULL;
	FILE *HzkFd        = NULL;

	Ret = ASLText2Bmp_LibMngGetLib(FontLibAttrHandle, FontSize, TYPE_ASCII, &FontLibAttr);
	if (COM_SUCCESS != Ret)
	{
		COM_ERROR_INFO("get the lib attr error!\n");
		return COM_ERROR;
	}
	AscFd = FontLibAttr.FontLibFp;

	Ret = ASLText2Bmp_LibMngGetLib(FontLibAttrHandle, FontSize, TYPE_ZH, &FontLibAttr);
	if (COM_SUCCESS != Ret)
	{
		COM_ERROR_INFO("get the lib attr error!\n");
		return COM_ERROR;
	}
	HzkFd = FontLibAttr.FontLibFp;

	Ret = ASLText2Bmp_DM_GetLibData(ConvertStr, FontSize, AscFd, HzkFd, PosAttrHandle);
	if (COM_SUCCESS != Ret)
	{
		COM_ERROR_INFO("get lib data error!\n");
		return COM_ERROR;
	}

	Ret = ASLText2Bmp_DM_DrawBmpTwoPixel(OsdAttrHandle, BmpInfo);
	if (COM_SUCCESS != Ret)
	{
		COM_ERROR_INFO("draw the bmp error!\n");
		return COM_ERROR;
	}

	return COM_SUCCESS;
}
static Sint32 ASLText2Bmp_DM_GetLibData(CHAR *InStr, Sint32 InFontSize, \
		FILE *InAscFd,  FILE *InHzkFd, TEXT2BMP_POS_ATTR_HANDLE OutPosInfo)
{
	Uint32 FontSize       = InFontSize;
	UCHAR   *Str          = InStr;
	FILE  *AscFd          = InAscFd;
	FILE  *HzkFd          = InHzkFd;
	Uint32 HzkCharBufSize = 0;
	Uint32 AscCharBufSize = 0;
	Uint32 Offset         = 0;
	Uint64 location       = 0;
	Sint32 ret            = 0;
	Uint32 CharNum        = 0;

	TEXT2BMP_POS_ATTR_HANDLE BufInfo = OutPosInfo;

	HzkCharBufSize = FontSize * COM_ALIGN_UP(FontSize, 8) >> 3;
	AscCharBufSize = FontSize * COM_ALIGN_UP(FontSize / 2, 8) >> 3;

	BufInfo->FontSize = FontSize;

	while(*Str)
	{
		if (*Str & 0x80)
		{
			location = ((Str[0] - 0xa1) * 94 + (Str[1] - 0xa1)) * HzkCharBufSize;
			fseek(HzkFd, location + FONTLIB_HEAD, SEEK_SET);
			ret = fread(BufInfo->buf + Offset, 1, HzkCharBufSize, HzkFd);
			if (0 == ret)
			{
				COM_ERROR_INFO("read error!\n");
				return COM_ERROR;
			}
			Offset += HzkCharBufSize;
			Str    += 2;
			BufInfo->CharWidth[BufInfo->CharNum] = FontSize;
// 			BufInfo->CharWidth[BufInfo->CharNum] = COM_ALIGN_UP(FontSize, 8);
			BufInfo->Offset[BufInfo->CharNum]    = HzkCharBufSize;
			BufInfo->CharNum++;
		}
		else
		{
			location = AscCharBufSize * Str[0];
			fseek(AscFd, location + FONTLIB_HEAD, SEEK_SET);
			ret = fread(BufInfo->buf + Offset, 1, AscCharBufSize, AscFd);
			if (0 == ret)
			{
				COM_ERROR_INFO("read error!\n");
				return COM_ERROR;
			}
			Offset += AscCharBufSize;
			Str    += 1;
			BufInfo->CharWidth[BufInfo->CharNum] = FontSize / 2;
// 			BufInfo->CharWidth[BufInfo->CharNum] = COM_ALIGN_UP(FontSize/2, 8);
			BufInfo->Offset[BufInfo->CharNum]    = AscCharBufSize;
			BufInfo->CharNum++;
		}
	}

//++==apostle++[2014-02-20]+++++++++++++++++++++++++++++++++++++++++++++
#if 0
	int i, j, k;
	char *tmp;
	Offset = 0;
	for(i = 0; i < BufInfo->CharNum; i++)
	{
		for (j = 0; j < BufInfo->Offset[i]; j++)
		{
			tmp = BufInfo->buf + Offset;
			Offset += 1;

			if (j % (COM_ALIGN_UP(BufInfo->CharWidth[i], 8) >> 3) == 0) 
			{
				printf("\n");	
			}
			for (k = 0; k < 8; k++)
			{
				if ((*tmp) & (0x80 >> k))
					printf("0");
				else
					printf("-");
			}
		}
		printf("\n");
	}
#endif
//++__apostle++[2014-02-20]+++++++++++++++++++++++++++++++++++++++++++++

	return COM_SUCCESS;
}

static Sint32 ASLText2Bmp_DM_DrawBmpTwoPixel(TEXT2BMP_OSD_ATTR_HANDLE handle, BMP_INFO_HANDLE bmp_info)
{
	TEXT2BMP_POS_ATTR_HANDLE PosInfo    = &(handle->PosAttr);
	TEXT2BMP_BASE_ATTR_HANDLE  BaseInfo = &(handle->PixelBaseAttr);
	TEXT2BMP_ADVC_ATTR_HANDLE  AdvcInfo = &(handle->AdvanceAttr);
	BMP_INFO_HANDLE BmpInfo = bmp_info;

	Uint32 HoriScale      = AdvcInfo->HoriScale;
	Uint32 VertScale      = AdvcInfo->VertScale;
	Uint32 CharNum        = PosInfo->CharNum;
	CHAR   *SrcBuf        = PosInfo->buf;
	CHAR   *OffsetSrcBuf  = NULL;
	Uint32 i              = 0;
	Uint32 j              = 0;
	Uint32 k              = 0;
	Uint32 BufWidth       = 0;
	Uint32 BufHeight      = 0;
	Uint32 FontAlignWdith = 0;
	Uint32 Offset         = 0;
	Uint32 CharWidthNum   = 0;

	HoriScale = HoriScale + 1;
	VertScale = VertScale + 1;
	for (i = 0; i < CharNum; i++)
	{
		BufWidth += PosInfo->CharWidth[i];
	}
	BufHeight = PosInfo->FontSize;
	BufWidth  = BufWidth * HoriScale;
	BufHeight = BufHeight * VertScale;
///////////////////////////////////////////////////
	Uint16 *DesBuf         = NULL;
	Uint16 *OffSetBuf      = NULL;
	Uint16 *TmpBuf         = NULL;
	Uint16 BackGroundValue = (Uint16)(BaseInfo->PixelBackGround & 0xFFFF);
	Uint16 FontValue       = (Uint16)(BaseInfo->PixelFont & 0xFFFF);

	DesBuf    = malloc(BufWidth*BufHeight*2);
	COM_VERFY_NULL(NULL == DesBuf);
	memset(DesBuf, BackGroundValue, BufWidth*BufHeight*2);

	OffSetBuf = DesBuf;
	for (i = 0; i < CharNum; i++)
	{
		FontAlignWdith = PosInfo->CharWidth[i];
		FontAlignWdith = COM_ALIGN_UP(FontAlignWdith, 8) >> 3;
		OffSetBuf      = DesBuf + Offset - BufWidth; //first value
		Offset        += PosInfo->CharWidth[i]*HoriScale;
		for (j = 0; j < PosInfo->Offset[i]*VertScale; j++)
		{
			if (j % FontAlignWdith == 0)
			{
				OffSetBuf    = OffSetBuf + BufWidth;
				TmpBuf       = OffSetBuf;
				if ((j / FontAlignWdith) % VertScale != 0)
				{
					SrcBuf = OffsetSrcBuf;
				}
				else
				{
					OffsetSrcBuf = SrcBuf;
				}
// 				printf("\n");
			}

			for (k = 0; k < 8*HoriScale; k++)
			{
				if ((*SrcBuf) & (0x80 >> (k/HoriScale)))
				{
					*TmpBuf = FontValue;
// 					printf("0");
				}
				else
				{
// 					printf("-");
				}
				TmpBuf++;

				CharWidthNum++;
				if (CharWidthNum == PosInfo->CharWidth[i]*HoriScale)
				{
					CharWidthNum = 0;
					break;
				}
			}
			SrcBuf++;
		}
	}

	BmpInfo->BufWidth  = BufWidth;
	BmpInfo->BufHeight = BufHeight;
	BmpInfo->BufStartX = 0;
	BmpInfo->BufStartY = 0;
	BmpInfo->Buf       = (CHAR *)DesBuf;
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
	return COM_SUCCESS;
}



//font lib manage
//////////////////////////////////////////////////////////////////
static Sint32 ASLText2Bmp_OpenFile(FILE **Fd, CHAR *FontPath)
{
	if (NULL == (*Fd = fopen(FontPath, "rb")))
	{
		COM_ERROR_INFO("con't open the %s\n", FontPath);
		return COM_ERROR;
	}
	return COM_SUCCESS;
}
static Sint32 ASLText2Bmp_CloseFile(FILE **Fd)
{
	if (*Fd != NULL)
	{
		fclose(*Fd);
		*Fd = NULL;
	}
	return COM_SUCCESS;
}
///////////////private///////////////////////////////////////////////////////
//
static Sint32 ASLText2Bmp_LibMngInit(TEXT2BMP_FONTLIB_MNG_HANDLE *Handle)
{
	HANDLE FontLibAttrHandle;
	Sint32 Ret = 0;
	Ret = ASLHashMem_Init(&FontLibAttrHandle, 2);
	if (Ret != COM_SUCCESS)
	{
		COM_ERROR_INFO("the init hash mem error!\n");
		return COM_ERROR;
	}
	*Handle = FontLibAttrHandle;
	return COM_SUCCESS;
}
static Sint32 ASLText2Bmp_LibMngGetLib(TEXT2BMP_FONTLIB_MNG_HANDLE Handle, \
		Sint32 FontSize, Sint32 FontType, TEXT2BMP_FONTLIB_ATTR_HANDLE Cfg)
{
	HANDLE FontLibAttrHandle = Handle;
	Sint32 Ret     = 0;
	Sint32 HashKey = 0;
	HashKey        = FontType;
	Ret = ASLHashMem_GetData_VerifyValue(FontLibAttrHandle, HashKey, &FontSize, Cfg);
	if (Ret != COM_SUCCESS)
	{
		COM_ERROR_INFO("fontlib add data error!\n");
		return COM_ERROR;
	}
	return COM_SUCCESS;
}
static Sint32 ASLText2Bmp_LibMngAddLib(TEXT2BMP_FONTLIB_MNG_HANDLE Handle, \
		TEXT2BMP_FONTLIB_ATTR_HANDLE Cfg)
{
	HANDLE FontLibAttrHandle = Handle;
	Sint32 Ret     = 0;
	Sint32 HashKey = 0;
	HashKey        = Cfg->FontType;

	Ret = ASLHashMem_AddData(FontLibAttrHandle, HashKey, Cfg);
	if (Ret != COM_SUCCESS)
	{
		COM_ERROR_INFO("fontlib add data error!\n");
		return COM_ERROR;
	}
	return COM_SUCCESS;
}
static Sint32 ASLText2Bmp_LibMngExit(TEXT2BMP_FONTLIB_MNG_HANDLE *Handle)
{
	HANDLE FontLibAttrHandle = *Handle;
	Sint32 Ret = 0;
	Ret = ASLHashMem_Exit(&FontLibAttrHandle);
	if (Ret != COM_SUCCESS)
	{
		COM_ERROR_INFO("the init hash mem error!\n");
		return COM_ERROR;
	}
	*Handle = FontLibAttrHandle;
	return COM_SUCCESS;
}
Sint32 AddData(HANDLE *DesHashData, HANDLE SrcInData)
{
	*DesHashData = malloc(sizeof(TEXT2BMP_FONTLIB_ATTR_OBJ));
	COM_VERFY_NULL(*DesHashData == NULL);
	memcpy(*DesHashData, SrcInData, sizeof(TEXT2BMP_FONTLIB_ATTR_OBJ));
	return COM_SUCCESS;
}
Sint32 DeleteData(HANDLE *HashData)
{
	free(*HashData);
	*HashData = NULL;
	return COM_SUCCESS;
}
Sint32 GetData(HANDLE DesInData, HANDLE SrcHashData)
{
	memcpy(DesInData, SrcHashData, sizeof(TEXT2BMP_FONTLIB_ATTR_OBJ));
	return COM_SUCCESS;
}
Sint32 VerifyValue(HANDLE HashData, HANDLE InValue)
{
	TEXT2BMP_FONTLIB_ATTR_HANDLE FontLibCfgHandle = HashData;
	Uint32 Value = *(Uint32*)InValue;
	if (FontLibCfgHandle->FontSize == Value)
		return COM_SUCCESS;
	else
		return COM_ERROR;
}
Sint32 PrintData(HANDLE HashData, CHAR *PrintHead)
{
	TEXT2BMP_FONTLIB_ATTR_HANDLE FontLibCfgHandle = HashData;
	printf("%s FontSize   = [%d]\n", PrintHead, FontLibCfgHandle->FontSize);
	printf("%s FontWidth  = [%d]\n", PrintHead, FontLibCfgHandle->FontWidth);
	printf("%s FontHeight = [%d]\n", PrintHead, FontLibCfgHandle->FontHeight);
	printf("%s FontType   = [%d]\n", PrintHead, FontLibCfgHandle->FontType);
	return COM_SUCCESS;
}
