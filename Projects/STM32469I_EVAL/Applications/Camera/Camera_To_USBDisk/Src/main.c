/**
  ******************************************************************************
  * @file    Camera/Camera_To_USBDisk/Src/main.c
  * @author  MCD Application Team
  * @brief   This application describes how to configure the camera in continuous mode
             and save picture under USBDisk.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_HAL_Applications
  * @{
  */

/** @addtogroup Camera_To_USBDisk
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define VSYNC           1  
#define VBP             1 
#define VFP             1
#define VACT            480
#define HSYNC           1
#define HBP             1
#define HFP             1
#define HACT            400

#define LEFT_AREA         1
#define RIGHT_AREA        2


#define CAMERA_STATE_RESET               0
#define CAMERA_STATE_CAPTURE_ONGOING     1
#define CAMERA_STATE_DISPLAY_ONGOING     2
#define CAMERA_STATE_SAVING_PICTURE      3

/* Private macro -------------------------------------------------------------*/
#define __DSI_MASK_TE()   (GPIOJ->AFR[0] &= (0xFFFFF0FFU))   /* Mask DSI TearingEffect Pin*/
#define __DSI_UNMASK_TE() (GPIOJ->AFR[0] |= ((uint32_t)(GPIO_AF13_DSI) << 8)) /* UnMask DSI TearingEffect Pin*/

/* Private variables ---------------------------------------------------------*/
uint8_t __IO cameraState = CAMERA_STATE_RESET;

FATFS MSC_FatFs;  /* File system object for USB disk logical drive */
FIL MyFile;       /* File object */
char MSC_Path[4]; /* USB Host logical drive path */
USBH_HandleTypeDef  hUSBHost;

/* Image header */
const uint32_t aBMPHeader[14]=
{0xFA364D42, 0x00000005, 0x00360000, 0x00280000, 0x01E00000, 0x01100000, 0x00010000,
 0x00000018, 0xF5400000, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0000};

typedef enum {
  STORAGE_IDLE = 0,
  STORAGE_READY,
}MSC_ApplicationTypeDef;

MSC_ApplicationTypeDef Appli_state = STORAGE_IDLE;

/* Camera resolution is 480x272 */
static uint32_t   CameraResX = CAMERA_480x272_RES_X;
static uint32_t   CameraResY = CAMERA_480x272_RES_Y;
static uint32_t   LcdResX    = WVGA_RES_X; /* WVGA landscape 800x480 */
static uint32_t   LcdResY    = WVGA_RES_Y;


extern LTDC_HandleTypeDef hltdc_eval;
extern DSI_HandleTypeDef hdsi_eval;

static __IO int32_t pending_buffer = -1;
static __IO int32_t active_area = 0;

uint8_t pColLeft[]    = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
uint8_t pColRight[]   = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */
uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
uint8_t pScanCol[]    = {0x02, 0x15};             /* Scan @ 533 */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void PicturePrepare(void);
static void Error_Handler(void);
static void SavePicture(void);
static void CAMERA_Capture(void);
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
static void LCD_LL_ConvertLineToARGB8888(void *pSrc, void *pDst);
static void DMA2D_TransferCompleteCallback(DMA2D_HandleTypeDef *hdma2d);

static uint8_t LCD_Init(void);
static void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address);
static void LTDC_Init(void);

