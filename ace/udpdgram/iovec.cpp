#include <ace/Log_Msg.h>
#include <ace/Signal.h>
#include <ace/Reactor.h>
#include <ace/Event_Handler.h>
#include <ace/Thread.h>
#include <ace/SOCK_Dgram.h>






int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P | %t hello world for ACE , thead id: %u ...\n"), ACE_Thread::self()));
 
  ACE_INET_Addr local(12701);
 
  ACE_SOCK_Dgram udp(local);
 
  
  udp.get_local_addr(local);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P udp bind address: %s, port: %u ...\n"), local.get_host_addr(), local.get_port_number()));
  
  udp.enable(ACE_NONBLOCK);

  iovec iov[2];
  iov[0].iov_base = reinterpret_cast<void*>(const_cast<char*>("hello world 1"));
  iov[0].iov_len  = ACE_OS::strlen("hello world 1");
  iov[1].iov_base = reinterpret_cast<void*>(const_cast<char*>("hello world 2"));;
  iov[1].iov_len  = ACE_OS::strlen("hello world 2");

  ACE_DEBUG((LM_DEBUG, ACE_TEXT ("%D %M %P udp send iov_len0: %u,  iov_len1: %u ...\n"), iov[0].iov_len, iov[1].iov_len));
  ACE_INET_Addr to(local.get_port_number(), "127.0.0.1");
  ssize_t len = udp.send(iov, 2, to);
  
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P udp send %d bytes, but only one pkg ...\n"), len));

  iovec recviov[2];
  char buf0[256] = {}, buf1[256] = {};
  recviov[0].iov_base = buf0;
  recviov[0].iov_len  = sizeof(buf0);
  recviov[1].iov_base = buf1;
  recviov[1].iov_len  = sizeof(buf1);

  ACE_OS::sleep(1);

  ACE_INET_Addr remote;
  ACE_INET_Addr localTo;
  len = udp.recv(recviov, 2, remote, 0, &localTo);
  if(len < 0)
  {
     ACE_DEBUG ((LM_ERROR, ACE_TEXT ("%D %M %P udp recv in failure ! errno: %d --> %s\n"), ACE_OS::last_error(), ACE_OS::strerror(ACE_OS::last_error()))); 
  }
  else
  {
     ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P udp recv %d byte iovec to address: %s, port: %u ; iov_len0: %u,  iov_len1: %u ...\nrecvmsg0: %s, recvmsg1: %s\n"), 
     len, localTo.get_host_addr(), localTo.get_port_number(), 
     recviov[0].iov_len, recviov[1].iov_len,
     recviov[0].iov_base, recviov[1].iov_base));

     ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P udp recv iovec only a pkg and fill the buffer in sequnce ...\n")));
  }

  
  
  
  ACE_Reactor* const r = ACE_Reactor::instance();
    
  while (!r->reactor_event_loop_done())
  {
     r->run_reactor_event_loop();
  }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D %M %P shutting down server logging daemon\n")));
  return 0;
}



