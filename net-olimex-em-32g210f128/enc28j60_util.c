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

/**
 * ENC28J60 driver utility functions for Olimex EM-32G210F128-H + UEXT ENC28J60 module.
 */

#include <picoos.h>
#include <picoos-net.h>
#include <picoos-u.h>

#include "enc28j60.h"

#include <em_gpio.h>
#include <em_cmu.h>
#include <em_usart.h>

static USART_InitSync_TypeDef spiConfig =
{
  .enable     = usartEnable,
  .refFreq    = 0,
  .baudrate   = 2000000,
  .databits   = usartDatabits8,
  .master     = true,
  .msbf       = true,
  .clockMode  = usartClockMode0
};

/**
 * Initializes the SPI pins, frequency, and SPI mode configuration.
 */
void enc28j60_spi_init(void)
{
  /*
   * UEXT connections with STM32-P152:
   * 1 +3.3V
   * 2 GND
   * 3 LEDA = TX_US1
   * 4 WOL  = RX_US1
   * 5 INT  = PD7
   * 6 RST  = PD6
   * 7 MISO = PE11
   * 8 MOSI = PE10
   * 9 SCK  = PE12
   * 10 CS  = PE13
   */

  // Enable clocks.
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_USART0, true);

  // Configure GPIO pins.
  // Set CS to 1 (inactive)
  GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 1);
  GPIO_PinModeSet(gpioPortE, 10, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortE, 11, gpioModeInput, 0);

  // Configure SPI.
  USART_Reset(USART0);
  USART_InitSync(USART0, &spiConfig);

  USART0->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_CLKPEN | USART_ROUTE_LOCATION_LOC0;

#ifdef ENC28J60_USE_INTERRUPTS

  // Configure UEXT enc28j60 interrupt line.

   CMU_ClockEnable(cmuClock_GPIO, true);
   GPIO_PinModeSet(gpioPortD, 7, gpioModeInputPullFilter, 1);
   GPIO_IntConfig(gpioPortD, 7, false, true, true);

   NVIC_EnableIRQ(GPIO_ODD_IRQn);
   NVIC_SetPriority(GPIO_ODD_IRQn, PORT_PENDSV_PRI - 1);

#endif
}

#ifdef ENC28J60_USE_INTERRUPTS
void GPIO_ODD_IRQHandler(void)
{
  c_pos_intEnter();

  GPIO_IntClear((1 << 7));

  uint8_t eir;
  uint8_t pktCnt;

  eir = enc28j60_Register_Read(EIR);
  pktCnt = enc28j60_Register_Read(EPKTCNT);

  if (pktCnt > 0) {

    enc28j60_Disable_Global_Interrupts();
    netInterrupt();
  }

  if (eir & EIR_PKTIF)
    enc28j60_Bitfield_Clear(EIR, EIR_PKTIF);

  c_pos_intExitQuick();
}
#endif

/**
 * Selects an ENC28J60 chip by bringing the chip's CS low.
 * The ENC28J60 to talk to is determined by the global ENC28J60_Index variable.
 */
void enc28j60_spi_select(void)
{
  GPIO_PinOutClear(gpioPortE, 13);
}

/**
 * Deselects an ENC28J60 chip by bringing the chip's CS high.
 * The ENC28J60 to talk to is determined by the global ENC28J60_Index variable.
 */
void enc28j60_spi_deselect(void)
{
  GPIO_PinOutSet(gpioPortE, 13);
}

/**
 * Writes a byte to the ENC28J60 through SPI.
 * The chip must be selected prior to this write.
 * @param data the 8-bit data byte to write.
 */
void enc28j60_spi_write(uint8_t data)
{
  USART_Tx(USART0, data);
  USART_Rx(USART0);
}

/**
 * Explicitly read a byte from the ENC28J60 by first sending the dummy byte
 * 0x00.
 * The chip must be selected prior to this write.
 * @return the data read.
 */
uint8_t enc28j60_spi_read(void)
{
  USART_Tx(USART0, 0);
  return USART_Rx(USART0);
}

