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
 * ENC28J60 driver utility functions for Olimex STM32-P152 + UEXT ENC28J60 module.
 */

#include <picoos.h>
#include <picoos-net.h>
#include <picoos-u.h>

#include "enc28j60.h"

#define CS_PIN GPIO_Pin_12

/**
 * Initializes the SPI pins, frequency, and SPI mode configuration.
 */
void enc28j60_spi_init(void) {
  /*
   * UEXT connections with STM32-P152:
   * 1 +3.3V
   * 2 GND
   * 3 LEDA = PC10
   * 4 WOL  = PC11
   * 5 INT  = PB6
   * 6 RST  = PB7
   * 7 MISO = SPI1 MISO = PE14
   * 8 MOSI = SPI1 MOSI = PE15
   * 9 SCK  = SPI1 SCK  = PE13
   * 10 CS  = PE12
   */

  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  // Enable clocks.

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

  // Set CS to 1 (inactive)

  GPIO_WriteBit(GPIOE, CS_PIN, Bit_SET);

  // CS pin

  GPIO_InitStructure.GPIO_Pin = CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);


  // SCK, MISO & MOSI pin

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_SPI1);

  // Configure SPI bus

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //XXX 4
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  // Enable SPI1

  SPI_Cmd(SPI1, ENABLE);

#ifdef ENC28J60_USE_INTERRUPTS
#endif
}

/**
 * Selects an ENC28J60 chip by bringing the chip's CS low.
 * The ENC28J60 to talk to is determined by the global ENC28J60_Index variable.
 */
void enc28j60_spi_select(void) {

  GPIO_WriteBit(GPIOE, CS_PIN, Bit_RESET);
}

/**
 * Deselects an ENC28J60 chip by bringing the chip's CS high.
 * The ENC28J60 to talk to is determined by the global ENC28J60_Index variable.
 */
void enc28j60_spi_deselect(void) {

  GPIO_WriteBit(GPIOE, CS_PIN, Bit_SET);
}

/**
 * Writes a byte to the ENC28J60 through SPI.
 * The chip must be selected prior to this write.
 * @param data the 8-bit data byte to write.
 */
void enc28j60_spi_write(uint8_t data) {

  // Wait until transmit buffer is empty
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  // Send byte
  SPI_I2S_SendData(SPI1, data);

  // Wait receive (value is not needed, but it tells that operation is complete)
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  SPI_I2S_ReceiveData(SPI1);
}

/**
 * Explicitly read a byte from the ENC28J60 by first sending the dummy byte
 * 0x00.
 * The chip must be selected prior to this write.
 * @return the data read.
 */
uint8_t enc28j60_spi_read(void) {

  // Wait until transmit buffer is empty
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  // Send dummy byte
  SPI_I2S_SendData(SPI1, '\0');

  // Wait for receive
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  // Return received byte
  return SPI_I2S_ReceiveData(SPI1);
}
