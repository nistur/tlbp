#pragma once
#ifndef __TLBP_H__
#define __TLBP_H__
#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#ifdef TLBP_DYNAMIC
# ifdef WIN32
#  ifdef TLBP_BUILD
#   define TLBP_EXPORT __declspec( dllexport )
#  else
#   define TLBP_EXPORT __declspec( dllimport )
#  endif
# endif
#endif
 
#ifndef TLBP_EXPORT
# define TLBP_EXPORT
#endif

#include "tlbp-types.h"

typedef int tlbpReturn;

typedef struct _tlbpContext tlbpContext;

#define TLBP_SUCCESS       0
#define TLBP_NO_CONTEXT    1
#define TLBP_OUT_OF_MEMORY 2
#define TLBP_NO_DATA       3

TLBP_EXPORT tlbpReturn   tlbpInitContext     (tlbpContext** context);
TLBP_EXPORT tlbpReturn   tlbpTerminateContext(tlbpContext** context);

TLBP_EXPORT tlbpReturn   tlbpSetBuffer       (tlbpContext* context, u8* buffer, u32 size);

TLBP_EXPORT tlbpReturn   tlbpRewind          (tlbpContext* context);
TLBP_EXPORT tlbpReturn   tlbpTell            (tlbpContext* context, u32* pos);

//------------------------------------------------------------------------------------------
// Write
//------------------------------------------------------------------------------------------
TLBP_EXPORT tlbpReturn   tlbpWriteU8         (tlbpContext* context, u8 val);
TLBP_EXPORT tlbpReturn   tlbpWriteU8Array    (tlbpContext* context, u8* vals, u32 num);
TLBP_EXPORT tlbpReturn   tlbpWriteU16        (tlbpContext* context, u16 val);
TLBP_EXPORT tlbpReturn   tlbpWriteU16Array   (tlbpContext* context, u16* vals, u32 num);
TLBP_EXPORT tlbpReturn   tlbpWriteU32        (tlbpContext* context, u32 val);
TLBP_EXPORT tlbpReturn   tlbpWriteU32Array   (tlbpContext* context, u32* vals, u32 num);
TLBP_EXPORT tlbpReturn   tlbpWriteU64        (tlbpContext* context, u64 val);
TLBP_EXPORT tlbpReturn   tlbpWriteU64Array   (tlbpContext* context, u64* vals, u32 num);
//------------------------------------------------------------------------------------------
TLBP_EXPORT tlbpReturn   tlbpWriteS8         (tlbpContext* context, s8 val);
TLBP_EXPORT tlbpReturn   tlbpWriteS8Array    (tlbpContext* context, s8* vals, u32 num);
TLBP_EXPORT tlbpReturn   tlbpWriteS16        (tlbpContext* context, s16 val);
TLBP_EXPORT tlbpReturn   tlbpWriteS16Array   (tlbpContext* context, s16* vals, u32 num);
TLBP_EXPORT tlbpReturn   tlbpWriteS32        (tlbpContext* context, s32 val);
TLBP_EXPORT tlbpReturn   tlbpWriteS32Array   (tlbpContext* context, s32* vals, u32 num);
TLBP_EXPORT tlbpReturn   tlbpWriteS64        (tlbpContext* context, s64 val);
TLBP_EXPORT tlbpReturn   tlbpWriteS64Array   (tlbpContext* context, s64* vals, u32 num);
//------------------------------------------------------------------------------------------
TLBP_EXPORT tlbpReturn   tlbpWriteBU8        (tlbpContext* context, bu8 val, u8* limit);
//------------------------------------------------------------------------------------------
TLBP_EXPORT tlbpReturn   tlbpReadBU8         (tlbpContext* context, bu8* val, u8 limit);
//------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------
// Read
//------------------------------------------------------------------------------------------
TLBP_EXPORT tlbpReturn   tlbpReadU8         (tlbpContext* context, u8* val);
TLBP_EXPORT tlbpReturn   tlbpReadU8Array    (tlbpContext* context, u8* vals, u32 num);
TLBP_EXPORT tlbpReturn   tlbpReadU16        (tlbpContext* context, u16* val);
TLBP_EXPORT tlbpReturn   tlbpReadU16Array   (tlbpContext* context, u16* vals, u32 num);
TLBP_EXPORT tlbpReturn   tlbpReadU32        (tlbpContext* context, u32* val);
TLBP_EXPORT tlbpReturn   tlbpReadU32Array   (tlbpContext* context, u32* vals, u32 num);
TLBP_EXPORT tlbpReturn   tlbpReadU64        (tlbpContext* context, u64* val);
TLBP_EXPORT tlbpReturn   tlbpReadU64Array   (tlbpContext* context, u64* vals, u32 num);
//------------------------------------------------------------------------------------------
TLBP_EXPORT tlbpReturn   tlbpReadS8         (tlbpContext* context, s8* val);
TLBP_EXPORT tlbpReturn   tlbpReadS8Array    (tlbpContext* context, s8* vals, u32 num);
TLBP_EXPORT tlbpReturn   tlbpReadS16        (tlbpContext* context, s16* val);
TLBP_EXPORT tlbpReturn   tlbpReadS16Array   (tlbpContext* context, s16* vals, u32 num);
TLBP_EXPORT tlbpReturn   tlbpReadS32        (tlbpContext* context, s32* val);
TLBP_EXPORT tlbpReturn   tlbpReadS32Array   (tlbpContext* context, s32* vals, u32 num);
TLBP_EXPORT tlbpReturn   tlbpReadS64        (tlbpContext* context, s64* val);
TLBP_EXPORT tlbpReturn   tlbpReadS64Array   (tlbpContext* context, s64* vals, u32 num);
//------------------------------------------------------------------------------------------

TLBP_EXPORT const char*  tlbpError();

#ifdef __cplusplus
}
#endif/*__cplusplus*/
#endif/*__TLBP_H__*/
