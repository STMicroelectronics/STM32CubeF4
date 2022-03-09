                                                 /**
  ******************************************************************************
  * @file    system_stm32f4xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer System Source File.
  *
  *   This file provides two functions and one global variable to be called from 
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32f4xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *      - SystemInit_ExtMemCtl(): Configures the GPIO and the QSPI in order to
  *                                access the external QSPI memory at the init.
  *                                This function is called when the switch DATA_IN_QSPI
  *                                is activated.
  *
  *      - SetSysClk(): This function is called when the switch DATA_IN_QSPI is 
  *                     activated, it configures the clock at 180s MHz with the PLL 
  *                     activated. It allows to access QSPI memory with high speed.
  *
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f4xx_system
  * @{
  */  
  
/** @addtogroup STM32F4xx_System_Private_Includes
  * @{
  */


#include "stm32f4xx.h"

#if !defined  (HSE_VALUE) 
#if defined(USE_STM32469I_DISCO_REVA) 
  #define HSE_VALUE    ((uint32_t)25000000)  /*!< Default value of the External oscillator in Hz */
#else
  #define HSE_VALUE    ((uint32_t)8000000)   /*!< Default value of the External oscillator in Hz */
#endif /* USE_STM32469I_DISCO_REVA */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Defines
  * @{
  */

/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to use QSPI memory mounted
     on DK as data memory  */
#define DATA_IN_QSPI

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field. 
                                   This value must be a multiple of 0x200. */
/******************************************************************************/

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Variables
  * @{
  */
  /* This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency 
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */

#if defined(__CC_ARM)
uint32_t SystemCoreClock __attribute__((section("NoInit"),zero_init)); /* Uninitialized Variable */

#elif defined(__ICCARM__)
__no_init uint32_t SystemCoreClock;

#elif defined(__GNUC__)
uint32_t SystemCoreClock = 16000000;
#endif

const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};
/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_FunctionPrototypes
  * @{
  */

#if defined (DATA_IN_QSPI)
  static void SetSysClk(void);
  static void SystemInit_ExtMemCtl(void); 
#endif /* DATA_IN_QSPI */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the FPU setting, vector table location and External memory 
  *         configuration.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  SystemCoreClock = 16000000;
  
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x24003010;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIR = 0x00000000;

#if defined (DATA_IN_QSPI)
  SetSysClk();
  SystemInit_ExtMemCtl(); 
#endif /* DATA_IN_QSPI */

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
}

/**
   * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *           
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.         
  *     
  * @note   - The system frequency computed by this function is not the real 
  *           frequency in the chip. It is calculated based on the predefined 
  *           constant and the selected clock source:
  *             
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *                                              
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *                          
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**) 
  *             or HSI_VALUE(*) multiplied/divided by the PLL factors.
  *         
  *         (*) HSI_VALUE is a constant defined in stm32f4xx_hal_conf.h file (default value
  *             16 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (**) HSE_VALUE is a constant defined in stm32f4xx_hal_conf.h file (its value
  *              depends on the application requirements), user has to ensure that HSE_VALUE
  *              is same as the real frequency of the crystal used. Otherwise, this function
  *              may have wrong result.
  *                
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  *     
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;
  
  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock source */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:  /* HSE used as system clock source */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08:  /* PLL used as system clock source */

      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
         SYSCLK = PLL_VCO / PLL_P
         */    
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
      
      if (pllsource != 0)
      {
        /* HSE used as PLL clock source */
        pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      }
      else
      {
        /* HSI used as PLL clock source */
        pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      }

      pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
      SystemCoreClock = pllvco/pllp;
      break;
    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }
  /* Compute HCLK frequency --------------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK frequency */
  SystemCoreClock >>= tmp;
}

#if defined (DATA_IN_QSPI)
/**
  * @brief  Configures the clock at 180MHz.
  *         Called in startup_stm32f4xx.s before jump to main.
  *         This function configures the clock for fast access to external memories
  * @param  None
  * @retval None
  */
