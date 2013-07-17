#ifndef __TLBP_TESTS_H__
#define __TLBP_TESTS_H__

#include "testsuite/tests.h"

#define TEST_TYPE(x, size)\
TEST(x, Types, 0.0f, {}, {}, { ASSERT(sizeof(x) == size/8); }, {} );

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

#endif/*__TLBP_TESTS_H__*/
