#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ASLCommon.h>
#include "ASLHashMemMng.h"
#define COM_VERFY_VALUE(x) \
	if ((x) == 1){\
		COM_ERROR_INFO("the value is ERROR!\n");\
	}
int main()
{
	HANDLE handle;
	char p[3] = {1,2,3};
	int ret = 0;
	int i = 0;
	ASLHashMem_Init(&handle, 3);
	ret = ASLHashMem_AddData(handle, 0, p);
	COM_VERFY_VALUE(COM_ERROR == ret);
	ret = ASLHashMem_AddData(handle, 1, p);
	COM_VERFY_VALUE(COM_ERROR == ret);
	ret = ASLHashMem_AddData(handle, 2, p);
	COM_VERFY_VALUE(COM_ERROR == ret);
	p[2] = 5;
	ret = ASLHashMem_AddData(handle, 0, p);
	COM_VERFY_VALUE(COM_ERROR == ret);
	for (i = 0; i < 10; i++)
	{
		p[i % 3] = i + 2 / 4;
		ret = ASLHashMem_AddData(handle, i, p);
		COM_VERFY_VALUE(COM_ERROR == ret);
	}
	ASLHashMem_PrintHash(handle);
	char out[3] = {0};

	ret = ASLHashMem_GetData(handle, 0, out);
	COM_VERFY_VALUE(COM_ERROR == ret);
	printf("out is [%d %d %d]\n", out[0], out[1], out[2]);
	char x = 5;
	ret = ASLHashMem_DeleteData_VerifyValue(handle, 1, &x);
	COM_VERFY_VALUE(COM_ERROR == ret);

	ret = ASLHashMem_GetData_VerifyValue(handle, 0, &x, out);
	COM_VERFY_VALUE(COM_ERROR == ret);
	printf("out is [%d %d %d]\n", out[0], out[1], out[2]);
	ASLHashMem_Exit(&handle);
	return 0;

}

Sint32 AddData(HANDLE *DesHashData, HANDLE SrcInData)
{
	char *p  = malloc(3);
	char *in = SrcInData;
	memcpy(p, in, 3);
	*DesHashData = p;
	return COM_SUCCESS;
}
Sint32 DeleteData(HANDLE *HashData)
{
	free(*HashData);
	*HashData = NULL;
	return COM_SUCCESS;
}
Sint32 GetData(HANDLE DesInData, HANDLE SrcHashData)
{
	memcpy(DesInData, SrcHashData, 3);
	return COM_SUCCESS;
}
Sint32 VerifyValue(HANDLE HashData, HANDLE InValue)
{
	char *p = HashData;
	char *pp = InValue;
	if (p[2] == *pp)
		return COM_SUCCESS;
	else
		return COM_ERROR;
}
Sint32 PrintData(HANDLE HashData, CHAR *PrintHead)
{
	char *p = HashData;
	printf("%s [%d]\n", PrintHead, p[0]);
	printf("%s [%d]\n", PrintHead, p[1]);
	printf("%s [%d]\n", PrintHead, p[2]);
	return COM_SUCCESS;
}
