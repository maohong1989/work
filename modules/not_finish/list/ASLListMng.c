////////////////////////////////////////////////////////////////////////////////////
//                        版权所有，作者保留一切权利
// 在保证本程序完整性（包括版权申明，作者信息）的前提下，欢迎任何人对此进行修改传播。
// 作者邮箱：apostle9891@foxmail.com，欢迎进行交流，请勿用于商业用途
////////////////////////////////////////////////////////////////////////////////////
// 文 件 名: ASLList.c
// 作    者: apostle --- apostle9891@foxmail.com
// 版    本: version 1.0
// 日    期: 2017-04-02 23:44
// 描    述:
// 历史描述:
////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ASLList.h>


////////////////////////////////////////////////////////////////////////////////////
//=@_@=// define定义区


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//=@_@=// 内部结构体申明区
//        如果是大型的软件，内部结构最好使用一个private的头文件包含

typedef struct ASL_NODE_ATTR_S
{
    Uint32  node_count;
    DESTROY_DATA destroy_function;
    struct ASL_NODE_ATTR_S *prev_node; 
    struct ASL_NODE_ATTR_S **next_node; 
    NODE_DATA_OBJ  node_data;
}NODE_ATTR_OBJ, *NODE_ATTR_HANDLE;

typedef struct ASL_LIST_MNG_S
{
    Uint32 total_depth;
    Uint32 cur_depth;
    NODE_ATTR_HANDLE head; 
    NODE_ATTR_HANDLE cur_node_handle; 
}ASL_LIST_MNG_OBJ, *ASL_LIST_MNG_HANDLE:

////////////////////////////////////////////////////////////////////////////////////
//=@_@=// 软件代码实现区

Sint32 ASLList_Init(HANDLE *handle, LIST_DATA_CFG_OBJ data_cfg)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);

    if (data_cfg.list_count < 1)
    {
        COM_ERROR_INFO("the list count[%d] < 1!\n", data_cfg.list_count);
        return COM_ERROR;
    }

    NODE_ATTR_HANDLE    node_handle;
    ASL_LIST_MNG_HANDLE list_handle;
    list_handle = malloc(sizeof(ASL_LIST_MNG_OBJ));
    COM_VERFY_NULL(NULL == list_handle);
    node_handle = malloc(sizeof(NODE_ATTR_OBJ));
    COM_VERFY_NULL(NULL == node_handle);

    memset(list_handle, 0, sizeof(ASL_LIST_MNG_OBJ));
    memset(node_handle, 0, sizeof(NODE_ATTR_OBJ));

    list_handle->head            = node_handle;
    list_handle->cur_node_handle = node_handle;
    list_handle->total_depth     = 0;
    list_handle->cur_depth       = 0;

    node_handle->prev_node        = NULL;
    node_handle->node_count       = data_cfg.list_count;
    node_handle->node_data.size   = data_cfg.list_data.size;
    node_handle->node_data.data   = data_cfg.list_data.data;
    node_handle->destroy_function = data_cfg.destroy_function;
    node_handle->next_node = malloc(sizeof(NODE_ATTR_HANDLE) * data_cfg.list_count);
    if (NULL == list_handle->next_node)
    {
        COM_ERROR_INFO("malloc the list is null!\n");     
        free(node_handle);
        free(list_handle);
        return COM_ERROR;
    }
    memset(node_handle->next_node, 0, sizeof(NODE_ATTR_HANDLE) * data_cfg.list_count);
    *handle = list_handle;
    return COM_SUCCESS;
}

Sint32 ASLList_Exit(HANDLE *handle)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(*handle != NULL);
#endif
	COM_VERFY_NULL(NULL == *handle);
    ASL_LIST_MNG_HANDLE list_handle = *handle;
}

Sint32 ASLList_SetNodeData(HANDLE handle, NODE_DATA_OBJ data_cfg)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
    assert_info(data_cfg.list_data.data != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);
	COM_VERFY_NULL(data_cfg.data == NULL);
    ASL_LIST_MNG_HANDLE list_handle = handle;

    //设置的node节点一定要在可用的范围内

    if ((list_handle->cur_node_handle->list_data.size != 0) && \
        (list_handle->cur_node_handle->list_data.data != NULL))
    {
        COM_WARN_INFO("the data is not NULL!,default free\n");
        free(list_handle->cur_node_handle->list_data.data);
        list_handle->cur_node_handle->list_data.size = 0;
        list_handle->cur_node_handle->list_data.data = NULL;

    }
    list_handle->cur_node_handle->node_data.size = data_cfg.size;
    list_handle->cur_node_handle->node_data.data = data_cfg.data;

    return COM_SUCCESS;
}

Sint32 ASLList_GetData(HANDLE handle, LIST_DATA_HANDLE list_data)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);
    ASL_LIST_MNG_HANDLE list_handle = handle;

    list_data.size = list_handle->cur_node_handle->node_data.size;
    list_data.data = list_handle->cur_node_handle->node_data.data;
    
    return COM_SUCCESS;
}

Sint32 ASLList_GoNext(HANDLE handle, Uint32 node_num)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);
    ASL_LIST_MNG_HANDLE list_handle = handle;

    if ((node_num > list_handle->cur_node_handle->node_count) || (node_num < 0))
    {
        COM_ERROR_INFO("the node num[%d] is not range to node_count[%d]\n", \
                        node_num, list_handle->node_count);
        return COM_ERROR;
    }

    if (list_handle->cur_node_handle->next_node[node_num] != NULL)
    {
        list_handle->cur_node_handle = list_handle->cur_node_handle->next_node[node_num];
        list_handle->cur_depth++;
    }
    else
    {
        COM_ERROR_INFO("the node is the last node, next is NULL!please check\n");
        return COM_ERROR;
    }
    return COM_SUCCESS;
}
Sint32 ASLList_GoPrev(HANDLE handle)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);

    list_handle->cur_node_handle = list_handle->cur_node_handle->prev_node;
    list_handle->cur_depth--;
    return COM_SUCCESS;
}

