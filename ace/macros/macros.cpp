#include <ace/Log_Msg.h>
#include <ace/Thread.h>



int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P hello world for ACE , thead id: %u ...\n")));

  #ifdef  __ACE_INLINE__
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P ACE's app compiling __ACE_INLINE__ macro is configured!\n")));
  #else
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P ACE's app compiling __ACE_INLINE__ macro is not configured!\n")));
  #endif


  return 0;
}



