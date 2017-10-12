/*************************************************************************
 * author  :mark
 * time    :2013.05.27
 * function:use freetype
 * **********************************************************************/
#include <stdio.h>
#include <stdlib.h>//mbstwcs
#include <locale.h>
#include <FT_osd_cn.h>
#include <FT_osd_cn_public.h>
#include <apostle_common.h>
#include <ft2build.h>
#include <wchar.h>
#include <string.h>

#include FT_FREETYPE_H
FT_Library   library;
FT_Face      face;

#define ASL_COMMON_DEBUG
FT_OSD_HANDLE FT_OSD_handle;

/*************************************************************************
 * 
 * warning:1. if the default path is chinese, mybe error
 * **********************************************************************/
Sint32 ft_osd_open(OSD_HANDLE *handle)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	FT_OSD_HANDLE *p_handle = &FT_OSD_handle;
	if (NULL == handle)
	{
		ERROR_INFO("the freetype init handle is NULL!\n");
		return OSD_ERROR;
	}
	memset((void *)p_handle, 0, sizeof(FT_OSD_HANDLE));
	p_handle->font_type              = DEFAULT_FONT_TYPE;
	p_handle->yuv_format             = DEFAULT_YUV_FORMAT;
	p_handle->font_width             = DEFAULT_FONT_WIDTH;
	p_handle->font_height            = DEFAULT_FONT_HEIGHT;
	p_handle->total_width            = DEFAULT_TOTAL_WIDTH;
	p_handle->total_height           = DEFAULT_TOTAL_HEIGHT;
	p_handle->font_color.color       = DEFAULT_FONT_COLOR;
	p_handle->font_color.y           = DEFAULT_FONT_Y;
	p_handle->font_color.uv          = DEFAULT_FONT_UV;
	p_handle->background_color.color = DEFAULT_BACKGROUND_COLOR;
	p_handle->background_color.y     = DEFAULT_BACKGROUND_Y;
	p_handle->background_color.uv    = DEFAULT_BACKGROUND_UV;

	p_handle->set_locale_fun         = setlocale;

	memset(p_handle->font_path, 0, DEFAULT_PATH_LEN);

	/********warning if it is chinese path, maybe error**********************/
	memcpy(p_handle->font_path, DEFAULT_FONT_PATH, strlen(DEFAULT_FONT_PATH));
	/**********************************************************************/
	memset(p_handle->cn_font_encode_type, 0, sizeof(p_handle->cn_font_encode_type));
	memcpy(p_handle->cn_font_encode_type, DEFAULT_FONT_ENCODE, strlen(DEFAULT_FONT_ENCODE));

	*handle = (OSD_HANDLE)p_handle;

	DEBUG_INFO("the ft_osd_open exit success!\n");
	return OSD_SUCCESS;
}

/*************************************************************************
 * 
 * warning:1. 
 * **********************************************************************/
Sint32 ft_osd_set_config(OSD_HANDLE handle, FT_OSD_CONFIG config)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif

	FT_OSD_HANDLE *p_handle = (FT_OSD_HANDLE *)handle;
	if (handle == NULL)
	{
		ERROR_INFO("the ft_osd_set_config error!\n");	
		return OSD_ERROR;
	}


	if (config.font_type < DEFAULT_VALUE || config.font_type >= END_FONT)
	{
		ERROR_INFO("the ft_osd_set_config font_type is invalid! [%d]\n", config.font_type);	
		return OSD_ERROR;
	}

	if (config.yuv_format < DEFAULT_VALUE || config.yuv_format >= YUV_FORMAT_END)
	{
		ERROR_INFO("the ft_osd_set_config yuv_format is invalid! [%d]\n", config.yuv_format);	
		return OSD_ERROR;
	}
	if (config.font_width < DEFAULT_VALUE)
	{
		ERROR_INFO("the ft_osd_set_config font_width is invalid! [%d]\n", config.font_width);	
		return OSD_ERROR; } if (config.font_height < DEFAULT_VALUE)
	{
		ERROR_INFO("the ft_osd_set_config font_height is invalid! [%d]\n", config.font_height);	
		return OSD_ERROR;
	}
	if (config.font_color.color < DEFAULT_VALUE)
	{
		ERROR_INFO("the ft_osd_set_config font_color is invalid! [%d]\n", config.font_color.color);
		return OSD_ERROR;
	}
	if (config.background_color.color < DEFAULT_VALUE)
	{
		ERROR_INFO("the ft_osd_set_config background_color is invalid! [%d]\n", config.background_color.color);
		return OSD_ERROR;
	}

	if (config.font_path == NULL)
	{
		WARN_INFO("the ft_osd_set_config font_path USE_DEFAULT_PATH!\n");	
	}

	if (DEFAULT_VALUE != config.font_type)
	{
		p_handle->font_type = config.font_type;
	}

	if (DEFAULT_VALUE != config.yuv_format)
	{
		p_handle->yuv_format = config.yuv_format;	
	}
	if (DEFAULT_VALUE != config.font_width)
	{
		p_handle->font_width = config.font_width;	
	}

	if (DEFAULT_VALUE != config.font_height)
	{
		p_handle->font_height = config.font_height;	
	}
	if (DEFAULT_VALUE != config.font_color.color)
	{
		if (FT_OSD_WHITE == config.font_color.color)				
		{
			p_handle->font_color.color = FT_OSD_WHITE;
			p_handle->font_color.y     = FT_OSD_WHITE_Y;
			p_handle->font_color.uv    = FT_OSD_WHITE_UV;
		}
		else if (FT_OSD_BLACK == config.font_color.color)				
		{
			p_handle->font_color.color = FT_OSD_BLACK;
			p_handle->font_color.y     = FT_OSD_BLACK_Y;
			p_handle->font_color.uv    = FT_OSD_BLACK_UV;
		}
		else if (FT_OSD_USER_DEFINE == config.font_color.color)				
		{
			p_handle->font_color.color = FT_OSD_USER_DEFINE;
			p_handle->font_color.y     = config.font_color.y;
			p_handle->font_color.uv    = config.font_color.uv;
		}
	}
	if (DEFAULT_VALUE != config.background_color.color)
	{
		if (FT_OSD_WHITE == config.background_color.color)				
		{
			p_handle->background_color.color = FT_OSD_WHITE;
			p_handle->background_color.y     = FT_OSD_WHITE_Y;
			p_handle->background_color.uv    = FT_OSD_WHITE_UV;
		}
		else if (FT_OSD_BLACK == config.background_color.color)				
		{
			p_handle->background_color.color = FT_OSD_BLACK;
			p_handle->background_color.y     = FT_OSD_BLACK_Y;
			p_handle->background_color.uv    = FT_OSD_BLACK_UV;
		}
		else if (FT_OSD_USER_DEFINE == config.background_color.color)				
		{
			p_handle->background_color.color = FT_OSD_USER_DEFINE;
			p_handle->background_color.y     = config.font_color.y;
			p_handle->background_color.uv    = config.font_color.uv;
		}
	}

	if (NULL != config.font_path)
	{
		memset(p_handle->font_path, 0, sizeof(p_handle->font_path));
		memcpy(p_handle->font_path, config.font_path, strlen(config.font_path));
	}
	
	DEBUG_INFO("exit ft_set_config !!");
	return OSD_SUCCESS;
}

