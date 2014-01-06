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
 * ENC28J60 driver utility functions for Olimex MSP430-T5510 + UEXT ENC28J60 module.
 */

#include <picoos.h>
#include <picoos-net.h>
#include <picoos-u.h>

#include "enc28j60.h"

/**
 * Initializes the SPI pins, frequency, and SPI mode configuration.
 */
void enc28j60_spi_init(void)
{
  /*
   * UEXT connections with MSP430-T5510:
   * 1 +3.3V
   * 2 GND
   * 3 LEDA = P4_4/UCA1TXD
   * 4 WOL  = P4_5/UCARXD
   * 5 INT  = P4_7
   * 6 RST  = P4_6
   * 7 MISO = P4_2/UCB1SOMI
   * 8 MOSI = P4_1/UCB1SIMO
   * 9 SCK  = P4_3/UCB1CLK
   * 10 CS  = PJ_0
   */

#define CS_PIN   BIT0
#define MOSI_PIN BIT1
#define MISO_PIN BIT2
#define SCK_PIN  BIT3
#define INT_PIN  BIT7

  // Configure SPI
  UCB1CTL1 = UCSWRST;
  UCB1CTL0 = UCSYNC + UCMST + UCMSB + UCCKPH;
  UCB1CTL1 |= UCSSEL_2; // cpu clock

  // Clock divisor == 1
  UCB1BR0 = 0x2;
  UCB1BR1 = 0;

  // Configure GPIO pins.
  P4SEL |= MOSI_PIN + MISO_PIN + SCK_PIN;
  P4DIR |= MOSI_PIN + SCK_PIN;
  PJDIR |= CS_PIN;
  P4DIR &= ~(MISO_PIN + INT_PIN);

  // CS high = inactive
  PJOUT |= CS_PIN;

  // Enable SPI.
  UCB1CTL1 &= ~UCSWRST;

#ifdef ENC28J60_USE_INTERRUPTS


#endif

   // turn off led red

   PJOUT &= ~BIT2;

}

/**
 * Selects an ENC28J60 chip by bringing the chip's CS low.
 * The ENC28J60 to talk to is determined by the global ENC28J60_Index variable.
 */
void enc28j60_spi_select(void)
{
  PJOUT &= ~CS_PIN;
}

/**
 * Deselects an ENC28J60 chip by bringing the chip's CS high.
 * The ENC28J60 to talk to is determined by the global ENC28J60_Index variable.
 */
void enc28j60_spi_deselect(void)
{
  PJOUT |= CS_PIN;
}

/**
 * Writes a byte to the ENC28J60 through SPI.
 * The chip must be selected prior to this write.
 * @param data the 8-bit data byte to write.
 */
void enc28j60_spi_write(uint8_t data)
{
  uint8_t dummy;

  UCB1TXBUF = data;
  while(!(UCB1IFG & UCRXIFG));
  dummy = UCB1RXBUF;
}

/**
 * Explicitly read a byte from the ENC28J60 by first sending the dummy byte
 * 0x00.
 * The chip must be selected prior to this write.
 * @return the data read.
 */
uint8_t enc28j60_spi_read(void)
{
  UCB1TXBUF = 0;
  while(!(UCB1IFG & UCRXIFG));
  return UCB1RXBUF;
}

