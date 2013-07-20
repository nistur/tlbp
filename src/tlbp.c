#include "tlbpInternal.h"

// for memcpy
#include <string.h>

tlbpReturn tlbpClearContext(tlbpContext* context)
{
    if(context == 0)
        tlbpReturn(NO_CONTEXT)

    context->buffer = 0;
    context->ptr    = 0;
    context->offset = 0;
    context->size   = 0;
    tlbpReturn(SUCCESS);
}

tlbpReturn tlbpPad(tlbpContext* context)
{
    if(context == NULL)
        tlbpReturn(NO_CONTEXT);
    if(context->offset == 0)
        tlbpReturn(SUCCESS); // nothing to pad
    u8 mask = ~((1<<8-context->offset)-1);
    *context->ptr &= mask;
    context->ptr += 1;
    context->offset = 0;
    tlbpReturn(SUCCESS);
}

tlbpReturn tlbpInitContext(tlbpContext** context)
{
    if(context == 0)
        tlbpReturn(NO_CONTEXT);
    *context = tlbpMalloc(tlbpContext);

    if(*context == 0)
        tlbpReturn(OUT_OF_MEMORY);

    if(tlbpClearContext(*context) != TLBP_SUCCESS)
    {
        tlbpTerminateContext(context);
        return g_tlbpError;
    }
    tlbpReturn(SUCCESS);
}

tlbpReturn tlbpTerminateContext(tlbpContext** context)
{
    if(*context == 0)
	tlbpReturn(NO_CONTEXT);

    tlbpFree(*context);
    *context = 0;
    tlbpReturn(SUCCESS);
}

tlbpReturn tlbpSetBuffer(tlbpContext* context, u8* buffer, u32 size)
{
    if(context == 0)
        tlbpReturn(NO_CONTEXT);

    if(buffer == 0 || size == 0)
        tlbpReturn(NO_DATA);

    tlbpClearContext(context);

    context->buffer = buffer;
    context->ptr    = buffer;
    context->size   = size;

    tlbpReturn(SUCCESS);
}

tlbpReturn tlbpRewind(tlbpContext* context)
{
    if(context == 0)
        tlbpReturn(NO_CONTEXT);
    context->ptr = context->buffer;
    context->offset = 0;
    tlbpReturn(SUCCESS);
}

tlbpReturn tlbpTell(tlbpContext* context, u32* pos)
{
    if(context == NULL)
        tlbpReturn(NO_CONTEXT);
    if(pos == NULL)
        tlbpReturn(NO_DATA);

    *pos = context->ptr - context->buffer;

    tlbpReturn(SUCCESS);
}

TLBP_LIMIT_IMPLEMENT(U8,  u8);
TLBP_LIMIT_IMPLEMENT(U16, u16);
TLBP_LIMIT_IMPLEMENT(U32, u32);
TLBP_LIMIT_IMPLEMENT(U64, u64);

tlbpReturn tlbpWrite(tlbpContext* context, void* data, u32 size)
{
    if(context == NULL)
        tlbpReturn(NO_CONTEXT);
    if(context->buffer == NULL || data == NULL || size == 0)
        tlbpReturn(NO_DATA);
    if((context->size - (context->ptr - context->buffer)) < size)
        tlbpReturn(OUT_OF_MEMORY);

    memcpy(context->ptr, data, size);
    context->ptr += size;

    tlbpReturn(SUCCESS);
}

tlbpReturn tlbpWriteBits(tlbpContext* context, void* val, u8 limit)
{
    if(context == NULL)
        tlbpReturn(NO_CONTEXT);
    if(val == NULL)
        tlbpReturn(NO_DATA);

    u8* src = (u8*)val;
    u8 offset = context->offset;
    while(limit > 0)
    {
        u8 count = (8 - context->offset > limit) ? limit : 8 - context->offset;

        u8 preMask = (-1 ^ ((1<<(8- context->offset))-1));
        u8 postMask = (1<<(8- context->offset-count))-1;

        *context->ptr = (*context->ptr & (preMask | postMask)) | // copy original contents
                        (((*src >> offset) | (*(src-1) << 8 - offset)) & (~preMask & ~postMask));
        // in theory this could copy from garbage before src[0] but the mask should solve problems

        limit -= count;
        context->offset += count;
        while(context->offset >= 8)
        {
            context->offset -= 8;
            context->ptr += 1;
            src += 1;
        }
    }

    tlbpReturn(SUCCESS);
}

tlbpReturn tlbpReadBits(tlbpContext* context, void* val, u8 limit)
{
    if(context == NULL)
        tlbpReturn(NO_CONTEXT);
    if(val == NULL)
        tlbpReturn(NO_DATA);

    u8* dst = (u8*)val;
    while(limit > 0)
    {
        u8 count = MIN(8-context->offset, limit);
        *dst <<= count;
        //u8 preMask = (-1 ^ ((1<<(8-context->offset))-1));
        //u8 postMask = (1<<(8-(context->offset + count)))-1;

        *dst = *context->ptr << context->offset |
                *(context->ptr+1) >> (8-context->offset);
        
        context->offset += MIN(8, count);
        while(context->offset >= 8)
        {
            context->offset -= 8;
            context->ptr += 1;
            dst += 1;
        }
        limit -= count;
    }

    tlbpReturn(SUCCESS);
}

