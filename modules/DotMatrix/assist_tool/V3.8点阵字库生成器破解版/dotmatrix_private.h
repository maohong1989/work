#ifndef _DOTMATRIX_PRIVATE_H_
#define _DOTMATRIX_PRIVATE_H_

#define PATH_HZK16 "HZK16.bin"
#define PATH_ASC16 "ASC16.bin"
#define PATH_HZK24 "HZK24.bin"
#define PATH_ASC24 "ASC24.bin"
typedef unsigned char       OSD_U8;
typedef unsigned short      OSD_U16;
typedef unsigned int        OSD_U32;
typedef unsigned long long  OSD_U64;
typedef signed char         OSD_S8;
typedef signed short        OSD_S16;
typedef signed int          OSD_S32;
typedef signed long long    OSD_S64;
typedef void                *OSD_HANDLE;

#define FONT_BUF_SIZE       (70 * 1000)
#define OSD_OK              (0)
#define OSD_ERROR           (-1)
#define NOT_SUPPORT         (-1)
#define ALIGN_UP(value, align) \
				(((value) & (align-1)) ? \
				 (((value)+(align-1)) & ~(align-1)) : (value))
#define ALIGN_DOWN(value, align) ((value) & ~(align-1))

typedef struct _tag_TEXT2BMP_PATH_FILE_S
{
	FILE *HZK16fp;
	FILE *ASC16fp;
	FILE *HZK24fp;
	FILE *ASC24fp;
}TEXT2BMP_PATH_FIFL_OBJ, *TEXT2BMP_PATH_FIFL_HANDLE;

typedef struct _tag_TEXT2BMP_POS_ATTR_S
{
	OSD_U8  buf[FONT_BUF_SIZE];
	OSD_U32 Offset[100];
	OSD_U32 CharWidth[100];
	OSD_U32 CharNum;
	OSD_U32 FontWidth;
}TEXT2BMP_POS_ATTR_OBJ, *TEXT2BMP_POS_ATTR_HANDLE;

typedef struct _tag_TEXT2BMP_CFG_S
{
	OSD_U8  Str[100];
	OSD_U32 FontSize;
}TEXT2BMP_CFG_OBJ, *TEXT2BMP_CFG_HANDLE;

typedef struct _tag_TEXT2BMP_BASE_ATTR_S
{
	OSD_U8 PixelBackGround;
	OSD_U8 PixelFont;
	OSD_U8 PixelOutline;
}TEXT2BMP_BASE_ATTR_OBJ, *TEXT2BMP_BASE_ATTR_HANDLE;

typedef struct _tag_TEXT2BMP_OSD_ATTR_S
{

	TEXT2BMP_BASE_ATTR_OBJ  PixelBaseAttr;
	TEXT2BMP_PATH_FIFL_OBJ  PathFp;
	TEXT2BMP_POS_ATTR_OBJ   PosInfo;
}TEXT2BMP_OSD_ATTR_OBJ, *TEXT2BMP_OSD_ATTR_HANDLE;

typedef struct _tag_TEXT2BMP_BMP_INFO_S
{
	OSD_U8  *Buf;
	OSD_U32 BufWidth;
	OSD_U32 BufHeight;
}TEXT2BMP_BMP_INFO_OBJ, *TEXT2BMP_BMP_INFO_HANDLE;

/******debug *******************************************/
#define APOSTLE_OSD_DEBUG_DEFINE
#ifdef APOSTLE_OSD_DEBUG_DEFINE
#define OSD_DEBUG_INFO(fmt,arg...) \
		do\
		{\
			fprintf(stdout, fmt, ##arg);\
			fprintf(stdout, "[FILE:%s, FUNCTION:%s, LINE:%d]\n\n",\
					 __FILE__, __FUNCTION__, __LINE__);\
		}while(0);
#define osd_assert_info(x) \
		if((x) == 0){\
			do{\
			fprintf(stderr, "assert:the value unequal, press 'q' to continue\n");\
			fprintf(stderr, "[FILE:%s, FUNCTION:%s, LINE:%d]\n",\
					 __FILE__, __FUNCTION__, __LINE__);\
			}while(getchar()!='q');\
		}
#else
#define OSD_DEBUG_INFO(fmt,arg...)
#define osd_assert_info(x)
#endif

#define OSD_ERROR_INFO(fmt,arg...) \
		do\
		{\
			fprintf(stderr, fmt, ##arg);\
			fprintf(stderr, "[FILE:%s, FUNCTION:%s, LINE:%d]\n\n",\
					 __FILE__, __FUNCTION__, __LINE__);\
		}while(0);
#endif
