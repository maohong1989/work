#include <stdio.h>
#include <ft2build.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <wchar.h>
#include <stdlib.h>

#include FT_FREETYPE_H

//#define NIRVANA_DEBUG

#define YUV_Y		0
#define YUV_UV		1
#define WIDTH		0
#define HEIGHT		1
#define BLACK		0
#define WHITE		1

static FT_Library library;
static FT_Face face;
static FT_GlyphSlot slot;

typedef struct yuv_param_s
{
	char *buf_y;
	char *buf_uv;
	unsigned long size;
	unsigned int width;
	unsigned int height;
}yuv_param_t;

typedef struct font_param_s
{
	unsigned int charCode;
	unsigned int size;		//total buffer size
	unsigned int width;		//bitmap width
	unsigned int height;	//bitmap height
	unsigned int color;
	char *buf_y;
	char *buf_uv;
}font_param_t;

static yuv_param_t yuv;

int string2bitmap(int font_size, unsigned int *wstring, int count, int color);
int osd_cn_main(char *str, int color, int size);
int osd_merge(font_param_t *src, yuv_param_t *dst, int src_count);
int show_bitmap(font_param_t *font,int font_size);
char *yuv_buffer(int param);
unsigned long yuv_buffer_size(int param);
int osd_set_window_param(void);

char *buf_uv = NULL, *buf_y = NULL;
font_param_t *font = NULL;

int osd_cn_main(char *string, int color, int size)
{
	wchar_t wstring[50];
	char tmp[5] = "";
	int count;
	int i;

	if (size < 0)
	{
		printf("osd_cn_main size error!\n");
		return -1;
	}

#ifdef NIRVANA_DEBUG
	printf("__Nirvana AVserver debug : osd_string->%s___\n", string);
#endif
	count = mbstowcs(wstring, string, 2 * strlen(string));

	//字符串中包含不能解析的字符导致函数失败，将其替换'?'
	while (count < 0)
	{
		for (i = 0; i < strlen(string); i++)
		{
			if (string[i] <= 0x80)
			{
				tmp[0] = string[i];
				count = mbstowcs(wstring, tmp, 2 * strlen(tmp));
				if (count == -1)
				{
					string[i] = '?';
				}
			}
			else
			{
				tmp[0] = string[i];
				tmp[1] = string[i + 1];
				count = mbstowcs(wstring, tmp, 2 * strlen(tmp));
				if (count < 0)
				{
					string[i] = '?';
					string[i + 1] = '?';
				}
				i++;
			}
		}
		count = mbstowcs(wstring, string, 2 * strlen(string));
	}

	string2bitmap(size, wstring, count, color);

	return 0;
}

// 0 : 字体大小 1 : 字符串 2 : 字符个数
int string2bitmap(int font_size, unsigned int *wstring, int count, int color)
{
	int i = 0;
	font = (font_param_t *)malloc(count * sizeof(font_param_t));

	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		printf("Freetype library init fail!\n");
		return -1;
	}

	error = FT_New_Face(library, "./gkai00mp.ttf", 0, &face);
	if (error == FT_Err_Unknown_File_Format)
	{
		printf("Font format not supported!\n");	
		return -1;
	}
	else if (error)
	{
		printf("Can't not open font file, maybe it's bad!\n");
		return -1;
	}

	error = FT_Set_Pixel_Sizes(face, 0, font_size);
	if (error)
	{
		printf("FT_Set_Pixel_Size failed!\n");
		return -1;
	}

	memset(&yuv, 0, sizeof(yuv));
	for (i = 0; i < count; i++)
	{
		font[i].charCode = wstring[i];
		font[i].color = color;
		show_bitmap(&font[i], font_size);
		yuv.size += font[i].size;
		yuv.width += font[i].width;
#ifdef NIRVANA_DEBUG
		printf("Word %d : buff_size = %ld\n", i, (long int)font[i].size);
#endif
	}
#ifdef NIRVANA_DEBUG
	printf("yuv buffer size = %ld\n", yuv.size);
	printf("yuv buffer width = %d\n", yuv.width);
