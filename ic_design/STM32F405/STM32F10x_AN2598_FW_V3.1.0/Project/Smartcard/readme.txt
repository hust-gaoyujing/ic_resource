/**
  @page Smartcard  AN2598 Smartcard Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2009 STMicroelectronics *******************
  * @file     Smartcard/readme.txt 
  * @author   MCD Application Team
  * @version  V3.1.0
  * @date     07/27/2009
  * @brief    Description of the AN2598 "Smartcard interface with the STM32F10x
  *           microcontrollers" .
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Example Description 

This AN describes a firmware Smartcard Interface based on the STM32F10x USART
peripheral. The main purpose of this firmware package is to provide resources 
facilitating the development of an application using the USART peripheral in 
smartcard mode.

The firmware interface is composed of library source files developed in order to
support ISO7816-3/4 specification, an application example is also provided.


@par Directory contents 

+ inc 
    - Smartcard/inc/platform_config.h    Evaluation board specific configuration file
    - Smartcard/inc/stm32f10x_conf.h     Library Configuration file
    - Smartcard/inc/stm32f10x_it.h       Interrupt handlers header file
    - Smartcard/inc/smartcard.h          Smart card Header file

+ src 
    - Smartcard/src/stm32f10x_it.c    Interrupt handlers
    - Smartcard/src/smartcard.c       Smart Card firmware functions
    - Smartcard/src/main.c            Main program  


@par Hardware and Software environment

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density 
    and Low-Density Devices.
  
  - This example has been tested with STMicroelectronics STM3210C-EVAL (STM32F10x 
    Connectivity line), STM3210E-EVAL (STM32F10x High-Density) and STM3210B-EVAL
    (STM32F10x Medium-Density) evaluation boards and can be easily tailored to
    any other supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in platform_config.h file.

  - STM3210C-EVAL Set-up 
    - Plug a Smartcard (ISO7816-3 T=0 compatible) into  the dedicated Smartcard 
      connector CN5.
    @note In this case USART3 Tx and CK pins are full remapped by software.
          Make sure that Jumper 11 (JP11), Jumper 12 (JP12) and Jumper 13 (JP13)
          are in position 1<-->2.

  - STM3210E-EVAL Set-up 
    - Plug a GSM11.11 smart card (ISO7816-3 T=0 compatible) into  the dedicated
      smart card  connector CN18.
    - Make sure that the Jumper 15 (JP15) and Jumper 16 (JP16) are fitted.

  - STM3210B-EVAL Set-up 
    - Plug a GSM11.11 smart card (ISO7816-3 T=0 compatible) into the dedicated
      smart card connector CN16.

@par How to use it ?

 - RVMDK 
    - Open the Smart_Card.Uv2 project
    - In the build toolbar select the project config:
        - STM3210C-EVAL: to configure the project for STM32 Connectivity line devices
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)

 - EWARMv5 (v5.30) 
    - Open the Smart_Card.eww workspace.
    - In the workspace toolbar select the project config:
        - STM3210C-EVAL: to configure the project for STM32 Connectivity line devices
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 - RIDE 
    - Open the Smart_Card.rprj project.
    - In the configuration toolbar(Project->properties) select the project config:
        - STM3210C-EVAL: to configure the project for STM32 Connectivity line devices
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)  

@note
 - Low-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 32 and 128 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.   
 
 * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
 */
