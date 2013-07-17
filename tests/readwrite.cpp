#include "tlbp-tests.h"
#include "tlbp.h"

TEST_WRITE(U8,  u8,  99);
TEST_WRITE(U16, u16, 99);
TEST_WRITE(U32, u32, 99);
TEST_WRITE(U64, u64, 99);

TEST_WRITE(S8,  s8,  99);
TEST_WRITE(S16, s16, 99);
TEST_WRITE(S32, s32, 99);
TEST_WRITE(S64, s64, 99);

TEST_READ(U8,  u8,  99);
TEST_READ(U16, u16, 99);
TEST_READ(U32, u32, 99);
TEST_READ(U64, u64, 99);

TEST_READ(S8,  s8,  99);
TEST_READ(S16, s16, 99);
TEST_READ(S32, s32, 99);
TEST_READ(S64, s64, 99);

TEST(WriteBU8, ReadWrite, 0,
{
  tlbpInitContext(&m_data.context);
  tlbpSetBuffer(m_data.context, m_data.buffer, 256);
},
{
  tlbpTerminateContext(&m_data.context);
},
{
  tlbpRewind(m_data.context);
  u8 limit = 0;
  ASSERT(tlbpWriteBU8(m_data.context, 99, &limit) == TLBP_SUCCESS);
  ASSERT(limit > 0);
},
{
  tlbpContext* context;
  u8           buffer[256];
}
);

TEST(ReadBU8, ReadWrite, 0,
{
  tlbpInitContext(&m_data.context);
  tlbpSetBuffer(m_data.context, m_data.buffer, 256);
  tlbpWriteBU8(m_data.context, 99, &m_data.limit);
},
{
  tlbpTerminateContext(&m_data.context);
},
{
  tlbpRewind(m_data.context);
  bu8 val = 0;
  ASSERT(tlbpReadBU8(m_data.context, &val, m_data.limit) == TLBP_SUCCESS);
  ASSERT(val == 99);
},
{
  tlbpContext* context;
  u8           buffer[256];
  u8           limit;
}
);