#ifndef _ASL_TIME_H_
#define _ASL_TIME_H_
#include <stdio.h>
#include <ASLCommon.h>

#define TIME_NUM     20
#define MARKNAME_LEN 30

#ifdef ASL_TIME_DEBUG
typedef struct{
	CHAR   mark_name[MARKNAME_LEN];
	Uint32 total_time;
	Uint32 max_time;
	Uint32 min_time;

	Uint32 count;
	Uint32 cur_time[TIME_NUM];
}ASL_TIME_HANDLE;

/*
********************demo for ASL_time***********************
	ASL_TIME_HANDLE handle_time;
	memset(&handle_time, 0, sizeof(ASL_TIME_HANDLE));
	ASLTime_SetMarkName(&handle_time, __FUNCTION__);
	ASLTime_Begin(&handle_time);
	ASLTime_End(&handle_time);
	ASLTime_Print(&handle_time);
************************************************************
*/
Sint32 ASLTime_Begin(ASL_TIME_HANDLE *handle);
Sint32 ASLTime_End(ASL_TIME_HANDLE *handle);
Sint32 ASLTime_Reset(ASL_TIME_HANDLE *handle);
Sint32 ASLTime_Print(ASL_TIME_HANDLE *handle);
Sint32 ASLTime_SetMarkName(ASL_TIME_HANDLE *handle, CHAR *mark_name);
#else
typedef struct{
	Uint32 rsv;
}ASL_TIME_HANDLE;
#define ASLTime_Begin(handle)
#define ASLTime_Reset(handle)
#define ASLTime_End(handle)
#define ASLTime_Print(handle)
#define ASLTime_SetMarkName(handle, mark_name)
#endif
#endif
