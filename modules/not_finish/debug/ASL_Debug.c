#include <stdio.h>
#include <stdlib.h>
//=@_@=// 内嵌的函数


[struct] ASL_DEBUG_PARAM_S
|
|-----[char]param -->[the sturct!]
|-----[char]param -->[the sturct!]


#define STRUCT_NAME_LEN 20
typedef struct ASL_DEBUG_PARAM_S
{
    CHAR   param_name[STRUCT_NAME_LEN];
    CHAR   param_type[STRUCT_NAME_LEN];
    Uint32 param_size;
    HANDLE param_value;
}DEBUG_CFG_OBJ, *ASL_DEBUG_CFG_HANDLE;

typedef struct ASL_DEBUG_PRINTF_S
{
    CHAR struct_name[STRUCT_NAME_LEN];
    ASL_DEBUG_PARMA_S *param;
}ASL_DEBUG_PRINTF_OBJ, *ASL_DEBUG_HANDLE;

void ASL_DeBug_Pinrt_Init(HANDLE *handle, ASL_DEBUG_CFG cfg)
{
    ASL_DEBUG_PARAM_HANDLE debug_handle;

    debug_handle = malloc(sizeof(ASL_DEBUG_PARAM_OBJ));
}

void ASL_DeBug_Print_Exit()
void ASL_DeBug_Add_Sub_Param()

void ASL_Debug_Pinrt(struct ASL_Debug_Pinrt)
