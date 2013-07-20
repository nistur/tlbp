#include "tlbp-tests.h"
#include "tlbp.h"

TEST_WRITE(U8,  u8,  99);
TEST_WRITE(U16, u16, 99);
TEST_WRITE(U32, u32, 99);
//TEST_WRITE(U64, u64, 99);

TEST_WRITE(S8,  s8,  99);
TEST_WRITE(S16, s16, 99);
TEST_WRITE(S32, s32, 99);
//TEST_WRITE(S64, s64, 99);

TEST_READ(U8,  u8,  99);
TEST_READ(U16, u16, 99);
TEST_READ(U32, u32, 99);
//TEST_READ(U64, u64, 99);

TEST_READ(S8,  s8,  99);
TEST_READ(S16, s16, 99);
TEST_READ(S32, s32, 99);
//TEST_READ(S64, s64, 99);

TEST_WRITE_UBITS(BU8,  bu8,  99);
TEST_WRITE_UBITS(BU16, bu16, 99);
TEST_WRITE_UBITS(BU32, bu32, 99);
//TEST_WRITE_UBITS(BU64, bu64, 99);

TEST_READ_UBITS(BU8,  bu8,  5);
TEST_READ_UBITS(BU16, bu16, 9);
TEST_READ_UBITS(BU32, bu32, 9);
//TEST_READ_UBITS(BU64, bu64, 27);