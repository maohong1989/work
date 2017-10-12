#ifndef _ASL_TIME_H_
#define _ASL_TIME_H_
#include <stdio.h>
#include <ASL_common.h>

#define TIME_NUM 200
#ifdef ASL_TIME_DEBUG
typedef struct{
	Uint32 total_time;
	Uint32 max_time;
	Uint32 min_time;

	Uint32 count;
	Uint32 cur_time[TIME_NUM];
}ASL_TIME_HANDLE;
	
Sint32 ASL_time_begin(ASL_TIME_HANDLE *handle);
Sint32 ASL_time_end(ASL_TIME_HANDLE *handle);
Sint32 ASL_time_reset(ASL_TIME_HANDLE *handle);
Sint32 ASL_time_print(ASL_TIME_HANDLE *handle);
Uint32 ASL_get_cur_time_ms();
Uint32 ASL_get_cur_time_us();
#else
typedef struct{
	Uint32 rsv;
}ASL_TIME_HANDLE;
#define ASL_time_begin(handle)
#define ASL_time_reset(handle)
#define ASL_time_end(handle)
#define ASL_time_print(handle)
#endif
#endif