Sint32  ft_osd_start(OSD_HANDLE handle)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	
	if (NULL == handle) { ERROR_INFO("the handle is NULL!\n");		
		return OSD_ERROR;
	}
	FT_OSD_HANDLE *p_handle = (FT_OSD_HANDLE *)handle;
	FT_Error error = FT_Init_FreeType(&library);	
	if (error)
	{
		ERROR_INFO("the ft_osd_start Freetype library init fail!\n");	
		return OSD_ERROR;
	}

	//set locale is cn, mbstowcs get the w_char
	p_handle->set_locale_fun(LC_ALL, p_handle->cn_font_encode_type);

	error = FT_New_Face(library, p_handle->font_path, 0, &face);
	if (FT_Err_Unknown_File_Format == error)
	{
		ERROR_INFO("ft_osd_start Font format not supported!\n");	
		return OSD_ERROR;
	}
	else if (error)
	{
		ERROR_INFO("ft_osd_start can't open font file, it is invalid!\n");	
		return OSD_ERROR;
	}

	error = FT_Set_Pixel_Sizes(face, p_handle->font_width, p_handle->font_height);
	if (error)
	{
		ERROR_INFO("ft_osd_start FT_Set_Pixel_Size failed!\n");
		return OSD_ERROR;
	}

	DEBUG_INFO("ft_osd_start exit success!\n");
	return OSD_SUCCESS;
}

Sint32 ft_osd_ioctl(OSD_HANDLE handle, Sint32 cmd, void *p_input, void *p_output)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	if (NULL == handle)
	{
		ERROR_INFO("the handle is NULL!\n");		
		return OSD_ERROR;
	}

	if (NULL == p_input)
	{
		ERROR_INFO("the p_input is NULL!\n");	
		return OSD_ERROR;
	}

	switch (cmd)
	{
		case FT_OSD_GET_YUV:
		{
						
		}
	}
}

static Sint32 ft_osd_get_single_yuv_data(OSD_HANDLE handle, wchar_t *w_char)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	if (NULL == handle)
	{
		ERROR_INFO("the handle is NULL!\n");		
		return OSD_ERROR;
	}

	if (NULL == w_char)
	{
		ERROR_INFO("the string is NULL!\n");	
		return OSD_ERROR;
	}

	FT_UInt glyph_index = FT_Get_Char_Index(face, *w_char);
	FT_GlyphSlot slot;

	if (glyph_index == 0)
	{
		glyph_index = 440; //字库中不存在的替换为?显示
	}
	FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT); //load face
	FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);//render the face

	slot = face->glyph;

	if (slot->bitmap_top < 0) //char "_"
	{
		top = (face->size->metrics.ascender >> 6) - slot->bitmap.rows;
	}
	else
	{
		top = (face->size->metrics.ascender >> 6) - slot->bitmap_top;
	}
	base = font_size - top - slot->bitmap.rows;


}

int main()
{
	OSD_HANDLE osd_handle;
	ft_osd_open(&osd_handle);	
	ft_osd_start(osd_handle);
	return 0;
}

