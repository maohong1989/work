#ifndef __ASLTEXT2BMP_DOTMATRIX_H__
#define __ASLTEXT2BMP_DOTMATRIX_H__
#define PIXELCHAR_N 8

typedef enum _tag_PIXEL_FORMAT_E
{
	THREE_PIXEL_FORMAT_YUV420 = 0,  //three pixel len, y u v
	TWO_PIXEL_FORMAT_RGB,           //
	ONE_PIXEL_FORMAT_RGB,
	PIXEL_FORMAT_END
}OSD_PIXEL_FORMAT_E;

typedef enum _tag_FONT_TYPE_E
{
	TYPE_ASCII = 0,
	TYPE_ZH,
	TYPE_END
}FONT_TYPE_E;

typedef struct _tags_TEXT2BMP_LIB_CFG_S
{
	Uint32      FontSize;
	Uint32      FontWidth;
	Uint32      FontHeight;
	FONT_TYPE_E FontType;   //TYPE_ZH-中文 TYPE_ASCII-ascii 
	CHAR        FontPath[100];
}FONTLIB_CFG_OBJ,*FONTLIB_CFG_HANDLE;

typedef struct _tag_BASE_ATTR_CFG_S
{
	OSD_PIXEL_FORMAT_E PixelFormat;
	Uint64             PixelBackGround; // 8 char compatible other pixel
	Uint64             PixelFont;
}BASE_ATTR_CFG_OBJ, *BASE_ATTR_CFG_HANDLE;

typedef struct _tag_ADVC_CFG_S
{
	Uint32 HoriScale;
	Uint32 VertScale;
}ADVC_CFG_OBJ, *ADVC_CFG_HANDLE;

typedef struct _tag_BMP_INFO_S
{
	CHAR   *Buf;
	Uint32 BufWidth;
	Uint32 BufHeight;
	Uint32 BufStartX;
	Uint32 BufStartY;
}BMP_INFO_OBJ, *BMP_INFO_HANDLE;
typedef struct _tag_TEXT2BMP_CFG_S
{
	CHAR   Str[100];
	Uint32 FontSize;
}BMP_CFG_OBJ, *BMP_CFG_HANDLE;
//////////////////////////////////////////////////////
#define FONTLIB_HEAD (0)
typedef struct _tag_TEXT2BMP_POS_ATTR_S
{
	Uint32 CharNum;
	Uint32 FontSize;
	Uint32 CharWidth[100];
	Uint32 CharHeight[100];
	Uint32 Offset[100];
	CHAR   buf[100*32*32];
}TEXT2BMP_POS_ATTR_OBJ, *TEXT2BMP_POS_ATTR_HANDLE;

typedef void* TEXT2BMP_FONTLIB_MNG_HANDLE;

typedef struct _tag_TEXT2BMP_FONTLIB_ATTR_S
{
	Uint32       FontSize;
	Uint32       FontWidth;
	Uint32       FontHeight;
	FONT_TYPE_E  FontType;   //TYPE_ZH-中文 TYPE_ASCII-ascii 
	FILE         *FontLibFp;
}TEXT2BMP_FONTLIB_ATTR_OBJ, *TEXT2BMP_FONTLIB_ATTR_HANDLE;

typedef struct _tag_TEXT2BMP_BASE_ATTR_S
{
	OSD_PIXEL_FORMAT_E PixelFormat;
	Uint64             PixelBackGround; // 8 char compatible other pixel
	Uint64             PixelFont;
	Uint64             PixelOutline;    //no support
}TEXT2BMP_BASE_ATTR_OBJ, *TEXT2BMP_BASE_ATTR_HANDLE;

//advance function
typedef struct _tag_TEXT2BMP_ADVC_ATTR_S
{
	Uint32 HoriScale;
	Uint32 VertScale;
	Uint32 OutlineWidth;  //no support
	Uint32 HoriBold;      //no support
	Uint32 VertBold;      //no support
	Uint32 Italic;        //no support
	Uint32 AntiAlias;     //no support
}TEXT2BMP_ADVC_ATTR_OBJ, *TEXT2BMP_ADVC_ATTR_HANDLE;

typedef struct _tag_TEXT2BMP_OSD_ATTR_S
{
	TEXT2BMP_BASE_ATTR_OBJ      PixelBaseAttr;
	TEXT2BMP_ADVC_ATTR_OBJ      AdvanceAttr;
	TEXT2BMP_POS_ATTR_OBJ       PosAttr;
	TEXT2BMP_FONTLIB_MNG_HANDLE FontLibMng;
}TEXT2BMP_OSD_ATTR_OBJ, *TEXT2BMP_OSD_ATTR_HANDLE;

Sint32 ASLText2Bmp_DM_Init(HANDLE *Handle);
Sint32 ASLText2Bmp_SetBaseAttr(HANDLE Handle, BASE_ATTR_CFG_HANDLE Cfg);
Sint32 ASLText2Bmp_DM_AddFontLib(HANDLE handle, FONTLIB_CFG_HANDLE Cfg);
Sint32 ASLText2Bmp_SetAdvanceAttr(HANDLE Handle, ADVC_CFG_HANDLE Cfg);
Sint32 ASLText2Bmp_DM_Convert(HANDLE Handle, BMP_CFG_HANDLE BmpCfg, \
								BMP_INFO_HANDLE BmpInfo);
Sint32 ASLText2Bmp_DM_Exit(HANDLE *Handle);

#endif
