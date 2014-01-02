/*
 * Copyright (c) 2006-2014, Ari Suutari <ari@stonepile.fi>.
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

void Default_Handler(void);

void Default_Handler()
{
  __disable_irq();
  while (1)
    ;
}

extern unsigned int __stack[];

PORT_WEAK_HANDLER(WWDG_IRQHandler);
PORT_WEAK_HANDLER(PVD_IRQHandler);
PORT_WEAK_HANDLER(TAMPER_STAMP_IRQHandler);
PORT_WEAK_HANDLER(RTC_WKUP_IRQHandler);
PORT_WEAK_HANDLER(FLASH_IRQHandler);
PORT_WEAK_HANDLER(RCC_IRQHandler);
PORT_WEAK_HANDLER(EXTI0_IRQHandler);
PORT_WEAK_HANDLER(EXTI1_IRQHandler);
PORT_WEAK_HANDLER(EXTI2_IRQHandler);
PORT_WEAK_HANDLER(EXTI3_IRQHandler);
PORT_WEAK_HANDLER(EXTI4_IRQHandler);
PORT_WEAK_HANDLER(DMA1_Channel1_IRQHandler);
PORT_WEAK_HANDLER(DMA1_Channel2_IRQHandler);
PORT_WEAK_HANDLER(DMA1_Channel3_IRQHandler);
PORT_WEAK_HANDLER(DMA1_Channel4_IRQHandler);
PORT_WEAK_HANDLER(DMA1_Channel5_IRQHandler);
PORT_WEAK_HANDLER(DMA1_Channel6_IRQHandler);
PORT_WEAK_HANDLER(DMA1_Channel7_IRQHandler);
PORT_WEAK_HANDLER(ADC1_IRQHandler);
PORT_WEAK_HANDLER(USB_HP_IRQHandler);
PORT_WEAK_HANDLER(USB_LP_IRQHandler);
PORT_WEAK_HANDLER(DAC_IRQHandler);
PORT_WEAK_HANDLER(COMP_IRQHandler);
PORT_WEAK_HANDLER(EXTI9_5_IRQHandler);
PORT_WEAK_HANDLER(LCD_IRQHandler);
PORT_WEAK_HANDLER(TIM9_IRQHandler);
PORT_WEAK_HANDLER(TIM10_IRQHandler);
PORT_WEAK_HANDLER(TIM11_IRQHandler);
PORT_WEAK_HANDLER(TIM2_IRQHandler);
PORT_WEAK_HANDLER(TIM3_IRQHandler);
PORT_WEAK_HANDLER(TIM4_IRQHandler);
PORT_WEAK_HANDLER(I2C1_EV_IRQHandler);
PORT_WEAK_HANDLER(I2C1_ER_IRQHandler);
PORT_WEAK_HANDLER(I2C2_EV_IRQHandler);
PORT_WEAK_HANDLER(I2C2_ER_IRQHandler);
PORT_WEAK_HANDLER(SPI1_IRQHandler);
PORT_WEAK_HANDLER(SPI2_IRQHandler);
PORT_WEAK_HANDLER(USART1_IRQHandler);
PORT_WEAK_HANDLER(USART2_IRQHandler);
PORT_WEAK_HANDLER(USART3_IRQHandler);
PORT_WEAK_HANDLER(EXTI15_10_IRQHandler);
PORT_WEAK_HANDLER(RTC_Alarm_IRQHandler);
PORT_WEAK_HANDLER(USB_FS_WKUP_IRQHandler);
PORT_WEAK_HANDLER(TIM6_IRQHandler);
PORT_WEAK_HANDLER(TIM7_IRQHandler);
PORT_WEAK_HANDLER(BootRAM);

PortExcHandlerFunc vectorTable[] __attribute__ ((section(".vectors"))) =
{ (PortExcHandlerFunc) __stack,        // stack pointer
    Reset_Handler,                     // code entry point
    Reset_Handler,                     // NMI handler (not really)
    HardFault_Handler,                 // hard fault handler (let's hope not)
    Reset_Handler,                     // MemManage failt
    Reset_Handler,                     // Bus fault
    UsageFault_Handler,                // Usage fault
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    SVC_Handler,                       // SVC
    Reset_Handler,                     // Debug monitor
    0,                                 // Reserved
    PendSV_Handler,                    // Context switch
    SysTick_Handler,
// STM32L1xx MD handlers
    WWDG_IRQHandler,
    PVD_IRQHandler,
    TAMPER_STAMP_IRQHandler,
    RTC_WKUP_IRQHandler,
    FLASH_IRQHandler,
    RCC_IRQHandler,
    EXTI0_IRQHandler,
    EXTI1_IRQHandler,
    EXTI2_IRQHandler,
    EXTI3_IRQHandler,
    EXTI4_IRQHandler,
    DMA1_Channel1_IRQHandler,
    DMA1_Channel2_IRQHandler,
    DMA1_Channel3_IRQHandler,
    DMA1_Channel4_IRQHandler,
    DMA1_Channel5_IRQHandler,
    DMA1_Channel6_IRQHandler,
    DMA1_Channel7_IRQHandler,
    ADC1_IRQHandler,
    USB_HP_IRQHandler,
    USB_LP_IRQHandler,
    DAC_IRQHandler,
    COMP_IRQHandler,
    EXTI9_5_IRQHandler,
    LCD_IRQHandler,
    TIM9_IRQHandler,
    TIM10_IRQHandler,
    TIM11_IRQHandler,
    TIM2_IRQHandler,
    TIM3_IRQHandler,
    TIM4_IRQHandler,
    I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler,
    I2C2_ER_IRQHandler,
    SPI1_IRQHandler,
    SPI2_IRQHandler,
    USART1_IRQHandler,
    USART2_IRQHandler,
    USART3_IRQHandler,
    EXTI15_10_IRQHandler,
    RTC_Alarm_IRQHandler,
    USB_FS_WKUP_IRQHandler,
    TIM6_IRQHandler,
    TIM7_IRQHandler,
    0,
    0,
    0,
    0,
    0,
    BootRAM
 };
