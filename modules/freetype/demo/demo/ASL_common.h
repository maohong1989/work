#ifndef _ASL_COMMOM_H_
#define _ASL_COMMOM_H_
#include <stdio.h>
/***********************************************************************/
/********* my typedef*****************************************************/
typedef signed char         Sint8;
typedef signed short        Sint16;
typedef signed int          Sint32;
typedef signed long         Slng32;
typedef signed long long    Sint64;
typedef unsigned char       Uint8;
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


/********common define*************************************************/
#define SUCCESS             (0)
#define ERROR               (1)

/********debug define*************************************************/
//#define ASL_COMMON_DEBUG
#define ASL_TIME_DEBUG
/***********************************************************************/

/***********common function********************************************/
#define ASL_MEMCPY(des, src, count) {\
	Uint32 *p = (Uint32 *)des;\
	Uint32 *q = (Uint32 *)src;\
	Uint32 n  = (count >> 2);\
	while(n--)\
	{\
		*p++ = *q++;\
	}\
}

/***********debug function********************************************/
#ifdef ASL_COMMON_DEBUG
#define DEBUG_INFO(fmt,arg...) \
		do\
		{\
			fprintf(stdout, fmt, ##arg);\
			fprintf(stdout, "[FILE:%s, FUNCTION:%s, LINE:%d]\n\n",\
					 __FILE__, __FUNCTION__, __LINE__);\
		}while(0);
#define assert_info(x) \
		if((x) == 0){\
			fprintf(stderr, "assert:the value unequal, press 'q' to continue\n");\
			fprintf(stderr, "[FILE:%s, FUNCTION:%s, LINE:%d]\n",\
					 __FILE__, __FUNCTION__, __LINE__);\
		}while(getchar()!='q');
#else
#define DEBUG_INFO(fmt,arg...) 
//#define ERROR_INFO(fmt,arg...)
#define assert_info(x)
#endif

#define WARN_INFO(fmt,arg...) \
		do\
		{\
			fprintf(stderr, fmt, ##arg);\
			fprintf(stderr, "[FILE:%s, FUNCTION:%s, LINE:%d]\n\n",\
					 __FILE__, __FUNCTION__, __LINE__);\
		}while(0);

#define ERROR_INFO(fmt,arg...) \
		do\
		{\
			fprintf(stderr, fmt, ##arg);\
			fprintf(stderr, "[FILE:%s, FUNCTION:%s, LINE:%d]\n\n",\
					 __FILE__, __FUNCTION__, __LINE__);\
		}while(0);
#endif
