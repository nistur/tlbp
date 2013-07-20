#ifndef __TLBP_TESTS_H__
#define __TLBP_TESTS_H__

#include "testsuite/tests.h"

#define TEST_TYPE(x, size)\
TEST(x, Types, 0.0f, {}, {}, { ASSERT(sizeof(x) == size/8); }, {} );

#define TEST_LIMIT(x, type)                                           \
TEST(Limit##x, Types, 0.0f, {},{},                                    \
     {                                                                \
          for(u8 i = 0; i < sizeof(type) * 8; ++i)                    \
          {                                                           \
               type test = 1 << i;                                    \
               u8 lim = 0;                                            \
               ASSERT(tlbpLimit##x(&test, 1, &lim) == TLBP_SUCCESS);  \
               ASSERT(lim == i+1);                                    \
          }                                                           \
     },                                                               \
     {}                                                               \
    );                                                                \
TEST(LimitList##x, Types, 0.0f, {},{},                                \
     {                                                                \
          u8 num = rand() % (sizeof(type) * 8);                       \
          type vals[64];                                              \
          for(u8 i = 0; i < num; ++i)                                 \
               vals[i] = 1 << i;                                      \
          u8 lim = 0;                                                 \
          ASSERT(tlbpLimit##x(vals, num, &lim) == TLBP_SUCCESS);      \
          ASSERT(lim == num);                                         \
     },                                                               \
     {}                                                               \
    );

#define TEST_WRITE(x, type, val)													\
TEST(Write##x, ReadWrite, 0.0f,														\
     {																				\
      tlbpInitContext(&m_data.context);												\
      tlbpSetBuffer(m_data.context, m_data.buffer, 256);							\
     },																				\
     {																				\
      tlbpTerminateContext(&m_data.context);										\
     },																				\
     {																				\
          tlbpRewind(m_data.context);												\
          ASSERT(tlbpWrite##x(m_data.context, val) == TLBP_SUCCESS);				\
          																			\
          /* make sure we're actually adding to the buffer */						\
          u32 pos;																	\
          tlbpTell(m_data.context, &pos);											\
          ASSERT(pos == sizeof(type));												\
          																			\
          type array[3];															\
          ASSERT(tlbpWrite##x##Array(m_data.context, array, 3) == TLBP_SUCCESS);	\
          ASSERT(tlbpWrite##x##Array(m_data.context, array, 0) == TLBP_NO_DATA);	\
          ASSERT(tlbpWrite##x##Array(m_data.context, 0, 0) == TLBP_NO_DATA);		\
     },																				\
     {																				\
      tlbpContext* context;															\
      u8           buffer[256];														\
     }																				\
    );

#define TEST_READ(x, type, val)														\
TEST(Read##x, ReadWrite, 0,															\
  {																					\
      tlbpInitContext(&m_data.context);												\
      tlbpSetBuffer(m_data.context, m_data.buffer, 256);							\
      tlbpWrite##x(m_data.context, val);											\
  },																				\
  {																					\
      tlbpTerminateContext(&m_data.context);										\
  },																				\
  {																					\
      tlbpRewind(m_data.context);													\
      type test = 0;																\
      ASSERT(tlbpRead##x(m_data.context, &test) == TLBP_SUCCESS);					\
      ASSERT(test == val);															\
  },																				\
  {																					\
    u8           buffer[256];														\
    tlbpContext* context;															\
  }																					\
);

#define TEST_WRITE_UBITS(x, type, val)                              \
TEST(Write##x, ReadWrite, 0,                                        \
{                                                                   \
  tlbpInitContext(&m_data.context);                                 \
  tlbpSetBuffer(m_data.context, m_data.buffer, 256);                \
},                                                                  \
{                                                                   \
  tlbpTerminateContext(&m_data.context);                            \
},                                                                  \
{                                                                   \
  tlbpRewind(m_data.context);                                       \
  u8 limit = 0;                                                     \
  ASSERT(tlbpWrite##x(m_data.context, val, &limit) == TLBP_SUCCESS);\
  ASSERT(limit > 0);                                                \
},                                                                  \
{                                                                   \
  tlbpContext* context;                                             \
  u8           buffer[256];                                         \
}                                                                   \
);

#define TEST_READ_UBITS(x, type, lim)                                         \
TEST(Read##x, ReadWrite, 0,                                                   \
{                                                                             \
  tlbpInitContext(&m_data.context);                                           \
  tlbpSetBuffer(m_data.context, m_data.buffer, 256);                          \
  m_data.limit = lim;                                                         \
  for(type i = 0; i < 10; ++i)                                                \
    tlbpWrite##x(m_data.context, i, &m_data.limit);                           \
},                                                                            \
{                                                                             \
  tlbpTerminateContext(&m_data.context);                                      \
},                                                                            \
{                                                                             \
  tlbpRewind(m_data.context);                                                 \
  type val = 0;                                                               \
  for(type i = 0; i < 10; ++i)                                                \
  {                                                                           \
    ASSERT(tlbpRead##x(m_data.context, &val, m_data.limit) == TLBP_SUCCESS);  \
    ASSERT(val == i);                                                         \
  }                                                                           \
},                                                                            \
{                                                                             \
  tlbpContext* context;                                                       \
  u8           buffer[256];                                                   \
  u8           limit;                                                         \
}                                                                             \
);

#endif/*__TLBP_TESTS_H__*/
