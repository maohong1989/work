#include <stdio.h>
#include <ASL_time.h>
#include <locale.h>
#include <string.h>
#include <ft2build.h>
#include <wchar.h>
#include <string.h>
#include FT_FREETYPE_H
static FT_Library library;
static FT_Face face;
static FT_GlyphSlot slot;
#define NIRVANA_DEBUG
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

int main(void)
{
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
	error = FT_Set_Pixel_Sizes(face, 0, 128);
	if (error)
	{
		printf("FT_Set_Pixel_Size failed!\n");
		return -1;
	}

	wchar_t char_code[] = {L"毛"};
//	int len = wcslen(char_code);

	return 0;

}
int show_bitmap(font_param_t *font, int font_size)
{
	int row, col, i, count;
	int top, base;
	int frame_width;
	int fill_left, fill_right;
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


/*

int main(int argc, char **argv)
{
	ASL_TIME_HANDLE time_handle;
	char buf[20] = {0};
	int i = 100;
	setlocale(LC_ALL, "zh_CN.UTF-8");
	if (argc > 2)
	memcpy(buf, argv[1], strlen(argv[1]));
	else
		memset(buf, 1, 20);

	ASL_time_reset(&time_handle);
	while(i-- > 0)
	{
		ASL_time_begin(&time_handle);
		osd_cn_main(buf, 0, 32);
		yuv_free();
		ASL_time_end(&time_handle);
	}
	ASL_time_print(&time_handle);
	return 0;

}

*/