#endif

	yuv.height = font_size;
	yuv.buf_y = (char *)malloc(yuv.size);
	yuv.buf_uv = (char *)malloc(yuv.size / 2);

	osd_merge(font, &yuv, count);

	FT_Done_Face(face);
	face = NULL;
	FT_Done_FreeType(library);
	library = NULL;

	return 0;
}

void yuv_free(void)
{
	free(yuv.buf_y);
	yuv.buf_y = NULL;

	free(yuv.buf_uv);
	yuv.buf_uv = NULL;

	free(buf_uv);
	buf_uv = NULL;

	free(buf_y);
	buf_y = NULL;

	free(font);
	font = NULL;
}

char *yuv_buffer(int param)
{
	if (param == YUV_Y)
	{
		return yuv.buf_y;
	}
	else if (param == YUV_UV)
	{
		return yuv.buf_uv;
	}
	else
	{
		printf("Error param for yuv_buffer.\n");
		return NULL;
	}
}

unsigned long yuv_buffer_size(int param)
{
	if (param == WIDTH)
	{
		return yuv.width;
	}
	else if (param == HEIGHT)
	{
		return yuv.height;
	}
	else
	{
		printf("Error param for yuv_buffer_size.\n");
		return -1;
	}
}


int osd_merge(font_param_t *src, yuv_param_t *dst, int src_count)
{
	int i, j, k, count;
	int height = src[0].height;

	for (j = 0, count = 0; j < height; j++)
	{
		for (k = 0; k < src_count; k++)
		{
			for (i = 0; i < src[k].width; i++)
			{
				dst->buf_y[count++] = src[k].buf_y[i + j * src[k].width];
			}
		}
	}

	for (j = 0, count = 0; j < height / 2; j++)
	{
		for (k = 0; k < src_count; k++)
		{
			for (i = 0; i < src[k].width; i++)
			{
				dst->buf_uv[count++] = src[k].buf_uv[i + j * src[k].width];
			}
		}
	}

	return 0;
}