void SetSysClk(void)
{
  register uint32_t tmpreg = 0, timeout = 0xFFFF;
  
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
  
/************************* PLL Parameters for clock at 180MHz******************/
  uint32_t PLL_M = 8,PLL_Q = 7, PLL_R = 2, PLL_N = 360, PLL_P = 2;
  
  /* Enable Power Control clock */
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  
  /* Config Voltage Scale 1 */
  PWR->CR |= PWR_CR_VOS;
  
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
  
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    tmpreg = RCC->CR & RCC_CR_HSERDY;
  } while((tmpreg != RCC_CR_HSERDY) && (timeout-- > 0));
  
  if(timeout != 0)
  {  
    /* Select regulator voltage output Scale 1 mode */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    
    PWR->CR |= PWR_CR_VOS_1;
    
    /* Enable Over Drive to reach the 180MHz frequency */
    /* Enable ODEN */
    PWR->CR |= 0x00010000;
    timeout = 0xFFFF;
    /* Wait till ODR is ready and if Time out is reached exit */
    do
    {
      tmpreg = PWR->CSR & PWR_CSR_ODRDY;
    } while((tmpreg != PWR_CSR_ODRDY) && (timeout-- > 0));
    
    /* Enable ODSW */
    PWR->CR |= 0x00020000;
    timeout = 0xFFFF;
    /* Wait till ODR is ready and if Time out is reached exit */
    do
    {
      tmpreg = PWR->CSR & PWR_CSR_ODSWRDY;
    } while((tmpreg != PWR_CSR_ODSWRDY) && (timeout-- > 0)); 
    
    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
    
    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
      (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24) | (PLL_R << 28);
    
    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;
  }  
  /* Wait that PLL is ready */
  timeout = 0xFFFF;
  do
  {
    tmpreg = (RCC->CR & RCC_CR_PLLRDY); 
  } while((tmpreg != RCC_CR_PLLRDY) && (timeout-- > 0));
  
  if(timeout != 0)
  {
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
    
    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    
    timeout = 0xFFFF;
    do
    {
      tmpreg = (RCC->CFGR & RCC_CFGR_SWS); 
    } while((tmpreg != RCC_CFGR_SWS) && (timeout-- > 0));
  }
  
  SystemCoreClockUpdate();
}