Sint32 ASLList_Prev_Insert(HANDLe handle, Uint32 node_num, LIST_DATA_CFG_OBJ data_cfg)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
    assert_info(data_cfg.list_data.data != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);
	COM_VERFY_NULL(data_cfg.list_data.data == NULL);

    if (data_cfg.list_count < 1)
    {
        COM_ERROR_INFO("the list count[%d] < 1!\n", data_cfg.list_count);
        return COM_ERROR;
    }
    if (node_num < 0 || (node_num > data_cfg.list_count))
    {
        COM_ERROR_INFO("the node num [%d] is error!, list_count is[%d]\n", node_num, data_cfg.list_count);     
        return COM_ERROR;
    }

    ASL_LIST_MNG_HANDLE list_handle = handle;
    NODE_ATTR_HANDLE    node_handle;
    node_handle = malloc(sizeof(NODE_ATTR_OBJ));
    COM_VERFY_NULL(NULL == node_handle);
    memset(node_handle, 0, sizeof(NODE_ATTR_OBJ));

    node_handle->node_count       = data_cfg.list_count;
    node_handle->node_data.size   = data_cfg.list_data.size;
    node_handle->node_data.data   = data_cfg.list_data.data;
    node_handle->destroy_function = data_cfg.destroy_function;
    node_handle->next_node = malloc(sizeof(NODE_ATTR_HANDLE) * data_cfg.list_count);
    if (NULL == list_handle->next_node)
    {
        COM_ERROR_INFO("malloc the list is null!\n");     
        free(node_handle);
        return COM_ERROR;
    }
    memset(node_handle->next_node, 0, sizeof(NODE_ATTR_HANDLE) * data_cfg.list_count):

    if(NULL == list_count->cur_node_handle->prev_node)
    {
        node_handle->prev_node                  = NULL;
        node_handle->next_node[node_num]        = list_handle->cur_node_handle;
        list_handle->cur_node_handle->prev_node = node_handle;
        list_handle->head                       = node_handle;
    }
    else
    {
        node_handle->prev_node                  = list_handle->cur_node_handle->prev_node;
        node_handle->next_node[node_num]        = list_handle->cur_node_handle;
        list_handle->cur_node_handle->prev_node = node_handle;
        list_handle->cur_node_handle->prev_node = node_handle;
    }

    list_handle->cur_depth++;
    list_handle->total_depth++;

    return COM_SUCCESS;
}
Sint32 ASLList_Next_Insert(HANDLe handle, Uint32 prev_node_num, \
                           Uint32 next_node_num, LIST_DATA_CFG_OBJ data_cfg)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
    assert_info(data_cfg.list_data.data != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);
	COM_VERFY_NULL(data_cfg.list_data.data == NULL);

    if (data_cfg.list_count < 1)
    {
        COM_ERROR_INFO("the list count[%d] < 1!\n", data_cfg.list_count);
        return COM_ERROR;
    }
    if (prev_node_num < 0 || (prev_node_num > list_handle->cur_node_handle->node_count))
    {
        COM_ERROR_INFO("the node num [%d] is error!, cur_list_count is[%d]\n", \
                        prev_node_num, list_handle->cur_node_handle->node_count);     
        return COM_ERROR;
    }
    if(next_node_num < 0 || next_node_num > data_cfg.list_count)
    {
        COM_ERROR_INFO("the next node num [%d] is not range list_count[%d]\n", \
                        next_node_num, data_cfg.list_count);     
        return COM_ERROR;
    }

    ASL_LIST_MNG_HANDLE list_handle = handle;
    NODE_ATTR_HANDLE    node_handle;
    node_handle = malloc(sizeof(NODE_ATTR_OBJ));
    COM_VERFY_NULL(NULL == node_handle);
    memset(node_handle, 0, sizeof(NODE_ATTR_OBJ));

    node_handle->node_count       = data_cfg.list_count;
    node_handle->node_data.size   = data_cfg.list_data.size;
    node_handle->node_data.data   = data_cfg.list_data.data;
    node_handle->destroy_function = data_cfg.destroy_function;
    node_handle->next_node = malloc(sizeof(NODE_ATTR_HANDLE) * data_cfg.list_count);
    if (NULL == list_handle->next_node)
    {
        COM_ERROR_INFO("malloc the list is null!\n");     
        free(node_handle);
        return COM_ERROR;
    }
    memset(node_handle->node_handle, 0, sizeof(NODE_ATTR_HANDLE) * data_cfg.list_count);

    node_handle->prev_node = list_handle->cur_node_handle;

    if(NULL != list_handle->cur_node_handle->next_node[prev_node_num])
    {
        node_handle->next_node[next_node_num] = list_handle->cur_node_handle->next_node[prev_node_num];
        list_handle->cur_node_handle->next_node[prev_node_num]->prev_node = \
        node_handle;
        list_handle->cur_node_handle->next_node[prev_node_num] = node_handle;
    }
    else
    {
        list_handle->cur_node_handle->next_node[prev_node_num] = node_handle;
    }

    list_handle->total_depth++;
    return COM_SUCCESS;
}

Sint32 ASLList_delet(HANDLE handle)
{
#ifdef ASL_COMMON_DEBUG
	assert_info(handle != NULL);
#endif
	COM_VERFY_NULL(NULL == handle);

    ASL_LIST_MNG_HANDLE list_handle = handle;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*end _cplusplus*/

