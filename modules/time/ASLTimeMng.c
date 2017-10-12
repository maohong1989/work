////////////////////////////////////////////////////////////////////////////////////
//                        版权所有，作者保留一切权利
// 在保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播。
// 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途
////////////////////////////////////////////////////////////////////////////////////
// 文 件 名: ASLTimeMng.c
// 作    者: apostle --- apostle9891@foxmail.com
// 版    本: version 1.0
// 日    期: 2014-07-15 21:23
// 描    述: 测量程序运行时间
// 历史描述: 1 20170330 重新更改了架构，使用handle形式，更改了最多可以设置２０个时间
//             的BUG
////////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <ASLCommon.h>
#include <ASLTimeMng.h>


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif
#ifdef ASL_TIME_DEBUG

#define TIME_NUM     20
#define MARKNAME_LEN 30
#define MAX_TIME     5

////////////////////////////////////////////////////////////////////////////////////
typedef struct _ctagsTIME
{
	Uint32 num;
	Uint32 time;
}TIME_OBJ, *TIME_HANDLE;

typedef struct{
	CHAR       mark_name[MARKNAME_LEN];
	Uint32     total_time;
	Uint32     count;
	TIME_OBJ   cur_time[TIME_NUM];
	TIME_OBJ   max_time[MAX_TIME];
	TIME_OBJ   min_time[MAX_TIME];
}ASL_TIME_OBJ,*ASL_TIME_HANDLE;

static inline Uint32 ASL_get_cur_time_ms();
static inline Uint32 ASL_get_cur_time_us();

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASL_get_cur_time_ms
// 功    能：获取当前时间，以ms级
// 参    数：[IN] Null -- 输入为空
// 返 回 值: 失败-1，成功0
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
static inline Uint32 ASL_get_cur_time_ms()
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL) < 0)
	{
		COM_ERROR_INFO("get time error!\n");
		return 0;
	}
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASL_get_cur_time_us
// 功    能：获取当前时间，以us级
// 参    数：[IN] Null -- 输入为空
// 返 回 值: 失败-1，成功0
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
static inline Uint32 ASL_get_cur_time_us()
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL) < 0)
	{
		COM_ERROR_INFO("get time error!\n");
		return 0;
	}
	return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLTime_Init
// 功    能：初始化函数,mark_name为标记的名称，推荐使用__FUNCTION__
// 参    数：[IN] handle     -- 模块句柄
//           [IN] mark_name  -- 标记名称
// 返 回 值: 失败-1，成功0
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLTime_Init(HANDLE *handle, CHAR *mark_name)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);

	ASL_TIME_HANDLE time_handle;

	time_handle = malloc(sizeof(ASL_TIME_OBJ));
	COM_VERFY_NULL(NULL == time_handle);

	memset(time_handle, 0, sizeof(ASL_TIME_OBJ));

	if (NULL == mark_name)
	{
		memcpy(time_handle->mark_name, "ASL_TIME_SHOW", 14);
	}
	else
	{
		Uint32 string_len;
		string_len = strlen(mark_name);

		if (string_len > MARKNAME_LEN)
		{
			COM_WARN_INFO("the ASL_time_set_markname the mark_name is long than default len\n");
		}
		memcpy(time_handle->mark_name, mark_name, string_len > MARKNAME_LEN ? MARKNAME_LEN:string_len);
	}

	*handle = (HANDLE)time_handle;

	return COM_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLTime_Begin
// 功    能：ASLTime初始化
// 参    数：[IN] handle  -- 模块句柄
// 返 回 值: 失败-1，成功0
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLTime_Begin(HANDLE handle)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);

	ASL_TIME_HANDLE time_handle = (ASL_TIME_HANDLE)handle;;
	Uint32 Align                = 0;
	Uint32 Offset               = 0;

	//１存储当前的时间cur_time
	//如果存储的时间超过了最大存储时间，则从TIME_NUM／2后开始进行存储
	if (time_handle->count >= TIME_NUM)
	{
		Align  = (TIME_NUM >> 1);
		Offset = Align + time_handle->count % Align;
		time_handle->cur_time[Offset].time = ASL_get_cur_time_us();
		time_handle->cur_time[Offset].num  = time_handle->count;
	}
	else
	{
		time_handle->cur_time[time_handle->count].time = ASL_get_cur_time_us();
		time_handle->cur_time[time_handle->count].num  = time_handle->count;
	}

	return COM_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLTime_End