int show_bitmap(font_param_t *font,int font_size)
{
	int row, col, i, count;
	int top, base;
	int frame_width;
	int fill_left, fill_right;
	unsigned long size = 0;

	FT_UInt glyph_index = FT_Get_Char_Index(face, font->charCode);
	if (glyph_index == 0)
	{
		glyph_index = 440; //字库中不存在的替换为?显示
	}
	FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
	//FT_Load_Glyph(face, FT_Get_Char_Index(face, font->charCode), FT_LOAD_DEFAULT);
	FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

	slot = face->glyph;

	if (slot->bitmap_top < 0)
	{
		top = (face->size->metrics.ascender >> 6) - slot->bitmap.rows;
	}
	else
	{
		top = (face->size->metrics.ascender >> 6) - slot->bitmap_top;
	}

	//top = (face->size->metrics.ascender >> 6) - slot->bitmap_top;
	base = font_size - top - slot->bitmap.rows;

	while ((base < 0) && (top > 0))
	{
		base ++;
		top --;
	}
	slot->bitmap.rows = slot->bitmap.rows < font_size ? slot->bitmap.rows : font_size;

#ifdef NIRVANA_DEBUG
	printf("\n\n_______Nirvana debug_________\n");
	printf("111111111111111111111\n");
	printf("is scaleable is:%d\n", FT_IS_SCALABLE(face));
	printf("the units_er_EM is:%d\n", face->units_per_EM);
	printf("the x_ppem is:%d\n", face->size->metrics.x_ppem);
	printf("the y_ppem is:%d\n", face->size->metrics.y_ppem);
	printf("the x_scale is:%d\n", face->size->metrics.x_scale>>6);
	printf("the y_scale is:%d\n", face->size->metrics.y_scale>>6);
	printf("the ascender is:%d\n", face->size->metrics.ascender>>6);
	printf("the deacender is:%d\n", face->size->metrics.descender>>6);
	printf("the height is:%d\n", face->size->metrics.height>>6);
	printf("the max_advance is:%d\n", face->size->metrics.max_advance>>6);
	printf("bitmap_left = %d\n", slot->bitmap_left);
	printf("bitmap_top  = %d\n", slot->bitmap_top);
	printf("rows = %d\n", slot->bitmap.rows);
	printf("cols = %d\n", slot->bitmap.width);
	printf("my_top  = %d\n", top);
	printf("my_base = %d\n", base);
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	printf("the row is         :%d\n", slot->bitmap.rows);
	printf("the width is       :%d\n", slot->bitmap.width);
	printf("the pitch is       :%d\n", slot->bitmap.pitch);
	printf("the num_grays is   :%d\n", slot->bitmap.num_grays);
	printf("the pixel_mode is  :%d\n", slot->bitmap.pixel_mode);
	printf("the palette_mode is:%d\n", slot->bitmap.palette_mode);
	printf("the palette is     :%d\n", slot->bitmap.palette);
//add by apostle////////////////////////////
	for (row = 0; row < slot->bitmap.rows; row++)
	{
		for (col = 0; col < slot->bitmap.width; col++)	
		{
			printf("%2x ", slot->bitmap.buffer[row * slot->bitmap.pitch + col]);		
		}
		printf("\n");
	}
/////////add end by apostle//////////////////////////// 
#endif

	char y_ft, y_bg;
	if (font->color == BLACK)
	{
		y_ft = 0x10;
		y_bg = 0xeb;
	}
	else if (font->color == WHITE)
	{
		y_ft = 0xeb;
		y_bg = 0x10;
	}

	//空格解析
	if ((slot->bitmap.rows == 0) || (slot->bitmap.width == 0))
	{
		slot->bitmap.width = font_size / 2;
	}

	if (slot->bitmap.width > (font_size / 2))
	{
		frame_width = font_size;
	}
	else
	{
		frame_width = font_size / 2;
	}
	size = font_size * frame_width;
	buf_y = (char *)malloc(size);
	buf_uv = (char *)malloc(size / 2);

	font->height = font_size;
	//font->width = slot->bitmap.width;
	font->width = frame_width;
	font->buf_y = buf_y;
	font->buf_uv = buf_uv;
	font->size = size;

	fill_left = (frame_width - slot->bitmap.width) / 2;
	fill_right = frame_width - slot->bitmap.width - fill_left;

	count = 0;
	for (row = 0; row < top; row++)
	{
		for (col = 0; col < frame_width; col++)
		{
			buf_y[count++] = y_bg;
#ifdef NIRVANA_DEBUG
			printf("_");
#endif
		}
#ifdef NIRVANA_DEBUG
		printf("\n");
#endif
	}

	for (row = 0; row < slot->bitmap.rows; row++)
	{
		for (col = 0; col < fill_left; col++)
		{
			buf_y[count++] = y_bg;
#ifdef NIRVANA_DEBUG
			printf("_");
#endif
		}
		for (col = 0; col < slot->bitmap.width; col++)
		{
			if((slot->bitmap.buffer[row * slot->bitmap.pitch + col]) < 0x80)
			{
				buf_y[count++] = y_bg;
#ifdef NIRVANA_DEBUG
				printf("_");
#endif
			}
			else
			{
				if(font->color == WHITE)
				{
					buf_y[count++] = slot->bitmap.buffer[row * slot->bitmap.pitch + col];
				}
				else
					buf_y[count++] = 256 - slot->bitmap.buffer[row * slot->bitmap.pitch + col];
				//buf_y[count++] = y_ft;
#ifdef NIRVANA_DEBUG
				printf("0");
#endif
			}
		}
		for (col = 0; col < fill_right; col++)
		{
			buf_y[count++] = y_bg;
#ifdef NIRVANA_DEBUG
			printf("_");
#endif
		}
#ifdef NIRVANA_DEBUG
		printf("\n");
#endif
	}

	for (row = 0; row < base; row++)
	{
		for (col = 0; col < frame_width; col++)
		{
			buf_y[count++] = y_bg;
#ifdef NIRVANA_DEBUG
			printf("_");
#endif
		}
#ifdef NIRVANA_DEBUG
		printf("\n");
#endif
	}


	count = 0;
	for (i = 0; i < (size / 2); i++)
	{
		buf_uv[count++] = 0x80;
	}

	
	return 0;
}
