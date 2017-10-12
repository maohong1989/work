////////////////////////////////////////////////////////////////////////////////////
//                        版权所有，作者保留一切权利                                  
// 在保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播。 
// 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途                  
////////////////////////////////////////////////////////////////////////////////////
// 文 件 名: ASLQueueMng.h                                                         
// 作    者: apostle --- apostle9891@foxmail.com                                    
// 版    本: version 1.0                                                            
// 日    期: 2017-04-01 11:49                                                       
// 描    述:                                                                        
// 历史描述:                                                                      
////////////////////////////////////////////////////////////////////////////////////
#ifndef __ASL_QUEUE_MNG_H__
#define __ASL_QUEUE_MNG_H__
#include <ASLCommon.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

typedef enum 
{
	QUEUE_TIME_OUT  = COM_TIME_OUT,
	QUEUE_TIME_WAIT = COM_TIME_WAIT
}QUEUE_STATUS_E;

/*
 数据data是由用户提供，请使用完毕后记得注销，以免发生内存泄露
eg：
HANDLE handle;
HANDLE *data;

//申请双指针
data = (HANDLE *)malloc(sizeof(data));
data[0] = malloc(12);

ASLQueue_Init(&handle, 2);
ASLQueue_Put(handle, data[0], QUEUE_TIME_WAIT);
ASLQueue_Get(handle, &data[1], QUEUE_TIME_WAIT);
ASLQueue_Exit(&handle);

 */
Sint32 ASLQueue_Init(HANDLE *handle, Uint32 MaxLen);
Sint32 ASLQueue_Exit(HANDLE *handle);
Sint32 ASLQueue_Put(HANDLE handle, HANDLE data, QUEUE_STATUS_E timeout);
Sint32 ASLQueue_Get(HANDLE handle, HANDLE *data, QUEUE_STATUS_E timeout);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*end _cplusplus*/

#endif /*end ASL_QUEUE__MNG_H__*/