// 功    能：ASLtime模块停止
// 参    数：[IN] handle  -- 模块句柄
// 返 回 值: 失败-1，成功0
// 说    明: 1 存储当前的时间。现阶段最多支持存储TIME_NUM(20)个数值，如果超过存储个数，
//             则丢弃中间数值，存储前10个数值和后10个数值
//           2 存储最大的前５个数值与最小的５个数值
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLTime_End(HANDLE handle)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);

	ASL_TIME_HANDLE time_handle = (ASL_TIME_HANDLE)handle;;
	Uint32 Align                = 0;
	Uint32 Offset               = 0;
	Uint32 Cur_time             = 0;
	Uint32 i                    = 0;
	Uint32 j                    = 0;

	//１存储当前的时间cur_time
	//如果存储的时间超过了最大存储时间，则从TIME_NUM／2后开始进行存储
	if (time_handle->count >= TIME_NUM)
	{
		Align = (TIME_NUM >> 1);
		Offset = Align + time_handle->count % Align;
		time_handle->cur_time[Offset].time = ASL_get_cur_time_us() - time_handle->cur_time[Offset].time;
		Cur_time = time_handle->cur_time[Offset].time;
	}
	else
	{
		time_handle->cur_time[time_handle->count].time = \
		ASL_get_cur_time_us() - time_handle->cur_time[time_handle->count].time;
		Cur_time = time_handle->cur_time[time_handle->count].time;
	}

	//2 累加所有时间
	time_handle->total_time += Cur_time;

	//3 存储最大的５个时间
	for (i = 0; i < MAX_TIME; i++)
	{
		if (time_handle->max_time[i].time < Cur_time)
		{
			j = MAX_TIME - 1;
			while(j > i)
			{
				time_handle->max_time[j].time = time_handle->max_time[j-1].time;
				time_handle->max_time[j].num  = time_handle->max_time[j-1].num;
				j--;
			}

			time_handle->max_time[i].time = Cur_time;
			time_handle->max_time[i].num  = time_handle->count;
			break;
		}
	}

	//4 存储最少的５个时间
	for (i = 0; i < MAX_TIME; i++)
	{
		if (time_handle->min_time[i].time == 0)
		{
			time_handle->min_time[i].time = Cur_time;
			time_handle->min_time[i].num  = time_handle->count;
			break;
		}

		if (time_handle->min_time[i].time > 0 && time_handle->min_time[i].time > Cur_time)
		{
			j = MAX_TIME - 1;
			while(j > i)
			{
				time_handle->min_time[j].time = time_handle->min_time[j-1].time;
				time_handle->min_time[j].num  = time_handle->min_time[j-1].num;
				j--;
			}

			time_handle->min_time[i].time = Cur_time;
			time_handle->min_time[i].num  = time_handle->count;
			break;
		}
	}

	//5 时间数值增大
	time_handle->count++;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLTime_Reset
// 功    能：重新设置参数
// 参    数：[IN] handle  -- 模块句柄
// 返 回 值: 失败-1，成功0
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLTime_Reset(HANDLE handle)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(handle == NULL);
	memset(handle, 0, sizeof(ASL_TIME_OBJ));

	return COM_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLTime_Print