static void Display_WaitRefresh(void);
static void Display_StartRefresh(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

    /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  BSP_IO_Init();

  /* Reset and power down camera to be sure camera is Off prior start */
  BSP_CAMERA_HwReset();
  BSP_CAMERA_PwrDown();

  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* SDRAM initialization */
  BSP_SDRAM_Init();
  
  /*##-1- LCD DSI initialization in command mode  with one LTDC layers of size 800x480 */
  LCD_Init(); 
  
  LCD_LayertInit(0, LCD_FB_START_ADDRESS);
  BSP_LCD_SelectLayer(0); 
  
  HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft);
  HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_PASET, pPage);

  /* Update pitch : the draw is done on the whole physical X Size */
  HAL_LTDC_SetPitch(&hltdc_eval, BSP_LCD_GetXSize(), 0);
  
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  /* Display USB initialization message */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"USB init ..", RIGHT_MODE);  

  pending_buffer = 0;
  active_area = LEFT_AREA; 
  
  HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pScanCol);
  
  Display_WaitRefresh();
  /*##-2- Init Host Library ##################################################*/

  USBH_Init(&hUSBHost, USBH_UserProcess, 0);

  /* Add Supported Class */
  USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);

  /* Start Host Process */
  USBH_Start(&hUSBHost);

  /*##-3- Configure TAMPER Button ############################################*/
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /*##-4- Link the USB Host disk I/O driver ##################################*/
  FATFS_LinkDriver(&USBH_Driver, MSC_Path);

  /*##-5- Camera Initialization and start capture ############################*/

  Display_WaitRefresh();
  /* Display camera initialization message */
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"Camera init ..", RIGHT_MODE);
  Display_StartRefresh();
  
  /* Initialize the Camera */
  BSP_CAMERA_Init(RESOLUTION_R480x272);
  /* Wait for the camera initialization after HW reset*/
  HAL_Delay(100);
  
  /* Start the Camera Capture */
  BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);

  Display_WaitRefresh();  
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 48), (uint8_t *)"Press TAMPER button to take snapshot", RIGHT_MODE);
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"Camera Play ..", RIGHT_MODE);  
  Display_StartRefresh();

  /*##-6- Run Application ####################################################*/
  while (1)
  {
    /* USB Host Background task */
    USBH_Process(&hUSBHost);

    switch(Appli_state)
    {
    case STORAGE_READY:
      CAMERA_Capture();
      break;

    case STORAGE_IDLE:
      break;
    }
  }
}

/**
  * @brief  Saves Picture into USB disk.
  * @param  None
  * @retval None
  */
static void SavePicture(void)
{
  FRESULT res1, res2;     /* FatFs function common result code */
  uint32_t byteswritten;  /* File write count */
  static uint32_t counter = 0;
  uint8_t str[15];
  uint8_t str_display[50];
  
  /* Suspend the camera capture */
  BSP_CAMERA_Suspend();
  cameraState = CAMERA_STATE_SAVING_PICTURE;
  
  HAL_Delay(100);  
  
  Display_WaitRefresh(); 
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 48), (uint8_t *)"                                    ", RIGHT_MODE);
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"                     ", RIGHT_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"Camera stopped...", RIGHT_MODE);
  
  Display_StartRefresh();
  Display_WaitRefresh();  
  HAL_Delay(1000);
  
  /* Prepare the image to be saved */
  PicturePrepare();
  
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"                     ", RIGHT_MODE);
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"           Picture Prepared...", RIGHT_MODE);
  
  Display_StartRefresh(); 
  Display_WaitRefresh();  
  HAL_Delay(1000);

  
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"                                 ", RIGHT_MODE);
  /* Format the string */
  sprintf((char *)str,"image_%d.bmp", (int)counter);
  sprintf((char *)str_display,"         Saving picture in file %s ..", str);
  /* Display text on LTDC layer foreground */
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), str_display, RIGHT_MODE);
  
  Display_StartRefresh();
  Display_WaitRefresh();  
  HAL_Delay(1000);
    
  /* Create and Open a new text file object with write access */
  if(f_open(&MyFile, (const char*)str, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
  {
    Error_Handler();
  }
  else
  {
    /* Write data to the BMP file */
    res1 = f_write(&MyFile, (uint32_t *)aBMPHeader, 54, (void *)&byteswritten);
    
    
    BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"                                          ", RIGHT_MODE);
    BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"           Header BMP written...", RIGHT_MODE);
    
    Display_StartRefresh();     
    Display_WaitRefresh(); 
    HAL_Delay(1000);
    
    BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"                                          ", RIGHT_MODE);
    BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"           BMP pixel data save ongoing...", RIGHT_MODE);
    
    Display_StartRefresh(); 
    Display_WaitRefresh(); 

    
    res2 = f_write(&MyFile, (uint8_t *)((uint8_t *)CONVERTED_FRAME_BUFFER),
    		       (CameraResX * CameraResY * RGB888_BYTE_PER_PIXEL),
    		       (void *)&byteswritten);
  

    if((res1 != FR_OK) || (res2 != FR_OK) || (byteswritten == 0))
    {
      Error_Handler();
    }
    else
    {
      /* Close the open BMP file */
      f_close(&MyFile);

      /* Success of the demo: no error occurrence */
      BSP_LED_On(LED1);
  
     
      BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
      BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"                                          ", RIGHT_MODE);
      sprintf((char *)str_display,"File %s saved on USB disk ..", str);
      BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize()-24), str_display, RIGHT_MODE);
      Display_StartRefresh();       
      Display_WaitRefresh();      
      /* Wait for 2s */
      HAL_Delay(2000);
      
      BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"                                          ", RIGHT_MODE);

      counter++;
      BSP_LED_Off(LED1);
    
      BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
      BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 48), (uint8_t *)"Press TAMPER button to take snapshot", RIGHT_MODE);     
      BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize()-24), (uint8_t *)"Camera Play  ", RIGHT_MODE);
      Display_StartRefresh();
      Display_WaitRefresh(); 

      /* Resume the camera capture */
      BSP_CAMERA_Resume();      
      
    }
  }

}

