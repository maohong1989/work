////////////////////////////////////////////////////////////////////////////////////
//                        版权所有，作者保留一切权利
// 但保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播。
// 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途
////////////////////////////////////////////////////////////////////////////////////
// 文 件 名: HashMemMng.c
// 作    者: apostle --- apostle9891@foxmail.com
// 版    本: version 1.0
// 日    期: 2013-12-28 08:37
// 描    述:
// 历史描述:
////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ASLCommon.h"
#include "ASLHashMemMng.h"
extern Sint32 AddData(HANDLE *DesHashData, HANDLE SrcInData);
extern Sint32 DeleteData(HANDLE *HashData);
extern Sint32 GetData(HANDLE DesInData, HANDLE SrcHashData);
extern Sint32 VerifyValue(HANDLE HashData, HANDLE InValue);
extern Sint32 PrintData(HANDLE HashData, CHAR *PrintHead);
/************************************************************/
//private struct
typedef struct _tags_HASHMEM_NODE_S
{
	Sint32                       HashKey;
	HANDLE                       HashData;
	struct _tags_HASHMEM_NODE_S  *NextHashNode;
}HASHMEM_NODE_OBJ, *HASHMEM_NODE_HANDLE;

typedef struct _tags_HASHMEM_HEAD_S
{
	Sint32              NodeLen;
	HASHMEM_NODE_HANDLE HashNode;
}HASHMEM_HEAD_OBJ, *HASHMEM_HEAD_HANDLE;

typedef struct _tags_HASHMEM_MNG_S
{
	Sint32              HashTableLen;
	HASHMEM_HEAD_HANDLE HashMemHead;
	Sint32 (*Fun_AddData)(HANDLE *, HANDLE);
	Sint32 (*Fun_DeleteData)(HANDLE *);
	Sint32 (*Fun_GetData)(HANDLE, HANDLE);
	Sint32 (*Fun_VerifyValue)(HANDLE, HANDLE);
	Sint32 (*Fun_PrintData)(HANDLE, CHAR*);
}HASHMEM_MNG_OBJ, *HASHMEM_MNG_HANDLE;
/************************************************************/

