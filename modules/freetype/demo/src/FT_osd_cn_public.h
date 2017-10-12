#ifndef _FT_OSD_CN_PUBLIC_H_
#define _FT_OSD_CN_PUBLIC_H_
#include <apostle_common.h>
#include <stdio.h>
#define OSD_ERROR              -1
#define OSD_SUCCESS             0

#define FT_OSD_WHITE            0
#define FT_OSD_WHITE_Y          0x00
#define FT_OSD_WHITE_UV         0x80

#define FT_OSD_BLACK            1
#define FT_OSD_BLACK_Y          0xFF
#define FT_OSD_BLACK_UV         0x80
#define FT_OSD_USER_DEFINE      3

#define HORIZONTAL_FONT         0
#define VERTICAL_FONT           1
#define END_FONT                2

#define YUV_FORMAT_RAW          0
#define YUV_FORMAT_422          1
#define YUV_FORMAT_420          2
#define YUV_FORMAT_END          3

#define ENCODE_GB2312           "zh_CN.gbk"
#define ENCODE_UTF8             "zh_CN.UTF-8"

#define DEFAULT_VALUE            -1
#define DEFAULT_FONT_COLOR      FT_OSD_WHITE
#define DEFAULT_FONT_Y           FT_OSD_WHITE_Y
#define DEFAULT_FONT_UV          FT_OSD_WHITE_UV
#define DEFAULT_BACKGROUND_COLOR FT_OSD_BLACK
#define DEFAULT_BACKGROUND_Y     FT_OSD_BLACK_Y
#define DEFAULT_BACKGROUND_UV    FT_OSD_BLACK_UV
#define DEFAULT_YUV_FORMAT       YUV_FORMAT_420
#define DEFAULT_FONT_TYPE        HORIZONTAL_FONT
#define DEFAULT_FONT_WIDTH      32
#define DEFAULT_FONT_HEIGHT     32


/***************************************/
/****need modify***********************/
#define DEFAULT_FONT_PATH       "/home"
#define DEFAULT_FONT_ENCODE     ENCODE_GB2312
 /***************************************/


typedef struct
{
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
}FT_OSD_CONFIG;

typedef enum
{
	FT_OSD_GET_YUV = 0,
	FT_OSD_END
}FT_OSD_CMD_E;

Sint32 ft_osd_open(OSD_HANDLE *handle);
Sint32 ft_osd_set_config(OSD_HANDLE handle, FT_OSD_CONFIG config);
#endif