/**
  * @brief  Main routine for Camera capture
  * @param  None
  * @retval None
  */
static void CAMERA_Capture(void)
{
  static uint8_t buttonState = 0;
  
  if(BSP_PB_GetState(BUTTON_TAMPER) == GPIO_PIN_SET)
  {
    buttonState = 1;
  }
  else if(buttonState == 1)
  {
    buttonState = 0;
    SavePicture();
  }
}

/**
  * @brief  Camera Frame Event callback.
  */
void BSP_CAMERA_FrameEventCallback(void)
{
  BSP_CAMERA_Suspend();
  cameraState = CAMERA_STATE_DISPLAY_ONGOING;
  LCD_LL_ConvertLineToARGB8888((uint32_t *)(CAMERA_FRAME_BUFFER), (uint32_t *)(LCD_FB_START_ADDRESS));

}

/**
  * @brief  Copy the Captured Picture to the display Frame buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @retval None
  */
static void LCD_LL_ConvertLineToARGB8888(void *pSrc, void *pDst)
{
  uint32_t xPos, yPos, destination;   
  
  /* Init DMA2D */
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_ARGB8888;
  hdma2d_eval.Init.OutputOffset = LcdResX - CAMERA_480x272_RES_X;     
  
  /*##-2- DMA2D Callbacks Configuration ######################################*/
  hdma2d_eval.XferCpltCallback  = DMA2D_TransferCompleteCallback;
  
  /*##-3- Foreground Configuration ###########################################*/
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;
  
  hdma2d_eval.Instance = DMA2D;

  /*##-4- DMA2D Initialization     ###########################################*/
  HAL_DMA2D_Init(&hdma2d_eval);
  HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1);
  
  /*##-1- calculate the destination transfer address  ############*/
  xPos = (LcdResX  - CAMERA_480x272_RES_X)/2;
  yPos = (LcdResY  - CAMERA_480x272_RES_Y)/2;  
  
  destination = (uint32_t)pDst + (yPos * LcdResX + xPos) * ARGB8888_BYTE_PER_PIXEL; 
  HAL_DMA2D_Start_IT(&hdma2d_eval, (uint32_t)pSrc, destination, CAMERA_480x272_RES_X, CAMERA_480x272_RES_Y);

}

/**
  * @brief  Prepares the picture to be Saved in USB.
  * @param  None
  * @retval None
  */
