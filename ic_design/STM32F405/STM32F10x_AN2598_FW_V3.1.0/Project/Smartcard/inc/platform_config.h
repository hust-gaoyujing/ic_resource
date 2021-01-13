/**
  ******************************************************************************
  * @file    Smartcard/inc/platform_config.h 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   Evaluation board specific configuration file.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
#if !defined (USE_STM3210B_EVAL) &&  !defined (USE_STM3210E_EVAL) &&  !defined (USE_STM3210C_EVAL)
 //#define USE_STM3210B_EVAL
 //#define USE_STM3210E_EVAL
 //#define USE_STM3210C_EVAL
#endif

/* Define the STM32F10x hardware depending on the used evaluation board */
#ifdef USE_STM3210B_EVAL
  #define SC_USART                 USART3
  #define SC_USART_GPIO            GPIOB
  #define SC_USART_CLK             RCC_APB1Periph_USART3
  #define SC_USART_GPIO_CLK        RCC_APB2Periph_GPIOB
  #define SC_USART_PIN_TX          GPIO_Pin_10
  #define SC_USART_PIN_CK          GPIO_Pin_12
  #define SC_USART_IRQn            USART3_IRQn
  #define SC_USART_IRQHandler      USART3_IRQHandler 
  /* Smartcard Inteface GPIO pins */
  #define SC_PIN_3_5V               GPIO_Pin_11
  #define SC_PIN_3_5V_GPIO          GPIOD
  #define SC_PIN_3_5V_GPIO_CLK      RCC_APB2Periph_GPIOD
  #define SC_PIN_RESET              GPIO_Pin_11
  #define SC_PIN_RESET_GPIO         GPIOB
  #define SC_PIN_RESET_GPIO_CLK     RCC_APB2Periph_GPIOB
  #define SC_PIN_CMDVCC             GPIO_Pin_7
  #define SC_PIN_CMDVCC_GPIO        GPIOE
  #define SC_PIN_CMDVCC_GPIO_CLK    RCC_APB2Periph_GPIOE
  #define SC_PIN_OFF                GPIO_Pin_14
  #define SC_PIN_OFF_GPIO           GPIOE
  #define SC_PIN_OFF_GPIO_CLK       RCC_APB2Periph_GPIOE
  #define SC_DETECT_EXTI            EXTI_Line14
  #define SC_DETECT_PIN             GPIO_PortSourceGPIOE
  #define SC_DETECT_GPIO            GPIO_PinSource14
  #define SC_DETECT_IRQ             EXTI15_10_IRQn
  #define SC_DETECT_IRQHandler      EXTI15_10_IRQHandler
#elif defined USE_STM3210E_EVAL
  #define SC_USART                 USART3
  #define SC_USART_GPIO            GPIOB
  #define SC_USART_CLK             RCC_APB1Periph_USART3
  #define SC_USART_GPIO_CLK        RCC_APB2Periph_GPIOB
  #define SC_USART_PIN_TX          GPIO_Pin_10
  #define SC_USART_PIN_CK          GPIO_Pin_12
  #define SC_USART_IRQn            USART3_IRQn
  #define SC_USART_IRQHandler      USART3_IRQHandler
  /* Smartcard Inteface GPIO pins */
  #define SC_PIN_3_5V               GPIO_Pin_0
  #define SC_PIN_3_5V_GPIO          GPIOB
  #define SC_PIN_3_5V_GPIO_CLK      RCC_APB2Periph_GPIOB
  #define SC_PIN_RESET              GPIO_Pin_11
  #define SC_PIN_RESET_GPIO         GPIOB
  #define SC_PIN_RESET_GPIO_CLK     RCC_APB2Periph_GPIOB
  #define SC_PIN_CMDVCC             GPIO_Pin_6
  #define SC_PIN_CMDVCC_GPIO        GPIOC
  #define SC_PIN_CMDVCC_GPIO_CLK    RCC_APB2Periph_GPIOC
  #define SC_PIN_OFF                GPIO_Pin_7 
  #define SC_PIN_OFF_GPIO           GPIOC
  #define SC_PIN_OFF_GPIO_CLK       RCC_APB2Periph_GPIOC
  #define SC_DETECT_EXTI            EXTI_Line7
  #define SC_DETECT_PIN             GPIO_PortSourceGPIOC
  #define SC_DETECT_GPIO            GPIO_PinSource7
  #define SC_DETECT_IRQ             EXTI9_5_IRQn
  #define SC_DETECT_IRQHandler      EXTI9_5_IRQHandler
#elif defined USE_STM3210C_EVAL
  #define SC_USART                 USART3
  #define SC_USART_GPIO            GPIOD
  #define SC_USART_CLK             RCC_APB1Periph_USART3
  #define SC_USART_GPIO_CLK        RCC_APB2Periph_GPIOD
  #define SC_USART_PIN_TX          GPIO_Pin_8
  #define SC_USART_PIN_CK          GPIO_Pin_10
  #define SC_USART_IRQn            USART3_IRQn
  #define SC_USART_IRQHandler      USART3_IRQHandler 
  /* Smartcard Inteface GPIO pins */
  #define SC_PIN_3_5V               GPIO_Pin_0
  #define SC_PIN_3_5V_GPIO          GPIOC
  #define SC_PIN_3_5V_GPIO_CLK      RCC_APB2Periph_GPIOC
  #define SC_PIN_RESET              GPIO_Pin_9
  #define SC_PIN_RESET_GPIO         GPIOD
  #define SC_PIN_RESET_GPIO_CLK     RCC_APB2Periph_GPIOD
  #define SC_PIN_CMDVCC             GPIO_Pin_14
  #define SC_PIN_CMDVCC_GPIO        GPIOE
  #define SC_PIN_CMDVCC_GPIO_CLK    RCC_APB2Periph_GPIOE
  #define SC_PIN_OFF                GPIO_Pin_7 
  #define SC_PIN_OFF_GPIO           GPIOE
  #define SC_PIN_OFF_GPIO_CLK       RCC_APB2Periph_GPIOE
  #define SC_DETECT_EXTI            EXTI_Line7
  #define SC_DETECT_PIN             GPIO_PortSourceGPIOE
  #define SC_DETECT_GPIO            GPIO_PinSource7
  #define SC_DETECT_IRQ             EXTI9_5_IRQn
  #define SC_DETECT_IRQHandler      EXTI9_5_IRQHandler
#endif /* USE_STM3210B_EVAL */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
