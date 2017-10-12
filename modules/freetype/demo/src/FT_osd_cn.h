#ifndef _FT_OSD_CN_H_
#define _FT_OSD_CN_H_
#include <stdio.h>
#include <apostle_common.h>
#include <wchar.h>

typedef HANDLE  OSD_HANDLE;

#define DEFAULT_TOTAL_WIDTH  0
#define DEFAULT_TOTAL_HEIGHT 0

#define DEFAULT_PATH_LEN     100
#define DEFAULT_ENCODE_LEN   20

typedef struct
{
	Sint32 color;
	Uint8  y;
	Uint8  uv;
}COLOR_INFO;
/*
typedef struct
{
	FT_Library   library;
	FT_Face      face;
	FT_GlyphSlot slot;
}
*/
typedef struct
{
	wchar_t	   char_unicode;
	Uint32     size;
	Uint32     font_width;
	Uint32     font_height;
	COLOR_INFO font_color;
	COLOR_INFO background_color;
	Uint8     *buf_y;
	Uint8     *buf_uv;
}FONT_INFO_T;
typedef struct
{
	char*    (*set_locale_fun)(int category, const char *locale);   

	Sint32     font_type;
	Sint32     yuv_format;
	Sint32     font_width;
	Sint32     font_height;
	Sint32     total_width;
	Sint32     total_height;
	COLOR_INFO font_color;
	COLOR_INFO background_color;

	Sint8      font_path[DEFAULT_PATH_LEN];
	Sint8      cn_font_encode_type[DEFAULT_ENCODE_LEN];

}FT_OSD_HANDLE;

#endif
