#include <ASL_time.h>
#include <sys/time.h>
#include <ASL_common.h>
#include <string.h>
#ifdef ASL_TIME_DEBUG
Sint32 ASL_time_begin(ASL_TIME_HANDLE *handle)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	if (NULL == handle)
	{
		ERROR_INFO("the ASL_time_begin handle is NULL!\n");	
		return ERROR;
	}
	handle->cur_time[handle->count] = ASL_get_cur_time_us();
	return SUCCESS;
}

Sint32 ASL_time_end(ASL_TIME_HANDLE *handle)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	if (NULL == handle)
	{
		ERROR_INFO("ASL_time_end the handle is NULL\n");	
		return ERROR;
	}
	handle->cur_time[handle->count] = ASL_get_cur_time_us() - handle->cur_time[handle->count];	

	if (handle->count == 0)
	{
		handle->min_time = handle->cur_time[handle->count];	
	}

	handle->total_time += handle->cur_time[handle->count];
	if (handle->cur_time[handle->count] > handle->max_time)
	{
		handle->max_time = handle->cur_time[handle->count];
	}
	if (handle->cur_time[handle->count] < handle->min_time)
	{
		handle->min_time = handle->cur_time[handle->count];	
	}
	handle->count++;
}
Sint32 ASL_time_reset(ASL_TIME_HANDLE *handle)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	if (NULL == handle)
	{
		ERROR_INFO("ASL_time_reset handle is NULL!\n");	
		return ERROR;
	}
	memset(handle, 0, sizeof(ASL_TIME_HANDLE));
	return SUCCESS;
}
Sint32 ASL_time_print(ASL_TIME_HANDLE *handle)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	if (NULL == handle)
	{
		ERROR_INFO("ASL_time_printf the handle is NULL\n");	
		return ERROR;
	}
	Sint32 i;
/*
	printf("\n");
	for (i = 0; i < handle->count; i++)
	{
		printf("the cur_tim[%d] is:%d\n", i, handle->cur_time[i]);	
	}
	printf("count is: %d\n", handle->count);
	printf("total_time is:%d\n", handle->total_time);
	printf("average_time is:%9.2f\n", (float)handle->total_time / handle->count);
	printf("max_time is:%d\n", handle->max_time);
	printf("min_time is:%d\n", handle->min_time);
	printf("\n");
*/
	printf("\n");
	printf("num | cur_time | total_time | avg_time | max_time | min_time\n");
	for (i = 0; i < handle->count; i++)
	{
		printf("%2d  ",   i);
		printf("%9.2f  ",   (float)handle->cur_time[i] / 1000);
		printf("%10.2f  ",  (float)handle->total_time / 1000);
		printf("%9.2f  ", (float)handle->total_time /1000 / handle->count);
		printf("%9.2f  ",   (float)handle->max_time / 1000);
		printf("%9.2f  ",   (float)handle->min_time / 1000);

		printf("\n");
	}
	printf("\n");
	return SUCCESS;
}

Uint32 ASL_get_cur_time_ms()
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL) < 0)
	{
		ERROR_INFO("get time error!\n");
		return 0;
	}
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
Uint32 ASL_get_cur_time_us()
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL) < 0)
	{
		ERROR_INFO("get time error!\n");
		return 0;
	}
	return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}
#endif
