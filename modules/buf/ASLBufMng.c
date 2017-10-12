////////////////////////////////////////////////////////////////////////////////////
//                        版权所有，作者保留一切权利
// 在保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播。
// 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途
////////////////////////////////////////////////////////////////////////////////////
// 文 件 名: ASLBufMng.c
// 作    者: apostle --- apostle9891@foxmail.com
// 版    本: version 1.0
// 日    期: 2017-04-01 16:54
// 描    述: 1 本程序主要是通过full和empty队列去管理buffer,可以使用的场景在多个线程同时
//             取buffer数据进行读写，在本案例中，只是使用单路读写操作
// 历史描述: 20170331 本程序主要是通过full和empty队列去管理buffer,可以使用的场景在多个
//                    线程同时取buffer数据进行读写，在本案例中，只是使用单路读写操作
//           20170405 更改了代码功能，将此模块功能更纯粹，只用作buf的数据管理，更多的
//                    读写操作留到上层应用去
////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <ASLBufMng.h>
#include <ASLQueueMng.h>
#include <string.h>
#include <pthread.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif
// typedef enum
// {
// 	BLOCK_END = 0,
// 	BLOCK_NOT_END = 1
// }BLOCK_STATUS_E;
// 
// typedef struct _ASL_BUF_HEAD_S
// {
// 	HANDLE         buf;
// 	Uint32         block_serial;
// 	Uint32         block_use_size;
// 	BLOCK_STATUS_E block_status;
// }BLOCK_OBJ, *BLOCK_HANDLE;

typedef struct _ASL_BUF_MNG_S
{
	Uint32 total_size;
	Uint32 block_size;
	Uint32 block_num;
	Uint32 block_use;
	Uint32 block_free;
	HANDLE empty_buf_handle;
	HANDLE full_buf_handle;
	HANDLE *data;

}ASL_BUF_OBJ, *ASL_BUF_HANDLE;

#ifdef ASL_BUF_DEBUG
Sint32 ASLBuf_ShowStatus(HANDLE handle)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
#endif
    COM_VERFY_NULL(handle == NULL);
	ASL_BUF_HANDLE buf_handle = handle;

    static Uint32 prev_block_use  = 0;
    static Uint32 prev_block_free = 0;
    if ((buf_handle->block_use != prev_block_use) ||( buf_handle->block_free != prev_block_free))
    {
        prev_block_free = buf_handle->block_free;
        prev_block_use  = buf_handle->block_use;
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        printf("apostle>>>>buf_handle->total_size                   = [%d]\n", buf_handle->total_size);
        printf("apostle>>>>buf_handle->block_size                   = [%d]\n", buf_handle->block_size);
        printf("apostle>>>>buf_handle->block_num                    = [%d]\n", buf_handle->block_num);
        printf("apostle>>>>buf_handle->block_use                    = [%d]\n", buf_handle->block_use);
        printf("apostle>>>>buf_handle->block_free                   = [%d]\n", buf_handle->block_free);
        printf("**********************************************\n");
    }