static void PicturePrepare(void)
{
  static DMA2D_HandleTypeDef hdma2d_eval;
  uint32_t addrSrc = LCD_FB_START_ADDRESS + ((LcdResX - CameraResX) / 2 * ARGB8888_BYTE_PER_PIXEL);
  uint32_t addrDst = CONVERTED_FRAME_BUFFER;
  uint32_t lineCnt = 0;

  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_RGB888; /* Output color mode at the output of DMA2D operation */
  hdma2d_eval.Init.OutputOffset = 0;

  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;

  hdma2d_eval.Instance = DMA2D;

  /* Go to last line of camera image in LCD BG frame buffer in order to start building BMP file */
  /* As BMP file encoding starts with last line down to the first */
  addrSrc += (LcdResY - ((LcdResY - CameraResY) / 2) - 1 ) * LcdResX * ARGB8888_BYTE_PER_PIXEL;

  /* Convert picture line by line to RGB888 pixel format */
  for(lineCnt = 0; lineCnt < CameraResY; lineCnt++)
  {
    /* DMA2D Initialization */
    if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK)
    {
      if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK)
      {
        if (HAL_DMA2D_Start(&hdma2d_eval, addrSrc, addrDst, CameraResX, 1) == HAL_OK)
        {
          /* Polling For DMA transfer */
          HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
        }
      }
    }
    /* Manage the source and destination buffers */
    addrDst += (CameraResX * RGB888_BYTE_PER_PIXEL);
    addrSrc -= (LcdResX * ARGB8888_BYTE_PER_PIXEL);
  }
}


/**
  * @brief  Initializes the DSI LCD. 
  * The ititialization is done as below:
  *     - DSI PLL ititialization
  *     - DSI ititialization
  *     - LTDC ititialization
  *     - OTM8009A LCD Display IC Driver ititialization
  * @param  None
  * @retval LCD state
  */
static uint8_t LCD_Init(void){
  
  GPIO_InitTypeDef GPIO_Init_Structure;
  DSI_PHY_TimerTypeDef PhyTimings; 
  DSI_CmdCfgTypeDef CmdCfg;
  DSI_LPCmdTypeDef LPCmd;
  DSI_PLLInitTypeDef dsiPllInit;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  
  /* Toggle Hardware Reset of the DSI LCD using
     its XRES signal (active low) */
  BSP_LCD_Reset();
  
  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  BSP_LCD_MspInit();
  
  /* LCD clock configuration */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 417 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 417 MHz / 5 = 83.4 MHz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 83.4 / 2 = 41.7 MHz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 417;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hdsi_eval.Instance = DSI;
  
  HAL_DSI_DeInit(&(hdsi_eval));
  
  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;  

  hdsi_eval.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  hdsi_eval.Init.TXEscapeCkdiv = 0x4;
  HAL_DSI_Init(&(hdsi_eval), &(dsiPllInit));
    
  /* Configure the DSI for Command mode */
  CmdCfg.VirtualChannelID      = 0;
  CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
  CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
  CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.ColorCoding           = DSI_RGB888;
  CmdCfg.CommandSize           = HACT;
  CmdCfg.TearingEffectSource   = DSI_TE_EXTERNAL;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh      = DSI_AR_DISABLE;
  CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
  HAL_DSI_ConfigAdaptedCommandMode(&hdsi_eval, &CmdCfg);
  
  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_ENABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_ENABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_ENABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_ENABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_ENABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_ENABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_ENABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_ENABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_ENABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_ENABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_ENABLE;
  LPCmd.LPMaxReadPacket       = DSI_LP_MRDP_ENABLE;
  LPCmd.AcknowledgeRequest    = DSI_ACKNOWLEDGE_ENABLE;
  HAL_DSI_ConfigCommand(&hdsi_eval, &LPCmd);

  /* Configure DSI PHY HS2LP and LP2HS timings */
  PhyTimings.ClockLaneHS2LPTime = 35;
  PhyTimings.ClockLaneLP2HSTime = 35;
  PhyTimings.DataLaneHS2LPTime = 35;
  PhyTimings.DataLaneLP2HSTime = 35;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 10;
  HAL_DSI_ConfigPhyTimer(&hdsi_eval, &PhyTimings);

  /* Initialize LTDC */
  LTDC_Init();
  
  /* Start DSI */
  HAL_DSI_Start(&(hdsi_eval));
    
  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
  */
  OTM8009A_Init(OTM8009A_COLMOD_RGB888, LCD_ORIENTATION_LANDSCAPE);
  
  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_DISABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_DISABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_DISABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_DISABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_DISABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_DISABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_DISABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_DISABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_DISABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_DISABLE;
  LPCmd.LPMaxReadPacket       = DSI_LP_MRDP_DISABLE;  
  LPCmd.AcknowledgeRequest    = DSI_ACKNOWLEDGE_DISABLE;
  HAL_DSI_ConfigCommand(&hdsi_eval, &LPCmd);
  
  HAL_DSI_ConfigFlowControl(&hdsi_eval, DSI_FLOW_CONTROL_BTA);
  
  
  /* Enable GPIOJ clock */
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  
  /* Configure DSI_TE pin from MB1166 : Tearing effect on separated GPIO from KoD LCD */
  /* that is mapped on GPIOJ2 as alternate DSI function (DSI_TE)                      */
  /* This pin is used only when the LCD and DSI link is configured in command mode    */
  GPIO_Init_Structure.Pin       = GPIO_PIN_2;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DSI;
  HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);   
  
  
  /* Refresh the display */
  HAL_DSI_Refresh(&hdsi_eval);
  
  return LCD_OK;
}

