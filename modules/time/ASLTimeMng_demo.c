#include <stdio.h>
#include <string.h>
#include "ASLTimeMng.h"

HANDLE handle;
char test[2*1920*1080];
char test1[2*1920*1080];
int main(void)
{
	memset(test, 1, sizeof(test));

	ASLTime_Init(&handle, (CHAR *)__FUNCTION__);


	int i;
	char *pp = test;
	char *pp1 = test1;

	int *p = (int *)test;
	int *p1 = (int *)test1;

	long *p_64 = (long *)test;
	long *p1_64 = (long *)test1;

	//0
	ASLTime_Begin(handle);
	memcpy(test1, test, sizeof(test));
	ASLTime_End(handle);

	//1
	i = 1024*1024;
	ASLTime_Begin(handle);
	while(i--)
	{
		test1[i] = test[i];
	}
	ASLTime_End(handle);

	//2
	ASLTime_Begin(handle);
	i = 1024*1024;
	while(i--)
	{
		*pp1++ = *pp++;	
	}
	ASLTime_End(handle);

	//3	
	ASLTime_Begin(handle);
	i = 1024 * 1024>>2;
	while(i--)
	{
		*p1++ = *p++;
	}
	ASLTime_End(handle);

	//4
	ASLTime_Begin(handle);
	p = (int *)test;
	p1 = (int *)test1;
	i = 1024*1024>>2;
	COM_MEMCPY(p1,p,1024*1024);
	ASLTime_End(handle);

	ASLTime_Begin(handle);
	i = 1024 * 1024>>4;
	while(i--)
	{
		*p1_64++ = *p_64++;
	}
	ASLTime_End(handle);

// 	int j;	
// 	for (j = 0; j < 32; j++)
// 	{
// 		ASLTime_Begin(handle);
// 		p = (int *)test;
// 		p1 = (int *)test1;
// 		i = 1024*1024>>2;
// 		COM_MEMCPY(p1,p,1024*1024);
// 		ASLTime_End(handle);
// 	}
// 
	ASLTime_Print_And_Exit(handle, TIME_EXIT);
	return 0;
}
