#include <stdio.h>
#include <stdlib.h>
#include <ASLQueueMng.h>

char buf[10][20];
HANDLE handle;

void *set_get_data(void)
{
	void *data;
	while(1)
	{
		data = NULL;
		ASLQueue_Get(handle, &data, QUEUE_TIME_WAIT);	

		if (data != NULL)
			printf("get the data[%s]\n", (char *)data);

		sleep(2);

	}
}

int main(void)
{
	pthread_t pid;	
	int i;
	int j;
	j = 0;

	ASLQueue_Init(&handle, 5);

	//填满测试与等待测试
	for (i = 0; i < 5; i++)
	{
		sprintf(buf[i], "the data is [%d]", i);	
		ASLQueue_Put(handle, buf[j], QUEUE_TIME_WAIT);	
	}
	pthread_create(&pid, NULL, set_get_data, NULL);

	j = 0;
	while(1)
	{
		ASLQueue_Put(handle, buf[j], QUEUE_TIME_WAIT);	
		printf(">>>>>>>put the data [%s]\n", buf[j]);
		sleep(1);
		j = (j+1) % 10;
	}
	ASLQueue_Exit(&handle);
}