/**
  * @brief  Initialize the LTDC
  * @param  None
  * @retval None
  */
static void LTDC_Init(void)
{
  /* DeInit */
  hltdc_eval.Instance = LTDC;
  HAL_LTDC_DeInit(&hltdc_eval);
  
  /* LTDC Config */
  /* Timing and polarity */
  hltdc_eval.Init.HorizontalSync = HSYNC;
  hltdc_eval.Init.VerticalSync = VSYNC;
  hltdc_eval.Init.AccumulatedHBP = HSYNC+HBP;
  hltdc_eval.Init.AccumulatedVBP = VSYNC+VBP;
  hltdc_eval.Init.AccumulatedActiveH = VSYNC+VBP+VACT;
  hltdc_eval.Init.AccumulatedActiveW = HSYNC+HBP+HACT;
  hltdc_eval.Init.TotalHeigh = VSYNC+VBP+VACT+VFP;
  hltdc_eval.Init.TotalWidth = HSYNC+HBP+HACT+HFP;
  
  /* background value */
  hltdc_eval.Init.Backcolor.Blue = 0;
  hltdc_eval.Init.Backcolor.Green = 0;
  hltdc_eval.Init.Backcolor.Red = 0;
  
  /* Polarity */
  hltdc_eval.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc_eval.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc_eval.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc_eval.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc_eval.Instance = LTDC;

  HAL_LTDC_Init(&hltdc_eval);
}

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
static void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address)
{
  LCD_LayerCfgTypeDef  Layercfg;

  /* Layer Init */
  Layercfg.WindowX0 = 0;
  Layercfg.WindowX1 = BSP_LCD_GetXSize()/2 ;
  Layercfg.WindowY0 = 0;
  Layercfg.WindowY1 = BSP_LCD_GetYSize(); 
  Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  Layercfg.FBStartAdress = Address;
  Layercfg.Alpha = 255;
  Layercfg.Alpha0 = 0;
  Layercfg.Backcolor.Blue = 0;
  Layercfg.Backcolor.Green = 0;
  Layercfg.Backcolor.Red = 0;
  Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  Layercfg.ImageWidth = BSP_LCD_GetXSize() / 2;
  Layercfg.ImageHeight = BSP_LCD_GetYSize();
  
  HAL_LTDC_ConfigLayer(&hltdc_eval, &Layercfg, LayerIndex); 
}

/**
  * @brief  Initialize the BSP LCD Msp.
  * Application surcharge to reduce DSI, DMA2D and LTDC interrupts priorities 
  */