// 功    能：打印时间
// 参    数：[IN] handle  -- 模块句柄
// 返 回 值: 失败-1，成功0
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLTime_Print_And_Exit(HANDLE handle, EXIT_OR_NOT_EXIT_E Exit_Flags)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(handle == NULL);
	Sint32 i;

	ASL_TIME_HANDLE time_handle = (ASL_TIME_HANDLE)handle;;

	printf("\n");
	printf("*%28s%29s*\n", \
			strlen(time_handle->mark_name) > 0 ? time_handle->mark_name : (CHAR *)__FUNCTION__, " ");

	//1 set cmd
	printf("*---------------------------------------------------------*\n");
	printf("*%1s%9s%1s|%1s%10s%1s|%1s%8s%1s|%1s%8s%1s|%1s%8s%1s*\n", \
			"", "total_num", "", \
			"", "total_time", "", \
			"", "avg_time", "", \
			"", "max_time", "", \
			"", "min_time", "");

	//2 find time value
	printf("*%3s%4d%4s|%2s%8.3f%2s|%1s%8.3f%1s|%1s%8.3f%1s|%1s%8.3f%1s*\n", \
		   "", time_handle->count, "",\
		   "", (float)time_handle->total_time / 1000, "",\
		   "", (float)time_handle->total_time /1000 / time_handle->count, "",\
		   "", (float)time_handle->max_time[0].time / 1000, "", \
		   "", (float)time_handle->min_time[0].time / 1000, "");
	printf("*---------------------------------------------------------*\n");
	printf("*%3s%3s%1s|%1s%8s%3s|%1s%3s%1s|%1s%8s%3s|%1s%3s%1s|%1s%8s%1s *\n", \
			"", "num", "", "", "cur_time", "", \
			"", "num", "", "", "max_time", "", \
			"", "num", "", "", "min_time", "");

	if (time_handle->count < TIME_NUM)
	{
		for (i = 0; i < TIME_NUM; i++)
		{
			if (time_handle->cur_time[i].time != 0)
			{
				printf("*");
				printf("%3s%3d%1s", "",time_handle->cur_time[i].num, "");
				printf("%1s%8.3f%4s",  "", (float)time_handle->cur_time[i].time / 1000, "");
				printf("|");

				if (i < MAX_TIME)
				{
					//set max_time
					printf("%1s%3d%s", "", time_handle->max_time[i].num, "");
					printf("%2s%8.3f%4s", "", (float)time_handle->max_time[i].time / 1000, "");
					printf("|");

					//set max_time
					printf("%1s%3d%s", "", time_handle->min_time[i].num, "");
					printf("%1s%8.3f%2s", "", (float)time_handle->min_time[i].time / 1000, "");
					printf(" ");
				}
				else
				{
					printf("%35s", "");
				}
				printf(" *");
				printf("\n");
			}

		}
		printf("***********************************************************\n");
		printf("\n");
	}
	else
	{
		//1 打印前TIME_NUM/2 的数值
		for (i = 0; i < TIME_NUM / 2; i++)
		{
			if (time_handle->cur_time[i].time != 0)
			{
				printf("*");
				printf("%3s%3d%1s", "",time_handle->cur_time[i].num, "");
				printf("%1s%8.3f%4s",  "", (float)time_handle->cur_time[i].time / 1000, "");
				printf("|");

				if (i < MAX_TIME)
				{
					//set max_time
					printf("%1s%3d%s", "", time_handle->max_time[i].num, "");
					printf("%2s%8.3f%4s", "", (float)time_handle->max_time[i].time / 1000, "");
					printf("|");

					//set max_time
					printf("%1s%3d%s", "", time_handle->min_time[i].num, "");
					printf("%1s%8.3f%2s", "", (float)time_handle->min_time[i].time / 1000, "");
					printf(" ");
				}
				else
				{
					printf("%35s", "");
				}
				printf(" *");
				printf("\n");
			}

		}
		//2 打印省略号
		printf("*%2s%4s%s","", "...", ""); 
		printf("%2s%8s%4s|","", "...", ""); 
		printf("%36s*\n", "");
		printf("*%20s|%36s*\n", "", "");

		//3 打印后TIME_NUM/2的数值,由于后面的数值是循环写入，需要找到头

		int Align  = 0;
		int Offset = 0;

		Align  = (TIME_NUM >> 1);
		Offset = (time_handle->count - Align) % Align;

		for (i = TIME_NUM / 2; i < TIME_NUM; i++)
		{
			Offset = (i + Offset >= TIME_NUM) ? (Offset - Align):Offset;

			if (time_handle->cur_time[i+Offset].time != 0)
			{
				printf("*");
				printf("%3s%3d%1s", "",time_handle->cur_time[i + Offset].num, "");
				printf("%1s%8.3f%4s",  "", (float)time_handle->cur_time[i+Offset].time / 1000, "");

				printf("|");
				printf("%35s", "");
				printf(" *");
				printf("\n");
			}

		}
		printf("***********************************************************\n");
		printf("\n");
	}

	if (Exit_Flags == TIME_EXIT)
	{
		free(time_handle);
		time_handle = NULL;
	}

	return COM_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////
// 函 数 名: ASLTime_Exit
// 功    能：注销函数
// 参    数：[IN] handle     -- 模块句柄
// 返 回 值: 失败-1，成功0
// 说    明:
////////////////////////////////////////////////////////////////////////////////////
Sint32 ASLTime_Exit(HANDLE handle)
{
#ifdef ASL_COMMON_DEBUG
	com_assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(handle == NULL);

	free(handle);
	handle = NULL;

	return COM_SUCCESS;
}



#endif /*end def ASL_TIME_DEBUG*/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

