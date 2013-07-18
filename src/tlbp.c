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

tlbpReturn tlbpWriteBU8(tlbpContext* context, bu8 val, u8* limit)
{
    if(context == NULL)
        tlbpReturn(NO_CONTEXT);

    u8 lim = limit ? *limit : 0;

    if(lim == 0)
    {
        for(lim = 7; lim >= 0; --lim)
            if(lim == 0 || val & (1<<lim))
                break;
        // change from indices to numbers
        lim += 1;
    }

    // check for out of memory
    u32 pos;
    tlbpTell(context, &pos);
    if((context->size - pos) < sizeof(bu8))
        tlbpReturn(OUT_OF_MEMORY);

    // shift away the first bits, they're all 0 anyway
    val <<= 8 - lim;

    // first byte
    bu8 curr_mask = (-1 ^ ((1<<(8-context->offset))-1));
    *context->ptr = (*context->ptr & curr_mask) | val >> context->offset;

    // second byte
    // we don't care about any trailing 0s
    context->ptr[1] = val << 8-context->offset;

    // add on the amount to the offset
    context->offset += lim;
    while(context->offset >= 8)
    {
        context->offset -= 8;
        context->ptr += 1;
    }
    if(limit) *limit = lim;

    tlbpReturn(SUCCESS);
}
    
tlbpReturn tlbpReadBU8(tlbpContext* context, bu8* val, u8 limit)
{
    if(context == NULL)
        tlbpReturn(NO_CONTEXT);
    if(val == NULL)
        tlbpReturn(NO_DATA);
    // check for out of memory
    u32 pos;
    tlbpTell(context, &pos);
    if((context->size - pos) < sizeof(bu8))
        tlbpReturn(OUT_OF_MEMORY);

    // how much of this byte
    *val = 0;
    u8 prev = 0;
    while(limit > 0)
    {
        *val <<= prev;
        u8 amount = (8 - context->offset) > limit ? limit : 8 - context->offset;
        u8 pre_mask = (-1 ^ ((1<<(8-context->offset))-1));
        u8 post_mask = (1<<(8-(context->offset + limit)))-1;

        u8 byte = *context->ptr & (~pre_mask & ~post_mask);
        byte >>= 8-(context->offset + limit);
        *val |= byte;
        
        context->offset += amount;
        while(context->offset >= 8)
        {
            context->offset -= 8;
            context->ptr += 1;
        }
        limit -= amount;
        prev = amount;
    }

    tlbpReturn(SUCCESS);
}

const char* tlbpError()
{
    return g_tlbpErrors[g_tlbpError];
}