Sint32 ASLHashMem_Init(HANDLE *Handle, Sint32 HashTableLen)
{
	HASHMEM_MNG_HANDLE HashMemHandle = NULL;
	Sint32 Len = HashTableLen;
	Sint32 i   = 0;
	HashMemHandle = malloc(sizeof(HASHMEM_MNG_OBJ));
	COM_VERFY_NULL(NULL == HashMemHandle);
	memset(HashMemHandle, 0, sizeof(HASHMEM_MNG_OBJ));

	HashMemHandle->HashTableLen = Len;
	HashMemHandle->HashMemHead  = malloc(sizeof(HASHMEM_HEAD_OBJ) * Len);
	COM_VERFY_NULL(NULL == HashMemHandle->HashMemHead);
	memset(HashMemHandle->HashMemHead, 0, sizeof(HASHMEM_HEAD_OBJ) * Len);
	for (i = 0; i < Len; i++)
	{
		HashMemHandle->HashMemHead[i].HashNode = NULL;
	}

	HashMemHandle->Fun_AddData     = AddData;
	HashMemHandle->Fun_DeleteData  = DeleteData;
	HashMemHandle->Fun_GetData     = GetData;
	HashMemHandle->Fun_VerifyValue = VerifyValue;
	HashMemHandle->Fun_PrintData   = PrintData;

	*Handle = HashMemHandle;
	return COM_SUCCESS;
}
Sint32 ASLHashMem_Exit(HANDLE *Handle)
{
	HASHMEM_NODE_HANDLE NextHashNodeHandle = NULL;
	HASHMEM_NODE_HANDLE CurHashNodeHandle  = NULL;
	HASHMEM_MNG_HANDLE HashMemHandle       = *Handle;
	Sint32 Len = HashMemHandle->HashTableLen;
	Sint32 i   = 0;

	for (i = 0; i < Len; i++)
	{
		CurHashNodeHandle = HashMemHandle->HashMemHead[i].HashNode;
		while(NULL != CurHashNodeHandle)
		{
			NextHashNodeHandle = CurHashNodeHandle->NextHashNode;
			HashMemHandle->Fun_DeleteData(&CurHashNodeHandle->HashData);
			free(CurHashNodeHandle);
			CurHashNodeHandle = NextHashNodeHandle;
		}
		HashMemHandle->HashMemHead[i].HashNode = NULL;
	}
	free(HashMemHandle->HashMemHead);
	free(HashMemHandle);
	HashMemHandle = NULL;
	*Handle = HashMemHandle;
	return COM_SUCCESS;
}
Sint32 ASLHashMem_AddData(HANDLE Handle, Sint32 HashKey, HANDLE Data)
{
	HASHMEM_MNG_HANDLE  HashMemHandle      = Handle;
	HASHMEM_HEAD_HANDLE HashMemHeadHandle  = NULL;
	HASHMEM_NODE_HANDLE CurHashNodeHandle  = NULL;
	HASHMEM_NODE_HANDLE PrevHashNodeHandle = NULL;
	HASHMEM_NODE_HANDLE NextHashNodeHandle = NULL;
	Sint32 HashLen   = HashMemHandle->HashTableLen;
	Sint32 NodeIndex = 0;
// 	Sint32 Ret       = 0;

	NodeIndex = HashKey % HashLen;
	HashMemHeadHandle  = &(HashMemHandle->HashMemHead[NodeIndex]);
	PrevHashNodeHandle = HashMemHeadHandle->HashNode;
	NextHashNodeHandle = PrevHashNodeHandle;
	while(NULL != NextHashNodeHandle)
	{
		PrevHashNodeHandle = NextHashNodeHandle;
		NextHashNodeHandle = NextHashNodeHandle->NextHashNode;
	}
	CurHashNodeHandle = malloc(sizeof(HASHMEM_NODE_OBJ));
	COM_VERFY_NULL(NULL == CurHashNodeHandle);
	memset(CurHashNodeHandle, 0, sizeof(HASHMEM_NODE_OBJ));
	HashMemHandle->Fun_AddData(&CurHashNodeHandle->HashData, Data);
	CurHashNodeHandle->HashKey       = HashKey;
	CurHashNodeHandle->NextHashNode  = NULL;
	if (NULL == PrevHashNodeHandle)//the head
		HashMemHeadHandle->HashNode = CurHashNodeHandle;
	else
		PrevHashNodeHandle->NextHashNode = CurHashNodeHandle;
	HashMemHeadHandle->NodeLen      +=1;
	return COM_SUCCESS;
}
Sint32 ASLHashMem_GetData(HANDLE Handle, Sint32 HashKey, HANDLE Data)
{
	HASHMEM_MNG_HANDLE  HashMemHandle      = Handle;
	HASHMEM_NODE_HANDLE CurHashNodeHandle  = NULL;
	HASHMEM_HEAD_HANDLE HashMemHeadHandle  = NULL;
	Sint32 HashLen   = HashMemHandle->HashTableLen;
	Sint32 NodeIndex = 0;

	NodeIndex = HashKey % HashLen;
	HashMemHeadHandle  = &(HashMemHandle->HashMemHead[NodeIndex]);
	CurHashNodeHandle  = HashMemHeadHandle->HashNode;
	while(NULL != CurHashNodeHandle)
	{
		if (HashKey == CurHashNodeHandle->HashKey)
		{
			HashMemHandle->Fun_GetData(Data, CurHashNodeHandle->HashData);
			return COM_SUCCESS;
		}
		CurHashNodeHandle = CurHashNodeHandle->NextHashNode;
	}
	printf("there is no find!\n");
	return COM_ERROR;
}

Sint32 ASLHashMem_GetData_VerifyValue(HANDLE Handle, Sint32 HashKey, \
								   HANDLE Value, HANDLE Data)
{

	HASHMEM_MNG_HANDLE  HashMemHandle      = Handle;
	HASHMEM_NODE_HANDLE CurHashNodeHandle  = NULL;
	HASHMEM_HEAD_HANDLE HashMemHeadHandle  = NULL;
	Sint32 HashLen   = HashMemHandle->HashTableLen;
	Sint32 NodeIndex = 0;

	NodeIndex = HashKey % HashLen;
	HashMemHeadHandle  = &(HashMemHandle->HashMemHead[NodeIndex]);
	CurHashNodeHandle  = HashMemHeadHandle->HashNode;
	while(NULL != CurHashNodeHandle)
	{
		if (COM_SUCCESS == \
			HashMemHandle->Fun_VerifyValue(CurHashNodeHandle->HashData, Value))
		{
			HashMemHandle->Fun_GetData(Data, CurHashNodeHandle->HashData);
			return COM_SUCCESS;
		}
		CurHashNodeHandle = CurHashNodeHandle->NextHashNode;
	}
	printf("there is no find!\n");
	return COM_ERROR;
}
Sint32 ASLHashMem_DeleteData(HANDLE Handle, Sint32 HashKey)
{
	HASHMEM_MNG_HANDLE  HashMemHandle      = Handle;
// 	HASHMEM_NODE_HANDLE CurHashNodeHandle  = NULL;
	HASHMEM_HEAD_HANDLE HashMemHeadHandle  = NULL;
	HASHMEM_NODE_HANDLE PrevHashNodeHandle = NULL;
	HASHMEM_NODE_HANDLE NextHashNodeHandle = NULL;
	Sint32 HashLen   = HashMemHandle->HashTableLen;
	Sint32 NodeIndex = 0;

	NodeIndex = HashKey % HashLen;
	HashMemHeadHandle  = &(HashMemHandle->HashMemHead[NodeIndex]);
	NextHashNodeHandle = HashMemHeadHandle->HashNode;
	PrevHashNodeHandle = NextHashNodeHandle;
	while(NULL != NextHashNodeHandle)
	{
		if (HashKey == NextHashNodeHandle->HashKey)
		{
			HashMemHandle->Fun_DeleteData(&(NextHashNodeHandle->HashData));
			if (NULL != PrevHashNodeHandle->NextHashNode)//the nodelen > 1
			{
				if (PrevHashNodeHandle == NextHashNodeHandle) // delete the head
					HashMemHeadHandle->HashNode = NextHashNodeHandle->NextHashNode;
				else
					PrevHashNodeHandle->NextHashNode = NextHashNodeHandle->NextHashNode;
				free(NextHashNodeHandle);
			}
			else//the nodelen == 1
			{
				free(PrevHashNodeHandle);
				HashMemHeadHandle->HashNode = NULL;

			}
			HashMemHeadHandle->NodeLen -= 1;
			return COM_SUCCESS;
		}
		PrevHashNodeHandle = NextHashNodeHandle;
		NextHashNodeHandle = NextHashNodeHandle->NextHashNode;

	}
	return COM_ERROR;
}

