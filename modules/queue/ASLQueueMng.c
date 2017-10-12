////////////////////////////////////////////////////////////////////////////////////
//                        版权所有，作者保留一切权利
// 在保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播。
// 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途
////////////////////////////////////////////////////////////////////////////////////
// 文 件 名: ASLQueueMng.c
// 作    者: apostle --- apostle9891@foxmail.com
// 版    本: version 1.0
// 日    期: 2017-04-01 11:49
// 描    述: 本程序作为保护队列，主要在线程间使用,一般是作为读写管理，配合buf管理，
//           效果更好
// 历史描述:
////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ASLCommon.h>
#include <ASLQueueMng.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


typedef struct __ctags_ASL_QUEUE_MNG_S
{
	Uint32 CurRd;
	Uint32 CurWr;
	Uint32 Len;
	Uint32 Count;

	HANDLE *Queue;
	pthread_mutex_t Lock;
	pthread_cond_t CondRd;
	pthread_cond_t CondWr;
}ASL_QUEUE_OBJ, *ASL_QUEUE_HANDLE;



////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLQueue_Init
// 功    能：模块初始化
// 参    数：[IN] handle  -- 模块句柄
//           [IN] MaxLen  -- 结构体长度
// 返 回 值: COM_SUCCESS  -- 成功
//           COM_ERROR    -- 失败
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLQueue_Init(HANDLE *handle, Uint32 MaxLen)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(MaxLen != 0);
#endif

	ASL_QUEUE_HANDLE queue_handle;
	pthread_mutexattr_t mutex_attr;
	pthread_condattr_t cond_attr;
	Sint32 status = COM_SUCCESS;

	queue_handle = malloc(sizeof(ASL_QUEUE_OBJ));
	COM_VERFY_NULL(NULL == queue_handle);

	queue_handle->CurRd = 0;
	queue_handle->CurWr = 0;
	queue_handle->Count = 0;
	queue_handle->Len   = MaxLen;
	queue_handle->Queue = (void **)malloc(sizeof(queue_handle->Queue) * MaxLen);;
	COM_VERFY_NULL(NULL == queue_handle->Queue);

	status |= pthread_mutexattr_init(&mutex_attr);
	status |= pthread_condattr_init(&cond_attr);

	status |= pthread_mutex_init(&queue_handle->Lock, &mutex_attr);
	status |= pthread_cond_init(&queue_handle->CondRd, &cond_attr);
	status |= pthread_cond_init(&queue_handle->CondWr, &cond_attr);

	if (status != COM_SUCCESS)
	{
		COM_ERROR_INFO("the pthread mutex error!\n");
		return COM_ERROR;
	}

	pthread_condattr_destroy(&cond_attr);
	pthread_mutexattr_destroy(&mutex_attr);

	*handle = queue_handle;
	return COM_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLQueue_Exit
// 功    能：模块退出
// 参    数：[IN] handle  -- 模块句柄
// 返 回 值: COM_SUCCESS  -- 成功
//           COM_ERROR    -- 失败
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLQueue_Exit(HANDLE *handle)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(*handle != NULL);
#endif
	COM_VERFY_NULL(*handle == NULL);

	ASL_QUEUE_HANDLE queue_handle = *handle;
	Uint32 status                 = COM_SUCCESS;

	if (queue_handle->Queue != NULL)
		free(queue_handle->Queue);

  pthread_cond_destroy(&queue_handle->CondRd);
  pthread_cond_destroy(&queue_handle->CondWr);
  pthread_mutex_destroy(&queue_handle->Lock);

  free(queue_handle);

  *handle = NULL;
  return COM_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLQueue_Put
// 功    能：将数据放入队列
// 参    数：[IN] handle  -- 模块句柄
//           [IN] data    -- 放入队列的数据
//           [IN] timeout -- QUEUE_STATUS_E
// 返 回 值: COM_SUCCESS  -- 成功
//           COM_ERROR    -- 失败
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLQueue_Put(HANDLE handle, HANDLE data, QUEUE_STATUS_E timeout)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
	com_assert_info(data   != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);
	COM_VERFY_NULL(NULL == data);

	Uint32 status = 0;

	ASL_QUEUE_HANDLE queue_handle = handle;
	pthread_mutex_lock(&queue_handle->Lock);

	while(1)
	{
		if (queue_handle->Count < queue_handle->Len)
		{
			queue_handle->Queue[queue_handle->CurWr] = data;
			queue_handle->CurWr  = (queue_handle->CurWr + 1) % queue_handle->Len;
			queue_handle->Count++;
			status = COM_SUCCESS;
			pthread_cond_signal(&queue_handle->CondRd);
			break;
		}
		else
		{
			if (timeout == QUEUE_TIME_OUT)
			{
				break;
			}
			else
			{
				COM_WARN_INFO("the quque is full, please wait the condwr!\n");
				status = pthread_cond_wait(&queue_handle->CondWr, &queue_handle->Lock);
			}

		}
	}

	pthread_mutex_unlock(&queue_handle->Lock);

	return status;
}

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLQueue_Get
// 功    能：获取队列的数据
// 参    数：[IN] handle  -- 模块句柄
//           [IN] data    -- 获取的参数
//           [IN] timeout -- QUEUE_STATUS_E
// 返 回 值: COM_SUCCESS  -- 成功
//           COM_ERROR    -- 失败
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLQueue_Get(HANDLE handle, HANDLE *data, QUEUE_STATUS_E timeout)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
	com_assert_info(data   != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);
	COM_VERFY_NULL(NULL == data);

	Uint32 status = 0;

	ASL_QUEUE_HANDLE queue_handle = handle;
	pthread_mutex_lock(&queue_handle->Lock);
	while(1)
	{
		if (queue_handle->Count > 0)
		{
			*data = queue_handle->Queue[queue_handle->CurRd];
			queue_handle->CurRd = (queue_handle->CurRd + 1) % queue_handle->Len;
			queue_handle->Count--;
			status = COM_SUCCESS;
			pthread_cond_signal(&queue_handle->CondWr);
			break;

		}
		else
		{
			if (timeout == QUEUE_TIME_OUT)
			{
				break;
			}
			else
			{
				status = pthread_cond_wait(&queue_handle->CondRd, &queue_handle->Lock);
			}

		}
	}
	pthread_mutex_unlock(&queue_handle->Lock);
	return status;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