//     printf("apostle>>>>buf_handle->block_handle->block_serial   = [%d]\n", buf_handle->block_handle->block_serial);
//     printf("apostle>>>>buf_handle->block_handle->block_use_size = [%d]\n", buf_handle->block_handle->block_use_size);
    return 0;
}
#else
Sint32 ASLBuf_ShowStatus(HANDLE handle)
{
}
#endif
////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLBuf_init
// 功    能：模块初始化
// 参    数：[IN] handle  -- 模块句柄
//           [IN] buf_cfg -- ASL_BUF_CFG_OBJ
// 返 回 值: VIDEO_OK     -- 成功
//           VIDEO_ERROR  -- 失败
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLBuf_Init(HANDLE *handle, ASL_BUF_CFG_OBJ buf_cfg)
{
	ASL_BUF_HANDLE buf_handle;
	Sint32 status = COM_SUCCESS;
	Sint32 i;
	Uint32 offset;

	COM_VERFY_VALUE(0 != buf_cfg.total_size);
	COM_VERFY_VALUE(0 != buf_cfg.block_size);
	COM_VERFY_VALUE(0 != buf_cfg.block_num);
	if (buf_cfg.total_size != buf_cfg.block_num * buf_cfg.block_size)
	{
		COM_ERROR_INFO("init the buffer error, the total_size not equal block_size * block_num\n");
		return COM_ERROR;
	}

	buf_handle = malloc(sizeof(ASL_BUF_OBJ));
	COM_VERFY_NULL(NULL == buf_handle);
	memset(buf_handle, 0, sizeof(ASL_BUF_OBJ));


	buf_handle->total_size = buf_cfg.total_size;
	buf_handle->block_size = buf_cfg.block_size;
	buf_handle->block_num  = buf_cfg.block_num;
	buf_handle->block_use  = 0;
	buf_handle->block_free = 0;
	buf_handle->data       = malloc(sizeof(HANDLE *) * buf_handle->block_num);

	if(NULL == buf_handle->data)
	{
		COM_ERROR_INFO("get the block handle is NULL !\n");
		goto END_FREE_BUF;
	}
	memset(buf_handle->data, 0, sizeof(HANDLE *) * buf_handle->block_num);

	status |= ASLQueue_Init(&(buf_handle->empty_buf_handle), buf_handle->block_num);
	status |= ASLQueue_Init(&(buf_handle->full_buf_handle), buf_handle->block_num);

	if (COM_SUCCESS != status)
	{
		COM_ERROR_INFO("init the empty or full handle error!\n");
		goto END_FREE_BLOCK_HEAD;
	}

	for (i = 0; i < buf_handle->block_num; i++)
	{
		buf_handle->data[i] = malloc(buf_handle->block_size);
		if (buf_handle->data[i] == NULL)
			goto END_FREE_BLOCK;

		ASLQueue_Put(buf_handle->empty_buf_handle, buf_handle->data[i], QUEUE_TIME_WAIT);
		buf_handle->block_free = buf_handle->block_free + 1;
	}

	*handle = buf_handle;
	return COM_SUCCESS;


END_FREE_BLOCK:
	for (i = 0; i < buf_handle->block_num; i++)
	{
		if (NULL == buf_handle->data[i]) {
			free(buf_handle->data[i]);
			buf_handle->data[i] = NULL;
		}
	}

END_FREE_BLOCK_HEAD:
	free(buf_handle->data);
	buf_handle->data = NULL;

END_FREE_BUF:
	free(buf_handle);
	buf_handle = NULL;

	return COM_ERROR;
}
////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLBuf_Exit
// 功    能：模块退出
// 参    数：[IN] handle  -- 模块句柄
// 返 回 值: VIDEO_OK     -- 成功
//           VIDEO_ERROR  -- 失败
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLBuf_Exit(HANDLE *handle)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(*handle != NULL);
#endif
	ASL_BUF_HANDLE buf_handle = *handle;

	COM_VERFY_NULL(*handle == NULL);
	COM_VERFY_NULL(NULL == buf_handle->empty_buf_handle);
	COM_VERFY_NULL(NULL == buf_handle->full_buf_handle);
	COM_VERFY_NULL(NULL == buf_handle->data);

	Sint32 status = COM_SUCCESS;
	Sint32 i;


	status |= ASLQueue_Exit(&buf_handle->empty_buf_handle);
	status |= ASLQueue_Exit(&buf_handle->full_buf_handle);
	if (COM_SUCCESS != status)
	{
		COM_ERROR_INFO("exit the empty or full handle error!\n");
		return COM_ERROR;
	}

	for (i = 0; i < buf_handle->block_num; i++)
	{
		if (NULL == buf_handle->data[i]) {
			free(buf_handle->data[i]);
			buf_handle->data[i] = NULL;
		}
	}
	free(buf_handle->data);
	free(buf_handle);

	*handle = NULL;
	return COM_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLBuf_GetEmpty
// 功    能：获取参数
// 参    数：[IN] handle  -- 模块句柄
//           [IN] data    -- xxxx
//           [IN] timeout -- BUF_STATUS_E
// 返 回 值: VIDEO_OK     -- 成功
//           VIDEO_ERROR  -- 失败
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLBuf_GetEmpty(HANDLE handle, HANDLE *data, BUF_STATUS_E timeout)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(handle == NULL);

	ASL_BUF_HANDLE buf_handle = handle;
	Sint32 status = COM_SUCCESS;

	status |= ASLQueue_Get(buf_handle->empty_buf_handle, data, timeout);
	if(status != COM_SUCCESS)
	{
		*data = NULL;
		COM_ERROR_INFO("get the empty buf_handle error!!\n");
		COM_ERROR_INFO("maybe the buf is full, please check it\n");
		return COM_ERROR;
	}
	buf_handle->block_free--;
	return COM_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLBuf_PutEmpty