Sint32 ASLHashMem_DeleteData_VerifyValue(HANDLE Handle, Sint32 HashKey, HANDLE Value)
{
	HASHMEM_MNG_HANDLE  HashMemHandle      = Handle;
// 	HASHMEM_NODE_HANDLE CurHashNodeHandle  = NULL;
	HASHMEM_HEAD_HANDLE HashMemHeadHandle  = NULL;
	HASHMEM_NODE_HANDLE PrevHashNodeHandle = NULL;
	HASHMEM_NODE_HANDLE NextHashNodeHandle = NULL;
	Sint32 HashLen   = HashMemHandle->HashTableLen;
	Sint32 NodeIndex = 0;

	NodeIndex = HashKey % HashLen;
	HashMemHeadHandle  = &(HashMemHandle->HashMemHead[NodeIndex]);
	NextHashNodeHandle = HashMemHeadHandle->HashNode;
	PrevHashNodeHandle = NextHashNodeHandle;
	while(NULL != NextHashNodeHandle)
	{
		if (COM_SUCCESS == \
			HashMemHandle->Fun_VerifyValue(NextHashNodeHandle->HashData, Value))
		{
			HashMemHandle->Fun_DeleteData(&NextHashNodeHandle->HashData);
			if (NULL != PrevHashNodeHandle->NextHashNode)
			{
				if (PrevHashNodeHandle == NextHashNodeHandle) // delete the head
					HashMemHeadHandle->HashNode = NextHashNodeHandle->NextHashNode;
				else
					PrevHashNodeHandle->NextHashNode = NextHashNodeHandle->NextHashNode;
				free(NextHashNodeHandle);
			}
			else
			{
				free(PrevHashNodeHandle);
				HashMemHeadHandle->HashNode = NULL;
			}
			HashMemHeadHandle->NodeLen -= 1;
			return COM_SUCCESS;
		}
		PrevHashNodeHandle = NextHashNodeHandle;
		NextHashNodeHandle = NextHashNodeHandle->NextHashNode;

	}
	return COM_ERROR;
}
Sint32 ASLHashMem_PrintHash(HANDLE Handle)
{
	HASHMEM_MNG_HANDLE  HashMemHandle      = Handle;
	HASHMEM_NODE_HANDLE CurHashNodeHandle  = NULL;
	HASHMEM_HEAD_HANDLE HashMemHeadHandle  = NULL;
	HASHMEM_NODE_HANDLE NextHashNodeHandle = NULL;
	CHAR StrHead[] = "       |-->HashData --->";
	Sint32 i = 0;
	Sint32 j = 0;
	printf("***********************************************************\n");
	printf("* HashTableLen ===> [%d]\n", HashMemHandle->HashTableLen);
	printf("***********************************************************\n");
	for (i = 0; i < HashMemHandle->HashTableLen; i++)
	{
		HashMemHeadHandle  = &(HashMemHandle->HashMemHead[i]);
		CurHashNodeHandle  = HashMemHeadHandle->HashNode;
		printf("HashTableLen[%d]--->Nodelen ===> [%d]\n", \
				i, HashMemHeadHandle->NodeLen);
		j = 0;
		while(NULL != CurHashNodeHandle)
		{
			NextHashNodeHandle = CurHashNodeHandle->NextHashNode;
			printf("--------------------------------\n");
			printf("Node[%d]--->HashKey  ===> [%d]\n", j, CurHashNodeHandle->HashKey);	
			HashMemHandle->Fun_PrintData(CurHashNodeHandle->HashData, StrHead);
			j++;
			CurHashNodeHandle = NextHashNodeHandle;
		}
		printf("***********************************************************\n");
	}
}
