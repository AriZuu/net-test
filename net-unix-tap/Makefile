#
# Copyright (c) 2012-2013, Ari Suutari <ari@stonepile.fi>.
# All rights reserved. 
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. The name of the author may not be used to endorse or promote
#     products derived from this software without specific prior written
#     permission. 
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
# INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.

RELROOT = ../../picoos/

PORT = unix

BUILD = DEBUG
NETCFG_STACK = 4
export NETCFG_STACK

include $(RELROOT)make/common.mak

NANO = 1
TARGET = net
COMMON_SRC=../net-common
SRC_TXT =	$(COMMON_SRC)/net-test.c \
		$(COMMON_SRC)/shell.c \
		main.c
		
SRC_HDR = 
SRC_OBJ =
SRC_LIB =

CDEFINES += 

DIR_USRINC +=	$(RELROOT)/../picoos-micro  \
		$(COMMON_SRC) \
		$(RELROOT)/../picoos-micro/fat/src \
		$(RELROOT)/../picoos-net \
		$(RELROOT)/../picoos-net/drivers

DIR_CONFIG = $(CURRENTDIR)/config
DIR_OUTPUT = $(CURRENTDIR)/bin
MODULES +=  $(RELROOT)/../picoos-net $(RELROOT)/../picoos-micro

POSTLINK1 = size $(TARGETOUT)
POSTLINK2 = 

include $(MAKE_OUT)
