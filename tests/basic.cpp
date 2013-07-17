#include "tlbp-tests.h"
#include "tlbp.h"

TEST(InitTerminate, Basic, 0.0f,
     // initialisation
     {
	 m_data.context = 0;
     },
     // cleanup
     {
	 tlbpTerminateContext(&m_data.context);
     },
     // test
     {
	 ASSERT(tlbpInitContext(&m_data.context) == TLBP_SUCCESS);
	 ASSERT(tlbpTerminateContext(&m_data.context) == TLBP_SUCCESS);
     },
     // data
     {
	 tlbpContext* context;
     }
    );

TEST(SetBuffer, Basic, 0.0f,
     // initialisation
     {
      tlbpInitContext(&m_data.context);
     },
     // cleanup
     {
      tlbpTerminateContext(&m_data.context);
     },
     // test
     {
      ASSERT(tlbpSetBuffer(m_data.context, m_data.buffer, 256) == TLBP_SUCCESS);
      ASSERT(tlbpSetBuffer(m_data.context, 0, 0) == TLBP_NO_DATA);
     },
     // data
     {
      tlbpContext* context;
      u8           buffer[256];
     }
    );
