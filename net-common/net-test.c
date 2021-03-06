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
#include <stddef.h>
#include <sys/socket.h>
#include "net-test.h"

#ifdef TESTCFG_DHCP
#include "net/ip/dhcpc.h"
#endif

#ifndef TESTCFG_REAL_ETHADDR
static struct uip_eth_addr ethaddr = {

  { 0x00, 0xbd, 0x3b, 0x33, 0x04, 0xd5 }
};
#endif

#if NETCFG_BSD_SOCKETS != 1
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
#endif

void initNetwork()
{
#if NOSCFG_FEATURE_CONOUT == 1
  nosPrint("Starting network.\n");
#endif

#ifndef TESTCFG_REAL_ETHADDR
  uip_setethaddr(ethaddr);
#endif

#if !UIP_CONF_IPV6

#ifndef TESTCFG_DHCP

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
#endif

  netInit();

#ifdef TESTCFG_DHCP

  dhcpc_init(&uip_lladdr, sizeof(uip_lladdr));
  dhcpc_request();

#endif
}

#ifdef TESTCFG_DHCP
void dhcpc_configured(const struct dhcpc_state *s)
{
  nosPrintf("Got IP address %d.%d.%d.%d\n", uip_ipaddr_to_quad(&s->ipaddr));
  nosPrintf("Got netmask %d.%d.%d.%d\n", uip_ipaddr_to_quad(&s->netmask));
  nosPrintf("Got DNS server %d.%d.%d.%d\n", uip_ipaddr_to_quad(&s->dnsaddr));
  nosPrintf("Got default router %d.%d.%d.%d\n", uip_ipaddr_to_quad(&s->default_router));
  nosPrintf("Lease expires in %ld seconds\n", uip_ntohs(s->lease_time[0])*65536ul + uip_ntohs(s->lease_time[1]));

  uip_sethostaddr(&s->ipaddr);
  uip_setnetmask(&s->netmask);
  uip_setdraddr(&s->default_router);
}

void dhcpc_unconfigured(const struct dhcpc_state *s)
{
  nosPrintf("DHCP lease lost.\n");
  uip_ipaddr_t ipaddr;

  uip_ipaddr(&ipaddr, 0,0,0,0);
  uip_sethostaddr(&ipaddr);
}
#endif

#if UIP_CONF_IPV6 && NETCFG_BSD_SOCKETS
static struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;
#endif

void mainTask(void *arg)
{
  uosInit();
  uosBootDiag();

#if NOSCFG_FEATURE_CONOUT == 1
  nosPrint("Main task startup.\n");
#endif

  initNetwork();
#if NETCFG_BSD_SOCKETS == 1

  int lsn;
  socklen_t addrlen;
#if UIP_CONF_IPV6

  struct sockaddr_in6 me;
  struct sockaddr_in6 peer;

  me.sin6_family = AF_INET6;
  me.sin6_addr = in6addr_any;
  me.sin6_port = htons(23);

#else

  struct sockaddr_in me;
  struct sockaddr_in peer;

  me.sin_family = AF_INET;
  me.sin_addr.s_addr = INADDR_ANY;
  me.sin_port = htons(23);

#endif

  lsn = socket(AF_INET, SOCK_STREAM, 0);
  
  bind(lsn, (struct sockaddr*)&me, sizeof(me));
  listen(lsn, 5);

  while (true) {

    int s = accept(lsn, (struct sockaddr*)&peer, &addrlen);
    POSTASK_t task;

    task = posTaskCreate(shellTask, (void*)(intptr_t)s, 2, SHELL_STACK_SIZE);
    if (task == NULL) {

#if NOSCFG_FEATURE_CONOUT == 1
      nosPrint("net: out of tasks.");
#endif
      closesocket(s);
    }

    POS_SETTASKNAME(task, "shell");
  }

#else

  netSockAcceptHookSet(acceptHook);
  uip_listen(uip_htons(23));

  while (true)
    posTaskSleep(MS(10000));
#endif
}