tlbpReturn tlbpRead(tlbpContext* context, void* data, u32 size)
{
    if(context == NULL)
        tlbpReturn(NO_CONTEXT);
    if(context->buffer == NULL || data == NULL || size == 0)
        tlbpReturn(NO_DATA);
    if((context->size - (context->ptr - context->buffer)) < size)
        tlbpReturn(OUT_OF_MEMORY);

    memcpy(data, context->ptr, size);
    context->ptr += size;

    tlbpReturn(SUCCESS);
}

tlbpReturn tlbpWriteUBits(tlbpContext* context, void* val, u8 size, u8 limit)
{
    if(context == NULL)
        tlbpReturn(NO_CONTEXT);
    if(val == NULL)
        tlbpReturn(NO_DATA);

    u8 reqSize = ((limit / 8)*8 == limit) ? limit/8 : 1+(limit / 8);
    u8* tmp = tlbpMallocArray(u8, reqSize);
    u8 dSize = size - reqSize;
    u8 shift = reqSize*8 - limit;

    for(int i = 0; i < reqSize-1; ++i)
        tmp[i] = ((u8*)val)[i] << shift |
                ((u8*)val)[i+1] >> 8 - shift;
    tmp[reqSize-1] = ((u8*)val)[reqSize-1] << shift;


    tlbpWriteBits(context, tmp, limit);

    tlbpFree(tmp);
    return g_tlbpError;
}

tlbpReturn tlbpReadUBits(tlbpContext* context, void* val, u8 size, u8 limit)
{
    if(context == NULL)
        tlbpReturn(NO_CONTEXT);
    if(val == NULL)
        tlbpReturn(NO_DATA);

    u8 reqSize = ((limit / 8)*8 == limit) ? limit/8 : 1+(limit / 8);
    u8 dSize = size - reqSize;
    u8 shift = reqSize*8 - limit;

    memset(val, 0, size);
    u8* tmp = tlbpMallocArray(u8, reqSize);
    if(tlbpReadBits(context, tmp, limit) != TLBP_SUCCESS)
    {
        tlbpFree(tmp);
        return g_tlbpError;
    }

    for(int i = 1; i < reqSize; ++i)
        ((u8*)val)[i] = tmp[i] >> shift |
                              tmp[i-1] << 8-shift;
    ((u8*)val)[0] = tmp[0] >> shift;

    tlbpFree(tmp);
    tlbpReturn(SUCCESS);
}

TLBP_WRITE_IMPLEMENT      (U8,  u8);
TLBP_WRITE_ARRAY_IMPLEMENT(U8,  u8);
TLBP_WRITE_IMPLEMENT      (U16, u16);
TLBP_WRITE_ARRAY_IMPLEMENT(U16, u16);
TLBP_WRITE_IMPLEMENT      (U32, u32);
TLBP_WRITE_ARRAY_IMPLEMENT(U32, u32);
TLBP_WRITE_IMPLEMENT      (U64, u64);
TLBP_WRITE_ARRAY_IMPLEMENT(U64, u64);

TLBP_WRITE_IMPLEMENT      (S8,  s8);
TLBP_WRITE_ARRAY_IMPLEMENT(S8,  s8);
TLBP_WRITE_IMPLEMENT      (S16, s16);
TLBP_WRITE_ARRAY_IMPLEMENT(S16, s16);
TLBP_WRITE_IMPLEMENT      (S32, s32);
TLBP_WRITE_ARRAY_IMPLEMENT(S32, s32);
TLBP_WRITE_IMPLEMENT      (S64, s64);
TLBP_WRITE_ARRAY_IMPLEMENT(S64, s64);

TLBP_READ_IMPLEMENT      (U8,  u8);
TLBP_READ_ARRAY_IMPLEMENT(U8,  u8);
TLBP_READ_IMPLEMENT      (U16, u16);
TLBP_READ_ARRAY_IMPLEMENT(U16, u16);
TLBP_READ_IMPLEMENT      (U32, u32);
TLBP_READ_ARRAY_IMPLEMENT(U32, u32);
TLBP_READ_IMPLEMENT      (U64, u64);
TLBP_READ_ARRAY_IMPLEMENT(U64, u64);

TLBP_READ_IMPLEMENT      (S8,  s8);
TLBP_READ_ARRAY_IMPLEMENT(S8,  s8);
TLBP_READ_IMPLEMENT      (S16, s16);
TLBP_READ_ARRAY_IMPLEMENT(S16, s16);
TLBP_READ_IMPLEMENT      (S32, s32);
TLBP_READ_ARRAY_IMPLEMENT(S32, s32);
TLBP_READ_IMPLEMENT      (S64, s64);
TLBP_READ_ARRAY_IMPLEMENT(S64, s64);

TLBP_WRITE_UBITS_IMPLEMENT(BU8,  bu8);
TLBP_WRITE_UBITS_IMPLEMENT(BU16, bu16);
TLBP_WRITE_UBITS_IMPLEMENT(BU32, bu32);
TLBP_WRITE_UBITS_IMPLEMENT(BU64, bu64);

TLBP_READ_UBITS_IMPLEMENT(BU8,  bu8);
TLBP_READ_UBITS_IMPLEMENT(BU16, bu16);
TLBP_READ_UBITS_IMPLEMENT(BU32, bu32);
TLBP_READ_UBITS_IMPLEMENT(BU64, bu64);

const char* tlbpError()
{
    return g_tlbpErrors[g_tlbpError];
}
