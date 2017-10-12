////////////////////////////////////////////////////////////////////////////////////
//                        版权所有，作者保留一切权利
// 在保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播。
// 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途
////////////////////////////////////////////////////////////////////////////////////
// 文 件 名: ASLList.h
// 作    者: apostle --- apostle9891@foxmail.com
// 版    本: version 1.0
// 日    期: 2017-04-02 23:44
// 描    述:
// 历史描述:
////////////////////////////////////////////////////////////////////////////////////
#ifdef __ASL_LIST_H_
#define __ASL_LIST_H_
#include <stdio.h>
#include <ASLCommon.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

typedef struct ASL_LIST_DATA_S
{
    Uint32 size;
    HANDLE data;
}NODE_DATA_OBJ, *NODE_DATA_HANDLE;

//在销毁节点的时候，如果回调函数有操作，会调用回调函数

typedef void  (*DESTROY_DATA)(HANDLE data);
typedef struct ASL_LIST_DATA_CFG_S
{
    Uin32 list_count;
    LIST_DATA_OBJ list_data;
    DESTROY_DATA destroy_function;
}LIST_DATA_CFG_OBJ, *LIST_DATA_CFG_HANDLE;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*end _cplusplus*/

#endif /*end ASL_LIST_H_*/

