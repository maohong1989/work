////////////////////////////////////////////////////////////////////////////////////
//                        版权所有，作者保留一切权利
// 在保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播。
// 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途
////////////////////////////////////////////////////////////////////////////////////
// 文 件 名: ASLTimeMng.h
// 作    者: apostle --- apostle9891@foxmail.com
// 版    本: version 1.0
// 日    期: 2014-07-15 21:23
// 描    述: 测量程序运行时间
// 历史描述:
////////////////////////////////////////////////////////////////////////////////////
#ifndef _ASL_TIME_H_
#define _ASL_TIME_H_
#include <stdio.h>
#include <ASLCommon.h>

#ifdef ASL_TIME_DEBUG

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

typedef enum TIME_EXIT_OR_NOT_EXIT_E{
	TIME_NOT_EXIT = 0,
	TIME_EXIT     = 1
}EXIT_OR_NOT_EXIT_E;

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
Sint32 ASLTime_Init(HANDLE *handle, CHAR *mark_name);
Sint32 ASLTime_Begin(HANDLE handle);
Sint32 ASLTime_End(HANDLE handle);
Sint32 ASLTime_Print(HANDLE handle);
Sint32 ASLTime_Exit(HANDLE handle);
#else

#define ASLTime_Init(handle, mark_name)
#define ASLTime_Begin(handle)
#define ASLTime_End(handle)
#define ASLTime_Print(handle)
#define ASLTime_Exit(handle)

#endif /*end ASL_TIME_DEBUG*/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*end _cplusplus*/

#endif /*end ASL_TIME_H_*/

