#include <ace/Log_Msg.h>
#include <ace/Signal.h>
#include <ace/Reactor.h>
#include <ace/Event_Handler.h>
#include <ace/Thread.h>
#include <ace/Stack_Trace.h>

#include <execinfo.h>



class MyCTestSignal: public ACE_Event_Handler {
 public:
  virtual int 	handle_signal (int signum, siginfo_t* =0, ucontext_t* = 0);
};


class MyCTestTimer: public ACE_Event_Handler {
 public:
  virtual int 	handle_timeout (const ACE_Time_Value &current_time, const void *act=0);
};


int MyCTestTimer::handle_timeout (const ACE_Time_Value &current_time, const void *act)
{

  ACE_Stack_Trace st(-2);
  
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P | %t thead id: %u timer out ...\n%s"),  ACE_Thread::self(), st.c_str()));
  

  return 0;
}

void dumpStackUseBacktrace()
{

    #define BT_BUF_SIZE 100
    void *buffer[BT_BUF_SIZE];


    int nptrs = backtrace(buffer, BT_BUF_SIZE);

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P | %t thead id: %u backtrace() returned %d addresses ...\n\n"),  ACE_Thread::self(), nptrs));

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
    would produce similar output to the following: */

    char** strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        ACE_Thread::exit();
    }

    for (int j = 0; j < nptrs; j++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s\n"),  strings[j]));
    }

    free(strings);

}

int MyCTestSignal::handle_signal (int signum, siginfo_t *, ucontext_t *)
{


    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P | %t thead id: %u recv signal : %d ...\n"),  ACE_Thread::self(), signum));
  

    dumpStackUseBacktrace();
    
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n%D %M %P | %t thead id: %u exit from signal handler ...\n"),  ACE_Thread::self()));

    ACE_Thread::exit();
   
   return 0;
}


void produceFPEException()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P | %t child thead id: %u,  %s begin to run ...\n"), ACE_Thread::self(), __func__));
  int i = 0, j = 0;
  i = i/j;
}

void produceSigSEGVException()
{
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P | %t child thead id: %u,  %s begin to run ...\n"), ACE_Thread::self(), __func__));
    int i = 1;
    i = *(int*)0;
}


ACE_THR_FUNC_RETURN childThreadFunc(void * arg)
{

  int para = (int)arg;
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P | %t child thead id: %u, para: %d begin to run ...\n"), ACE_Thread::self(), para));
  
  if(para == 0)
  {
    produceFPEException();
  }
  else
  {
     
    produceSigSEGVException(); 
  }
 
  return (ACE_THR_FUNC_RETURN)NULL;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{


  MyCTestSignal mySignalHandler;
  MyCTestTimer  myTimerHandler;
  
  ACE_Reactor* const r = ACE_Reactor::instance();
  
  r->register_handler (SIGFPE,  &mySignalHandler);
  r->register_handler (SIGSEGV, &mySignalHandler);
  
  ACE_Time_Value delay(5);
  r->schedule_timer (&myTimerHandler, NULL, delay);

 
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P | %t hello world for ACE , thead id: %u ...\n"), ACE_Thread::self()));

 
 
 
  ACE_Thread::spawn(childThreadFunc, (void*)0);
  
  ACE_OS::sleep(1);
     
  ACE_Thread::spawn(childThreadFunc, (void*)1);

  	
  
  while (!r->reactor_event_loop_done())
   {
     r->run_reactor_event_loop();
   }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P | %t shutting down server logging daemon\n")));
  return 0;
}



