#include <stdio.h>
#include <stdlib.h>
#include <ASLCommon.h>
#include <pthread.h>
#include <string.h>
#include <ASLBufMng.h>

/***********************************************************/
typedef enum
{
	BLOCK_END = 0,
	BLOCK_NOT_END = 1
}BLOCK_STATUS_E;

typedef struct _ASL_BUF_HEAD_S
{
	HANDLE         buf;
	Uint32         block_serial;
	Uint32         block_use_size;
	BLOCK_STATUS_E block_status;
}BLOCK_OBJ, *BLOCK_HANDLE;

/***********************************************************/
void *monitor(void *handle)
{
   while(1) 
   {
        ASLBuf_ShowStatus(handle); 
//         sleep(1);
   }
}

void *read_or_write_buf(void *handle)
{
    HANDLE data;
    BLOCK_HANDLE block_handle;
    while(1)
    {
        ASLBuf_GetFull(handle, &data, BUF_TIME_WAIT);    
        block_handle = (BLOCK_HANDLE)data;

        printf("\n\n\n");
        printf("the pthread_read--->block_handle->block_serial   = [%d]\n", block_handle->block_serial);
        printf("the pthread_read--->block_handle->block_status   = [%d]\n", block_handle->block_status);
        printf("the pthread_read--->block_handle->block_use_size = [%d]\n", block_handle->block_use_size);
        printf("the pthread_read--->block_handle->buf            = [%s]\n", block_handle->buf);
        printf("\n\n\n");
        ASLBuf_PutEmpty(handle, data, BUF_TIME_WAIT);
        sleep(2);

    }
}

int main(void)
{

    //init buf
	void *handle;
    HANDLE data;
	int ret;
	ASL_BUF_CFG_OBJ buf_cfg;
    buf_cfg.block_num  = 5;
    buf_cfg.block_size = sizeof(BLOCK_OBJ) + 20;
    buf_cfg.total_size = buf_cfg.block_size * buf_cfg.block_num;
	ret = ASLBuf_Init(&handle, buf_cfg);
    if(ret != COM_SUCCESS)
    {
        printf("the init buf error!\n"); 
        return 0;
    }

	//create pthread
    pthread_t id;
    pthread_t id_1;
    pthread_create(&id, NULL, read_or_write_buf, handle);
    pthread_create(&id_1, NULL, monitor, handle);
    //
    BLOCK_HANDLE block_handle;

	//get the char
    char str[20];
    int char_len;
    int i;

    while(1)
	{
        ASLBuf_GetEmpty(handle, &data, BUF_TIME_WAIT); 

        memset(data, 0, sizeof(BLOCK_OBJ) + 4);
        block_handle      = (BLOCK_HANDLE)data;
        block_handle->buf = data + sizeof(BLOCK_OBJ);

        printf("create a block!");
        printf("set data:\n");
        scanf("%s", str);
        block_handle->block_serial   = i++;
        block_handle->block_status   = 0;
        block_handle->block_use_size = strlen(str);
        memcpy(block_handle->buf, str, strlen(str));

        ASLBuf_PutFull(handle, data, BUF_TIME_WAIT);
    }
	ASLBuf_Exit(&handle);
	return 0;
}