/**
  * @brief  Setup the external memory controller.
  *         Configures the GPIO and the QSPI in order to access the external 
  *         QSPI memory at the init.
  *         This function is called when the switch DATA_IN_QSPI is activated in 
  *         SystemInit() before jump to main.
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
  /****************************************************************************/
  /*                                                                          */
  /* Configuration of the IOs :                                               */
  /* --------------------------                                               */
  /* GPIOF10  : CLK                                                            */
  /* GPIOB6   : BK1_nCS                                                        */
  /* GPIOF8   : BK1_IO0/SO                                                     */
  /* GPIOF9   : BK1_IO1/SI                                                     */
  /* GPIOF7   : BK1_IO2                                                        */
  /* GPIOF6   : BK1_IO3                                                        */
  /*                                                                          */
  /* Configuration of the QSPI :                                              */
  /* ---------------------------                                              */
  /* - Instruction is on one single line                                      */
  /* - Address is 32-bits on four lines                                       */
  /* - No alternate bytes                                                     */
  /* - Ten dummy cycles                                                       */
  /* - Data is on four lines                                                  */
  /*                                                                          */
  /* If the clock is changed :                                                */
  /* -------------------------                                                */
  /* - Modify the prescaler in the control register                           */
  /* - Update the number of dummy cycles on the memory side and on            */
  /*   communication configuration register                                   */
  /*                                                                          */
  /* If the memory is changed :                                               */
  /* --------------------------                                               */
  /* - Update the device configuration register with the memory configuration */
  /* - Modify the instructions with the instruction set of the memory         */
  /* - Configure the number of dummy cycles as described in memory datasheet  */
  /* - Modify the data size and alternate bytes according memory datasheet    */
  /*                                                                          */
  /****************************************************************************/
  
  register uint32_t tmpreg = 0, datareg = 0,tmp = 0, timeout = 0xFFFF;
  
  /*--------------------------------------------------------------------------*/
  /*------------------ Activation of the peripheral clocks -------------------*/
  /*--------------------------------------------------------------------------*/      
  /* Enable GPIOB and GPIOF interface clock */ 
  /* Enable clock of the QSPI */
  RCC->AHB3ENR |= 0x00000002;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  
  /*--------------------------------------------------------------------------*/
  /*--------------------- Configuration of the I/O pins ----------------------*/
  /*--------------------------------------------------------------------------*/
  /* Configure alternate function selection for IO pins */
  GPIOF->AFR[0] = 0x99000000;
  GPIOF->AFR[1] = 0x000009AA;
  GPIOB->AFR[0] = 0x0A000000; 
  
  /* Configure alternate function mode for IO pins */
  GPIOF->MODER = 0x002AA000;
  GPIOB->MODER = 0x00002280;
  
  /* Configure output speed for IO pins */
  GPIOF->OSPEEDR = 0x003FF000;
  GPIOB->OSPEEDR = 0x000030C0;
  
  /* Configure pull-up or pull-down for IO pins */
  GPIOB->PUPDR   = 0x00001100;
  
  /*--------------------------------------------------------------------------*/
  /*----------------------- Initialization of the QSPI -----------------------*/
  /*--------------------------------------------------------------------------*/
  timeout = 0xFFFF;
  do
  {
    tmpreg = (QUADSPI->SR & QUADSPI_SR_BUSY); 
  } while((tmpreg != 0) && (timeout-- > 0));
  
  if (timeout != 0)
  {
    /* Configure device configuration register of QSPI */
    /* - FSIZE = 23 */
    QUADSPI->DCR = QUADSPI_DCR_CSHT_0|  23<<16;
    /* Configure control register of QSPI: precsaler, sample shift and enable QSPI */
    QUADSPI->CR = (1 << 24) | QUADSPI_CR_SSHIFT|QUADSPI_CR_EN;
  }  
  /*--------------------------------------------------------------------------*/
  /*----------- Configuration of the dummy cycles on flash side --------------*/
  /*--------------------------------------------------------------------------*/
  /* Configure communication register to read volatile configuration register */
  /* - FMODE = Indirect read
  - DMODE = Data on a single line
  - IMODE = Instruction on a single line
  - INSTRUCTION = READ_VOL_CFG_REG_CMD */
  tmp = QUADSPI->CCR;
  tmp = tmp& (~(QUADSPI_CCR_FMODE | QUADSPI_CCR_DMODE | QUADSPI_CCR_IMODE | QUADSPI_CCR_INSTRUCTION));
  tmp |= (QUADSPI_CCR_FMODE_0 | QUADSPI_CCR_DMODE_0 | QUADSPI_CCR_IMODE_0 | 0x85);
  QUADSPI->CCR = tmp;
  /* Wait that the transfer is complete */
  timeout = 0xFFFF;
  do
  {
    tmpreg = (QUADSPI->SR & QUADSPI_SR_TCF); 
  } while((tmpreg == 0) && (timeout-- > 0));
  
  if (timeout != 0)
  {
    /* Read received value */
    datareg = QUADSPI->DR;
    
    /* Clear transfer complete flag */
    QUADSPI->FCR = QUADSPI_FCR_CTCF;
    
    /* Perform abort (mandatory workaround for this version of QSPI) */
    tmp = QUADSPI->CR; 
    tmp = (tmp&(~QUADSPI_CR_ABORT));
    QUADSPI->CR = tmp|QUADSPI_CR_ABORT;
    
    /* Wait that the transfer is complete */
    timeout = 0xFFFF;
    do
    {
      tmpreg = (QUADSPI->SR & QUADSPI_SR_TCF); 
    } while((tmpreg == 0) && (timeout-- > 0));
    
    if (timeout != 0)
    {
      /* Clear transfer complete flag */
      QUADSPI->FCR = QUADSPI_FCR_CTCF;
      
      /* Configure communication register to enable write operations */
      tmp = QUADSPI->CCR;
      tmp = tmp& (~(QUADSPI_CCR_FMODE | QUADSPI_CCR_DMODE | QUADSPI_CCR_INSTRUCTION));
      tmp |= 0x06;
      QUADSPI->CCR = tmp;
      /* Wait that the transfer is complete */
      timeout = 0xFFFF;
      do
      {
        tmpreg = (QUADSPI->SR & QUADSPI_SR_TCF); 
      } while((tmpreg == 0) && (timeout-- > 0));
      
      if (timeout != 0)
      {
        /* Clear transfer complete flag */
        QUADSPI->FCR = QUADSPI_FCR_CTCF;
        
        /* Configure the mask for the auto-polling mode on write enable bit of status register */
        QUADSPI->PSMKR = 0x2;
        
        /* Configure the value for the auto-polling mode on write enable bit of status register */
        QUADSPI->PSMAR = 0x2;
        
        /* Configure the auto-polling interval */
        QUADSPI->PIR   = 0x10;
        
        /* Configure control register to automatically stop the auto-polling mode */
        QUADSPI->CR = (QUADSPI->CR&(~QUADSPI_CR_APMS));
        QUADSPI->CR |= QUADSPI_CR_APMS;
        
        /* Configure communication register to perform auto-polling mode on status register */           
        tmp = QUADSPI->CCR;
        tmp = tmp& (~(QUADSPI_CCR_FMODE | QUADSPI_CCR_DMODE | QUADSPI_CCR_INSTRUCTION));
        tmp |= (QUADSPI_CCR_FMODE_1 | QUADSPI_CCR_DMODE_0 | 0x05);
        QUADSPI->CCR = tmp;
        /* Wait that the status match occurs */
        timeout = 0xFFFF;
        do
        {
          tmpreg = (QUADSPI->SR & QUADSPI_SR_SMF); 
        } while((tmpreg == 0) && (timeout-- > 0));
        
        if (timeout != 0)
        {
          /* Clear status match flag */
          QUADSPI->FCR = QUADSPI_FCR_CSMF;
          
          /* Write volatile configuration register with new dummy cycles */  
          datareg = (datareg&0xF)| 10<<4;
          
          /* Configure communication register to write volatile configuration register */
          tmp = QUADSPI->CCR;
          tmp = tmp& (~(QUADSPI_CCR_FMODE | QUADSPI_CCR_INSTRUCTION));
          tmp |= 0x81;
          QUADSPI->CCR = tmp;
          /* Write the value to transmit */
          QUADSPI->DR = datareg;
          
          /* Wait that the transfer is complete */
          timeout = 0xFFFF;
          do
          {
            tmpreg = (QUADSPI->SR & QUADSPI_SR_TCF); 
          } while((tmpreg == 0) && (timeout-- > 0));
          
          if (timeout != 0)
          {
            /* Clear transfer complete flag */
            QUADSPI->FCR = QUADSPI_FCR_CTCF;
            
            /* Perform abort (mandatory workaround for this version of QSPI) */
            tmp = QUADSPI->CR; 
            tmp = (tmp&(~QUADSPI_CR_ABORT));
            QUADSPI->CR = tmp|QUADSPI_CR_ABORT;
            
            /* Wait that the transfer is complete */
            timeout = 0xFFFF;
            do
            {
              tmpreg = (QUADSPI->SR & QUADSPI_SR_TCF); 
            } while((tmpreg == 0) && (timeout-- > 0));
            
            if (timeout != 0)
            {
              /* Clear transfer complete flag */
              QUADSPI->FCR = QUADSPI_FCR_CTCF;
              
                /*------------------------------------------------------------*/
                /*--------- Configuration of the memory-mapped mode ----------*/
                /*------------------------------------------------------------*/
                /* Configure communication register for reading sequence in memory-mapped mode */
                /* - FMODE = Memory-mapped
                   - DMODE = Data on four lines
                   - DCYC = 10
                   - ADSIZE = 32-bit address
                   - ADMODE = Address on four lines
                   - IMODE = Instruction on a single line
                   - INSTRUCTION = QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD */
              tmp = QUADSPI->CCR;
              tmp = tmp& (~(QUADSPI_CCR_FMODE | QUADSPI_CCR_DMODE | QUADSPI_CCR_DCYC | QUADSPI_CCR_ADSIZE | QUADSPI_CCR_ADMODE | QUADSPI_CCR_INSTRUCTION));
              tmp |= (QUADSPI_CCR_FMODE | QUADSPI_CCR_DMODE |  (10 << POSITION_VAL(QUADSPI_CCR_DCYC)) | QUADSPI_CCR_ADSIZE_1 | QUADSPI_CCR_ADMODE | 0xEB);
              QUADSPI->CCR = tmp;
            }
          }
        }
      }
    }
  }
}

#endif /* DATA_IN_QSPI */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