// 功    能：xxxx
// 参    数：[IN] handle  -- 模块句柄
//           [IN] data    -- xxxx
//           [IN] timeout -- BUF_STATUS_E
// 返 回 值: VIDEO_OK     -- 成功
//           VIDEO_ERROR  -- 失败
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLBuf_PutEmpty(HANDLE handle, HANDLE data, BUF_STATUS_E timeout)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
	com_assert_info(data != NULL);
#endif
	COM_VERFY_NULL(handle == NULL);
	COM_VERFY_NULL(data   == NULL);

	ASL_BUF_HANDLE buf_handle = handle;
	Sint32 status = COM_SUCCESS;

	status |= ASLQueue_Put(buf_handle->empty_buf_handle, data, timeout);
	if(status != COM_SUCCESS)
	{
		COM_ERROR_INFO("put the empty buf_handle error!!\n");
		return COM_ERROR;
	}

	buf_handle->block_free++;
	return COM_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLBuf_GetFull
// 功    能：获取参数
// 参    数：[IN] handle  -- 模块句柄
//           [IN] data    -- xxxx
//           [IN] timeout -- BUF_STATUS_E
// 返 回 值: VIDEO_OK     -- 成功
//           VIDEO_ERROR  -- 失败
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLBuf_GetFull(HANDLE handle, HANDLE *data, BUF_STATUS_E timeout)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(handle == NULL);

	ASL_BUF_HANDLE buf_handle = handle;
	Sint32 status = COM_SUCCESS;

	status |= ASLQueue_Get(buf_handle->full_buf_handle, data, timeout);
	if(status != COM_SUCCESS)
	{
		*data = NULL;
		COM_ERROR_INFO("get the full buf_handle error!!\n");
		COM_ERROR_INFO("maybe the buf is empty, please check it\n");
		return COM_ERROR;
	}
	buf_handle->block_use--;
	return COM_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLBuf_PutFull
// 功    能：xxxx
// 参    数：[IN] handle  -- 模块句柄
//           [IN] data    -- xxxx
//           [IN] timeout -- BUF_STATUS_E
// 返 回 值: VIDEO_OK     -- 成功
//           VIDEO_ERROR  -- 失败
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLBuf_PutFull(HANDLE handle, HANDLE data, BUF_STATUS_E timeout)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
	com_assert_info(data != NULL);
#endif
	COM_VERFY_NULL(handle == NULL);
	COM_VERFY_NULL(data   == NULL);

	ASL_BUF_HANDLE buf_handle = handle;
	Sint32 status = COM_SUCCESS;

	status |= ASLQueue_Put(buf_handle->full_buf_handle, data, timeout);
	if(status != COM_SUCCESS)
	{
		COM_ERROR_INFO("put the full buf_handle error!!\n");
		return COM_ERROR;
	}
	buf_handle->block_use++;
	return COM_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLBuf_EmptyToFull
