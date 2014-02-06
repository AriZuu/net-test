/*
 * Copyright (c) 2006-2013, Ari Suutari <ari@stonepile.fi>.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <picoos.h>
#include <picoos-u.h>
#include <picoos-net.h>
#include "net-test.h"

static struct uip_eth_addr ethaddr = {

  { 0x00, 0xbd, 0x3b, 0x33, 0x04, 0xd5 }
};

static int acceptHook(NetSock* sock, int lport)
{
  POSTASK_t task;

  task = posTaskCreate(shellTask, (void*)sock, 2, SHELL_STACK_SIZE);
  if (task == NULL) {

#if NOSCFG_FEATURE_CONOUT == 1
    nosPrint("net: out of tasks.");
#endif
    return -1;
  }

  POS_SETTASKNAME(task, "shell");
  return 0;
}

void initNetwork()
{
#if NOSCFG_FEATURE_CONOUT == 1
  nosPrint("Starting network.\n");
#endif

  uip_setethaddr(ethaddr);

#if !UIP_CONF_IPV6

  uip_ipaddr_t ipaddr;

#ifdef unix

  uip_ipaddr(&ipaddr, 192, 168, 0, 2);
  uip_sethostaddr(&ipaddr);

  uip_ipaddr(&ipaddr, 192, 168, 0, 1);
  uip_setdraddr(&ipaddr);

#else

  uip_ipaddr(&ipaddr, 192, 168, 60, 213);
  uip_sethostaddr(&ipaddr);

  uip_ipaddr(&ipaddr, 192, 168, 60, 1);
  uip_setdraddr(&ipaddr);

#endif

  uip_ipaddr(&ipaddr, 255, 255, 255, 0);
  uip_setnetmask(&ipaddr);
#endif

  netInit();
  netSockAcceptHookSet(acceptHook);

  uip_listen(uip_htons(23));
}

void mainTask(void *arg)
{
  uosBootDiag();

#if NOSCFG_FEATURE_CONOUT == 1
  nosPrint("Main task startup.\n");
#endif

  initNetwork();
  while (true)
    posTaskSleep(MS(10000));
}