void BSP_LCD_MspInit(void)
{
  /** @brief Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /** @brief Toggle Sw reset of LTDC IP */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();

  /** @brief Enable the DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /** @brief Toggle Sw reset of DMA2D IP */
  __HAL_RCC_DMA2D_FORCE_RESET();
  __HAL_RCC_DMA2D_RELEASE_RESET();

  /** @brief Enable DSI Host and wrapper clocks */
  __HAL_RCC_DSI_CLK_ENABLE();

  /** @brief Soft Reset the DSI Host and wrapper */
  __HAL_RCC_DSI_FORCE_RESET();
  __HAL_RCC_DSI_RELEASE_RESET();

  /** @brief NVIC configuration for LTDC interrupt that is now enabled */
  HAL_NVIC_SetPriority(LTDC_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(LTDC_IRQn);

  /** @brief NVIC configuration for DMA2D interrupt that is now enabled */
  HAL_NVIC_SetPriority(DMA2D_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);

  /** @brief NVIC configuration for DSI interrupt that is now enabled */
  HAL_NVIC_SetPriority(DSI_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(DSI_IRQn);
}

/**
 * @brief  DMA2D Transfer completed callback
 * @param  hdma2d: DMA2D handle.
 * @retval None
 */
static void DMA2D_TransferCompleteCallback(DMA2D_HandleTypeDef *hdma2d)
{
  Display_StartRefresh();
}

/**
  * @brief  Tearing Effect DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_TearingEffectCallback(DSI_HandleTypeDef *hdsi)
{
  /* Mask the TE */
  __DSI_MASK_TE();
  
  /* Refresh the right part of the display */
  HAL_DSI_Refresh(hdsi);   
}

/**
  * @brief  End of Refresh DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
  if(pending_buffer >= 0)
  {
    if(active_area == LEFT_AREA)
    {     
      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(hdsi);
      /* Update LTDC configuration */
      LTDC_LAYER(&hltdc_eval, 0)->CFBAR = LCD_FB_START_ADDRESS + 400 * 4;
      __HAL_LTDC_RELOAD_IMMEDIATE_CONFIG(&hltdc_eval);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(hdsi);
      
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColRight);
      /* Refresh the right part of the display */
      HAL_DSI_Refresh(hdsi);    
      
    }
    else if(active_area == RIGHT_AREA)
    {

      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(&hdsi_eval);
      /* Update LTDC configuration */
      LTDC_LAYER(&hltdc_eval, 0)->CFBAR = LCD_FB_START_ADDRESS;
      __HAL_LTDC_RELOAD_IMMEDIATE_CONFIG(&hltdc_eval);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(&hdsi_eval);
      
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft); 
      
      if(cameraState == CAMERA_STATE_DISPLAY_ONGOING)
      {
        cameraState = CAMERA_STATE_CAPTURE_ONGOING;
        BSP_CAMERA_Resume();
      }
        
      pending_buffer = -1; 

    }
  }
  active_area = (active_area == LEFT_AREA)? RIGHT_AREA : LEFT_AREA; 
}

/**
  * @brief  Wait for Display end of refresh.
  * @param  None
  * @retval None
  */
static void Display_WaitRefresh(void)
{
  while(pending_buffer != -1)
  {
  } 
}

/**
  * @brief  Start display refresh.
  * @param  None
  * @retval None
  */
static void Display_StartRefresh(void)
{
  /* Frame Buffer updated , unmask the DSI TE pin to ask for a DSI refersh*/ 
  pending_buffer = 1;
  /* UnMask the TE */
  __DSI_UNMASK_TE();
}

/**
  * @brief  User Process
  * @param  phost: Host handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
  switch (id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
    Appli_state = STORAGE_IDLE;
    f_mount(NULL, (TCHAR const*)"", 0);
    break;

  case HOST_USER_CLASS_ACTIVE:
    /* Register the file system object to the FatFs module */
    if(f_mount(&MSC_FatFs, (TCHAR const*)MSC_Path, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      Error_Handler();
    }
    else
    {
      Appli_state = STORAGE_READY;
    }
    break;
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  BSP_LCD_SelectLayer(0);
  while(1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 180 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Select PLLSAI output as USB clock source on ck_pll48 (48 MHz) */
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CK48;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CK48CLKSOURCE_PLLSAIP;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
