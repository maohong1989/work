#ifndef _ASL_COMMOM_H_
#define _ASL_COMMOM_H_
// [目录]
// --行数--- [目录]
// --18----- 1 [typedef自定义基础类型]
// --39----- 2 [公共宏定义]
// --40-----   2.1 [COM_SUCCESS]
// --41-----   2.2 [COM_ERROR]
// --46----- 3 [公共debug]
// --49-----   3.1 [时间调试模块]
// --53----- 4 [自定义debug函数]
// --54-----   4.1 [COM_DEBUG_INFO]
// --55-----   4.2 [com_assert_info]
// --56-----   4.3 [COM_WARN_INFO]
// --96----- 5 [公共函数]
#include <stdio.h>
/***********************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

//=@_@=// typedef自定义基础类型
typedef signed char         CHAR;
typedef signed short        Sint16;
typedef signed int          Sint32;
typedef signed long         Slng32;
typedef signed long long    Sint64;
typedef unsigned char       UCHAR;
typedef unsigned short      Uint16;
typedef unsigned int        Uint32;
typedef unsigned long       Ulng32;
typedef unsigned long long  Uint64;

typedef unsigned char       BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef float               FLOAT;
typedef void                *HANDLE;
typedef void                *BUF_HANDLE;

/***********************************************************************/
//=@_@=// 公共宏定义
//=@__@=// COM_SUCCESS
//=@__@=// COM_ERROR
#define COM_SUCCESS             (0)
#define COM_ERROR               (-1)
#define COM_TIME_OUT            (0)
#define COM_TIME_WAIT           (1)

/***********************************************************************/
//=@_@=// 公共debug
#define ASL_COMMON_DEBUG

//=@__@=// 时间调试模块
#define ASL_TIME_DEBUG
#define ASL_BUF_DEBUG

/***********************************************************************/
//=@_@=// 自定义debug函数
//=@__@=// COM_DEBUG_INFO
//=@__@=// com_assert_info
//=@__@=// COM_WARN_INFO
#ifdef ASL_COMMON_DEBUG
#define COM_DEBUG_INFO(fmt,arg...) \
		do\
		{\
			fprintf(stdout, fmt, ##arg);\
			fprintf(stdout, "[FILE:%s, FUNCTION:%s, LINE:%d]\n\n",\
					 __FILE__, __FUNCTION__, __LINE__);\
		}while(0);
#define com_assert_info(x) \
		if((x) == 0){\
			do{\
			fprintf(stderr, "assert:the value unequal, press 'q' to continue\n");\
			fprintf(stderr, "[FILE:%s, FUNCTION:%s, LINE:%d]\n",\
					 __FILE__, __FUNCTION__, __LINE__);\
		}while(getchar()!='q');\
		}
#else
#define COM_DEBUG_INFO(fmt,arg...)
//#define COM_ERROR_INFO(fmt,arg...)
#define com_assert_info(x)
#endif /*end ASL_COMMON_DEBUG */

#define COM_WARN_INFO(fmt,arg...) \
		do\
		{\
			fprintf(stderr, "[***warnning***]:");\
			fprintf(stderr, fmt, ##arg);\
			fprintf(stderr, "[FILE:%s, FUNCTION:%s, LINE:%d]\n\n",\
					 __FILE__, __FUNCTION__, __LINE__);\
		}while(0);

#define COM_ERROR_INFO(fmt,arg...) \
		do\
		{\
			fprintf(stderr, fmt, ##arg);\
			fprintf(stderr, "[FILE:%s, FUNCTION:%s, LINE:%d]\n\n",\
					 __FILE__, __FUNCTION__, __LINE__);\
		}while(0);
/***********************************************************************/
//=@_@=// 公共函数
#define COM_VERFY_NULL(x) \
	if ((x) == 1){\
		COM_ERROR_INFO("the value is NULL!\n");\
		return COM_ERROR;\
	}
#define COM_VERFY_VALUE(x) \
	if ((x) != 1){\
		COM_ERROR_INFO("the queal is not true!\n");\
		return COM_ERROR;\
	}

#define COM_MEMCPY(des, src, count) {\
	Uint32 *p = (Uint32 *)des;\
	Uint32 *q = (Uint32 *)src;\
	Uint32 n  = (count >> 2);\
	while(n--)\
	{\
		*p++ = *q++;\
	}\
}
#define COM_ALIGN_UP(value, align) \
       (((value) & (align-1)) ? \
       (((value) + (align-1)) & ~(align-1)) : (value))

#define COM_ALIGN_DOWN(value, align) ((value) & ~(align-1))


/*
//new define
#define ERROR(ftm, arg...) fprintf(stdout, "video_cmd is:"arg"", ##arg)
*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*end _cplus*/

#endif /*end common*/
