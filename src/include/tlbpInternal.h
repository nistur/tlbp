#pragma once
#ifndef __TLBP_INTERNAL_H__
#define __TLBP_INTERNAL_H__

#include "tlbp.h"

/***************************************
 * Library context
 * - holds current state
 ***************************************/
struct _tlbpContext
{
	u8*   buffer; //-- current active buffer
	u8*   ptr;    //-- the current byte
	u8    offset; //-- offset within the byte
	u32   size;   //-- size of the buffer
};

/***************************************
 * Some basic memory management wrappers
 ***************************************/
#include <stdlib.h>
#define tlbpMalloc(x) (x*)malloc(sizeof(x))
#define tlbpFree(x)   free(x)

/***************************************
 * Error handling
 ***************************************/
extern tlbpReturn  g_tlbpError;
extern const char* g_tlbpErrors[];
#define tlbpReturn(x)				\
    {						\
	g_tlbpError = TLBP_##x;			\
	return TLBP_##x;			\
    }



#define TLBP_WRITE_IMPLEMENT(x, type)                           \
tlbpReturn tlbpWrite##x(tlbpContext* context, type val)         \
{                                                               \
    if(context == NULL)                                         \
        tlbpReturn(NO_CONTEXT);                                 \
                                                                \
    tlbpPad(context);                                           \
                                                                \
    if(tlbpWrite(context, &val, sizeof(type)) != TLBP_SUCCESS)  \
        return g_tlbpError;                                     \
    tlbpReturn(SUCCESS);                                        \
}

#define TLBP_WRITE_ARRAY_IMPLEMENT(x, type)                                 \
tlbpReturn tlbpWrite##x##Array(tlbpContext* context, type* vals, u32 size)  \
{                                                                           \
    if(context == NULL)                                                     \
        tlbpReturn(NO_CONTEXT);                                             \
    if(vals == NULL || size == 0)                                           \
        tlbpReturn(NO_DATA);                                                \
                                                                            \
    tlbpPad(context);                                                       \
    if(tlbpWrite(context, vals, sizeof(type) * size) != TLBP_SUCCESS)       \
        return g_tlbpError;                                                 \
    tlbpReturn(SUCCESS);                                                    \
}


#define TLBP_READ_IMPLEMENT(x, type)                            \
tlbpReturn tlbpRead##x(tlbpContext* context, type* val)         \
{                                                               \
    if(context == NULL)                                         \
        tlbpReturn(NO_CONTEXT);                                 \
                                                                \
    tlbpPad(context);                                           \
                                                                \
    if(tlbpRead(context, val, sizeof(type)) != TLBP_SUCCESS)    \
        return g_tlbpError;                                     \
    tlbpReturn(SUCCESS);                                        \
}

#define TLBP_READ_ARRAY_IMPLEMENT(x, type)                                  \
tlbpReturn tlbpRead##x##Array(tlbpContext* context, type* vals, u32 size)   \
{                                                                           \
    if(context == NULL)                                                     \
        tlbpReturn(NO_CONTEXT);                                             \
    if(vals == NULL || size == 0)                                           \
        tlbpReturn(NO_DATA);                                                \
                                                                            \
    tlbpPad(context);                                                       \
    if(tlbpRead(context, vals, sizeof(type) * size) != TLBP_SUCCESS)        \
        return g_tlbpError;                                                 \
    tlbpReturn(SUCCESS);                                                    \
}

#endif/*__TLBP_INTERNAL_H__*/
