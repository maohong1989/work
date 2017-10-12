////////////////////////////////////////////////////////////////////////////////////
//                        版权所有，作者保留一切权利                                  
// 在保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播。 
// 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途                  
////////////////////////////////////////////////////////////////////////////////////
// 文 件 名: ASLBufMng.h                                                           
// 作    者: apostle --- apostle9891@foxmail.com                                    
// 版    本: version 1.0                                                            
// 日    期: 2017-04-01 16:54                                                       
// 描    述:                                                                        
// 历史描述:                                                                      
////////////////////////////////////////////////////////////////////////////////////
#ifndef __ASL_BUF_MNG_H__
#define __ASL_BUF_MNG_H__
#include <ASLCommon.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

typedef struct _ASL_BUF_CFG_S
{
	Uint32 total_size;
	Uint32 block_size;
	Uint32 block_num;
}ASL_BUF_CFG_OBJ, *ASL_BUF_CFG_HANDLE;

typedef enum 
{
	BUF_TIME_OUT  = COM_TIME_OUT,
	BUF_TIME_WAIT = COM_TIME_WAIT
}BUF_STATUS_E;

Sint32 ASLBuf_Init(HANDLE *handle, ASL_BUF_CFG_OBJ buf_cfg);
Sint32 ASLBuf_Exit(HANDLE *handle);
Sint32 ASLBuf_GetEmpty(HANDLE handle, HANDLE *data, BUF_STATUS_E timeout);
Sint32 ASLBuf_PutEmpty(HANDLE handle, HANDLE data, BUF_STATUS_E timeout);
Sint32 ASLBuf_GetFull(HANDLE handle, HANDLE *data, BUF_STATUS_E timeout);
Sint32 ASLBuf_PutFull(HANDLE handle, HANDLE data, BUF_STATUS_E timeout);
Sint32 ASLBuf_EmptyToFull(HANDLE handle);
Sint32 ASLBuf_FullToEmpty(HANDLE handle);
Sint32 ASLBuf_ShowStatus(HANDLE handle);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*end _cplusplus*/



#endif /*end the ASL_BUF_MNG_H__*/