// 功    能：将空的缓冲区导入到写满中去
// 参    数：[IN] handle -- 模块句柄
// 返 回 值: VIDEO_OK    -- 成功
//           VIDEO_ERROR -- 失败
// 说    明: 1 此函数在这里用处不大，如果转换,会发生数据紊乱,出现重复数据，先写上，以后
//             可能有用处
//           2 可以将此buf装满
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLBuf_EmptyToFull(HANDLE handle)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(handle == NULL);

	ASL_BUF_HANDLE buf_handle = handle;
	Sint32 status = COM_SUCCESS;
	HANDLE new_data;

	status = ASLQueue_Get(buf_handle->empty_buf_handle, &new_data, QUEUE_TIME_OUT);
	if (status == COM_SUCCESS)
	{
		status = ASLQueue_Put(buf_handle->full_buf_handle, new_data, QUEUE_TIME_OUT);
	}
	return status;
}
////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLBuf_FullToEmpty
// 功    能：将buf中写满的数据清空
// 参    数：[IN] handle -- 模块句柄
// 返 回 值: VIDEO_OK    -- 成功
//           VIDEO_ERROR -- 失败
// 说    明: 1 此函数在这里用处不大，如果转换,会发生数据紊乱,出现重复数据，先写上，以后
//             可能有用处
//           2 可以将此buf清空
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLBuf_FullToEmpty(HANDLE handle)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(handle == NULL);

	ASL_BUF_HANDLE buf_handle = handle;
	Sint32 status = COM_SUCCESS;
	HANDLE new_data;

	status = ASLQueue_Get(buf_handle->full_buf_handle, &new_data, QUEUE_TIME_OUT);
	if (status == COM_SUCCESS)
	{
		status = ASLQueue_Put(buf_handle->empty_buf_handle, new_data, QUEUE_TIME_OUT);
	}
	return status;
}


#if 0
////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLBuf_WriteData 
// 功    能：写入数据
// 参    数：[IN] handle -- 模块句柄
//           [IN] data   -- 写入的数据，注意是一个指针
//           [IN] size   -- 写入数据的大小
// 返 回 值: COM_SUCCESS -- 成功
//           COM_ERROR   -- 失败
// 说    明: 1 由于实际项目中是用于单路读写操作，为避免过度设计，没有增加读写锁。
//          　 如果在多线程读写中，需增加读写锁，在超大帧下会发生读写的block不连续的情况
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLBuf_WriteData(HANDLE handle, HANDLE data, Uint32 size)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
	com_assert_info(data   != NULL);
#endif
	COM_VERFY_NULL(handle == NULL);
	COM_VERFY_NULL(data   == NULL);

	HANDLE new_data;
	BLOCK_HANDLE   block_handle;
	ASL_BUF_HANDLE buf_handle = handle;
	Sint32 data_block_num     = 1;
	Sint32 status             = COM_SUCCESS;
	Sint32 i                  = 0;
	Uint32 copy_size          = 0;

	//如果出现超大帧，把多个block合并在一起存储
	if (size > buf_handle->block_size)
	{
		data_block_num = (size % buf_handle->block_size) ? \
				          (size / buf_handle->block_size + 1): (size / buf_handle->block_size);
	}

	//如果数目超过了block的大小，表示帧太大，退出
	if(data_block_num > buf_handle->block_num)
	{
		COM_ERROR_INFO("the data size[%d] is too large, the cache is[%d]\n", size, buf_handle->total_size);
		return COM_ERROR;
	}

	//如果buf已经满了，则会等待
	//如果应用在多线程读写中，此处应该加锁，取出连续的两个buf
	for (i = 0; i < data_block_num; i++)
	{
		status = ASLQueue_Get(&buf_handle->empty_buf_handle, &new_data, QUEUE_TIME_WAIT);
		if (status == COM_SUCCESS)
		{
			buf_handle->block_free--;
			
			//如果当前的block是最后一个block，则使用size的余数。为了防止整除，如果等于０则表示是满block
			copy_size = (i == (data_block_num - 1)) ? buf_handle->block_size : (size % buf_handle->block_size);
			copy_size = (copy_size == 0) ? buf_handle->block_size : copy_size;

			//如果只是单帧,则标注为正常帧，如果是超大帧，则标记为为连续的block
			block_handle = (BLOCK_HANDLE)new_data;
			block_handle->block_serial   = i;
			block_handle->block_use_size = copy_size;
			block_handle->block_status = ((data_block_num == 1) || (i == (data_block_num - 1))) ? BLOCK_END : BLOCK_NOT_END;
			memcpy(block_handle->buf, data, copy_size);

			status = ASLQueue_Put(&buf_handle->full_buf_handle, new_data, QUEUE_TIME_WAIT);
			if (status == COM_SUCCESS)
			{
				buf_handle->block_use++;		
			}
		}
	}
	return COM_SUCCESS;
}

Sint32 ASLBuf_ReadData(HANDLE handle, )
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
	com_assert_info(data   != NULL);
#endif
	COM_VERFY_NULL(handle == NULL);
	COM_VERFY_NULL(data   == NULL);

}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*end _cplusplus*/
