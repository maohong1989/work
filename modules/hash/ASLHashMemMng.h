#ifndef __HASHMEMMNG_H__
#define __HASHMEMMNG_H__
/**********************************************************
用法：需填写下面几个回调函数
***********************************************************/
/*
static Sint32 AddData(HANDLE *DesHashData, HANDLE SrcInData)
{
	char *p  = malloc(3);
	char *in = SrcInData; memcpy(p, in, 3);
	*DesHashData = p;
	return COM_SUCCESS;
}
static Sint32 DeleteData(HANDLE *HashData)
{
	free(*HashData);
	*HashData = NULL;
	return COM_SUCCESS;
}
static Sint32 GetData(HANDLE DesInData, HANDLE SrcHashData)
{
	memcpy(DesInData, SrcHashData, 3);
	return COM_SUCCESS;
}
static Sint32 VerifyValue(HANDLE HashData, HANDLE InValue)
{
	char *p = HashData;
	char *pp = InValue;
	if (p[2] == *pp)
		return COM_SUCCESS;
	else
		return COM_ERROR;
}
static Sint32 PrintData(HANDLE HashData, CHAR *PrintHead)
{
	char *p = HashData;
	printf("%s [%d]\n", PrintHead, p[0]);
	printf("%s [%d]\n", PrintHead, p[1]);
	printf("%s [%d]\n", PrintHead, p[2]);
	return COM_SUCCESS;
}
*/
Sint32 ASLHashMem_Init(HANDLE *Handle, Sint32 HashTableLen);
Sint32 ASLHashMem_Exit(HANDLE *Handle);
Sint32 ASLHashMem_AddData(HANDLE handle, Sint32 HashKey, HANDLE Data);
Sint32 ASLHashMem_GetData_VerifyValue(HANDLE Handle, Sint32 HashKey, \
								   HANDLE Value, HANDLE Data);
Sint32 ASLHashMem_GetData(HANDLE Handle, Sint32 HashKey, HANDLE Data);
Sint32 ASLHashMem_DeleteData(HANDLE Handle, Sint32 HashKey);
Sint32 ASLHashMem_DeleteData_VerifyValue(HANDLE Handle, Sint32 HashKey, HANDLE Value);
Sint32 ASLHashMem_PrintHash(HANDLE Handle);
#endif

