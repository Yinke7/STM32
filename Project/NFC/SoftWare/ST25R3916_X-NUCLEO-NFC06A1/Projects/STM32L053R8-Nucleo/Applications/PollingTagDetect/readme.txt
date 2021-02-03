/**
  @page PollingTagDetect Readme file
  
  @verbatim
 ******************** (C) COPYRIGHT 2020 STMicroelectronics *******************
 * @file    readme.txt 
 * @author  SRA Application Team
 * @version v1.1.0
 * @date    19-Oct-2020   
 * @brief   This application reads/writes NDEF messages as well as work in CE mode.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0094, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0094
 *
  ******************************************************************************
  @endverbatim

@par Description

This directory contains a set of source files that implement a simple example based on 
ST25R3916

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.



@par Hardware and Software environment  

  - This example runs on STM32L476RG and STM32L03R8 devices.
    
  - This application has been tested with STMicroelectronics:
    STM32L4xx-Nucleo RevC
    STM32L0xx-Nucleo RevC
    boards and can be easily tailored to any other supported device 
    and development board.

  - STM32LXxx-Nucleo, STM32L0xx-Nucleo RevC Set-up    
    - Connect the Nucleo board to your PC with a USB cable type A to mini-B 
      to ST-LINK connector (CN1).
    - Please ensure that the ST-LINK connector CN2 jumpers are fitted.

        
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application

For IAR:
 - Open IAR toolchain.
 - Open the IAR project file EWARM\Project.eww.
 - Rebuild all files and load your image into target memory.
 - Run the example.

For µVision:
 - Open µVision 5 toolchain.
 - Open the µVision project file MDK-ARM\Project.uvprojx.
 - Rebuild all files and load your image into target memory.
 - Run the example.

For STM32CubeIDE:
 - Open STM32CubeIDE.
 - Set the default workspace proposed by the IDE (please be sure that there are not spaces in the workspace path).
 - Press "File" -> "Import" -> "Existing Projects into Workspace"; press "Browse" in the "Select root directory" and choose the path where the project is located (it should be <ProjectName>\STM32CubeIDE).
 - Rebuild all files and load your image into target memory.
 - Run the example.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
