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
#include "net-test.h"

#include "em_cmu.h"
#include "em_gpio.h"
#include <em_chip.h>
#include <em_dbg.h>

int main(int argc, char **argv)
{
  /*
   * Initalize chip.
   */
    CHIP_Init();

  /*
   * Use XT oscillator, disable internal RC osc.
   */
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);
    SystemCoreClockUpdate();

    // Allow deep sleep.

    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;    /* Configure Cortex-M for deep sleep  */

  #if PORTCFG_CONOUT_ITM == 1
  #ifdef _DBG
    if (DBG_Connected()) {

      CMU_ClockEnable(cmuClock_GPIO, true);
      DBG_SWOEnable(0);

      /* Enable trace in core debug, taken from Energy Aware Commander */
      CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
      ITM->LAR  = 0xC5ACCE55;
      ITM->TER  = 0x0;
      ITM->TCR  = 0x0;
      TPI->SPPR = 2;
      TPI->ACPR = 0xf;
      ITM->TPR  = 0x0;
      DWT->CTRL = 0x400003FE;
      ITM->TCR  = 0x0001000D;
      TPI->FFCR = 0x00000100;
      ITM->TER  = 0x1;

    }
  #endif
  #endif

  uosInit();
  nosInit(mainTask, NULL, 10, 600, 200);
  return 0;
}
