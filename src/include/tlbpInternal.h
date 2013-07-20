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
#define tlbpMallocArray(x, size) (x*)malloc(sizeof(x)*size)
#define tlbpFree(x)   if(x) { free(x); x = 0; }

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


#define MAX(a, b) ( a > b ? a : b)
#define MIN(a, b) ( a < b ? a : b)



#define TLBP_LIMIT_IMPLEMENT(x, type)                   \
u8 tlbpLimit##x##Internal(type val)                     \
{                                                       \
    u8 size = 8*sizeof(type);                           \
    u8 lim;                                             \
    for(lim = size-1; lim > 0; --lim)                   \
        if(val & (1 << lim))                            \
            break;                                      \
    return lim + 1;                                     \
}                                                       \
tlbpReturn tlbpLimit##x(type* vals, u32 num, u8* lim)   \
{                                                       \
    if(vals == NULL || lim == NULL || num == 0)         \
        tlbpReturn(NO_DATA);                            \
    *lim = 0;                                           \
    for(u32 i = 0; i < num; ++i)                        \
    {                                                   \
        u8 l = tlbpLimit##x##Internal(vals[i]);         \
        if(l > *lim)                                    \
            *lim = l;                                   \
    }                                                   \
    tlbpReturn(SUCCESS);                                \
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

#define TLBP_WRITE_UBITS_IMPLEMENT(x, type)                         \
tlbpReturn tlbpWrite##x(tlbpContext* context, type val, u8* limit)  \
{                                                                   \
    if(context == NULL)                                             \
        tlbpReturn(NO_CONTEXT);                                     \
    u8 lim = limit ? *limit : 0;                                    \
    if(lim == 0)                                                    \
    {                                                               \
        for(lim = 7; lim >= 0; --lim)                               \
            if(lim == 0 || val & (1<<lim))                          \
                break;                                              \
        /* change from indices to numbers */                        \
        lim += 1;                                                   \
    }                                                               \
    /* check for out of memory */                                   \
    u32 pos;                                                        \
    tlbpTell(context, &pos);                                        \
    if((context->size - pos) < sizeof(type))                        \
        tlbpReturn(OUT_OF_MEMORY);                                  \
    tlbpWriteUBits(context, &val, sizeof(type), lim);               \
    if(limit) *limit = lim;                                         \
    return g_tlbpError;                                             \
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

#define TLBP_READ_UBITS_IMPLEMENT(x, type)                          \
tlbpReturn tlbpRead##x(tlbpContext* context, type* val, u8 limit)   \
{                                                                   \
    if(context == NULL)                                             \
        tlbpReturn(NO_CONTEXT);                                     \
    if(val == NULL)                                                 \
        tlbpReturn(NO_DATA);                                        \
    /* check for out of memory */                                   \
    u32 pos;                                                        \
    tlbpTell(context, &pos);                                        \
    if((context->size - pos) < sizeof(type))                        \
        tlbpReturn(OUT_OF_MEMORY);                                  \
    *val = 0;                                                       \
    return tlbpReadUBits(context, val, sizeof(type), limit);        \
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
