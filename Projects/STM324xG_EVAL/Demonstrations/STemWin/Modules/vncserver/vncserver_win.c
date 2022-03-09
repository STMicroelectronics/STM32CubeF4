/**
  ******************************************************************************
  * @file    vncserver_win.c
  * @author  MCD Application Team
  * @brief   VNC Server functions
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
#include "DIALOG.h"
#include "k_module.h"
#include "vncserver_res.c"
#include "GUI_VNC.h"
#include "main.h"
#include "vncserver_app.h"
#include "lwip/netif.h"
#include "cmsis_os.h"
#include "ethernetif.h"
#include "k_modules_res.h"

/** @addtogroup VNC_SERVER_MODULE
  * @{
  */

/** @defgroup VNC_SERVER
  * @brief vnc server routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void _cbDialog_VNC(WM_MESSAGE * pMsg);
static void _cbDialog_VNC_SETTINGS(WM_MESSAGE * pMsg);
static void _VNCServer_DisplayIPAddress(void);
static void VNCServer_Start(void);
static void _VNCServer_SaveSettings(void);
static void _VNCServer_RestoreSettings(void);

/* Private typedef -----------------------------------------------------------*/
typedef union  {
    uint32_t d32;
    struct
    {
      uint8_t addr0;
      uint8_t addr1;
      uint8_t addr2;
      uint8_t addr3;
    }b;
}IP_AddressTypeDef;

typedef union  {
    uint32_t d32;
    struct
    {
      uint32_t dhcp       :1;
      uint32_t reserved   :31;
    }b;
}DHCPEnableTypeDef;

 typedef struct
{
  IP_AddressTypeDef ipaddr;      
  IP_AddressTypeDef maskaddr;    
  IP_AddressTypeDef gwaddr;      
  DHCPEnableTypeDef dhcp_use;
}
VNCSettingsTypeDef;
/* Private defines -----------------------------------------------------------*/
#define ID_FRAMEWIN_VNC  (GUI_ID_USER + 0x00)
#define ID_MULTIEDIT_VNC  (GUI_ID_USER + 0x01)
#define ID_BUTTON_SETTINGS  (GUI_ID_USER + 0x02)
#define ID_BUTTON_START  (GUI_ID_USER + 0x03)
#define ID_IMAGE_PCNETWORK  (GUI_ID_USER + 0x07)
#define ID_IMAGE_STATE  (GUI_ID_USER + 0x08)
#define ID_TEXT_DHCP  (GUI_ID_USER + 0x10)
#define ID_TEXT_DHCP_STATE  (GUI_ID_USER + 0x09)
#define ID_TEXT_IPADDR (GUI_ID_USER + 0x0A)
#define ID_TEXT_SUBNETMASK  (GUI_ID_USER + 0x0B)
#define ID_TEXT_GWADDR  (GUI_ID_USER + 0x0C)
#define ID_TEXT_IPADDR_VALUE  (GUI_ID_USER + 0x0D)
#define ID_TEXT_SUBNETMASK_VALUE  (GUI_ID_USER + 0x0E)
#define ID_TEXT_GWADDR_VALUE  (GUI_ID_USER + 0x0F)


#define ID_FRAMEWIN_SETTINGS  (GUI_ID_USER + 0x20)
#define ID_CHECKBOX_DHCP  (GUI_ID_USER + 0x21)
#define ID_TEXT_IPADDRESS  (GUI_ID_USER + 0x22)
#define ID_TEXT_SNMASK  (GUI_ID_USER + 0x23)
#define ID_TEXT_GWADDRESS  (GUI_ID_USER + 0x24)
#define ID_EDIT_IPADDRESS_3  (GUI_ID_USER + 0x25)
#define ID_EDIT_IPADDRESS_2  (GUI_ID_USER + 0x26)
#define ID_EDIT_IPADDRESS_1  (GUI_ID_USER + 0x27)
#define ID_EDIT_IPADDRESS_0  (GUI_ID_USER + 0x28)
#define ID_EDIT_SNMASK_3  (GUI_ID_USER + 0x29)
#define ID_EDIT_SNMASK_2  (GUI_ID_USER + 0x2A)
#define ID_EDIT_SNMASK_1  (GUI_ID_USER + 0x2B)
#define ID_EDIT_SNMASK_0  (GUI_ID_USER + 0x2C)
#define ID_EDIT_GWADDRESS_3  (GUI_ID_USER + 0x2D)
#define ID_EDIT_GWADDRESS_2  (GUI_ID_USER + 0x2E)
#define ID_EDIT_GWADDRESS_1  (GUI_ID_USER + 0x2F)
#define ID_EDIT_GWADDRESS_0  (GUI_ID_USER + 0x30)
#define ID_BUTTON_SETTINGS_CANCEL  (GUI_ID_USER + 0x31)
#define ID_BUTTON_SETTINGS_OK  (GUI_ID_USER + 0x32)

#define VNC_START                  0x00
#define VNC_HIDE                   0x01
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
K_ModuleItem_Typedef  vnc_server =
{
  11,
  "VNC Server",
  &bmvncserver,
  Startup,
  NULL,
}
;

/* _aDialog_VNC */
static const GUI_WIDGET_CREATE_INFO _aDialog_VNC[] = {
  { WINDOW_CreateIndirect, "VNC Server", ID_FRAMEWIN_VNC, 0, 0, 480, 246, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "VNC Server log messages", ID_MULTIEDIT_VNC, 2, 5, 225, 120, 0, 0x0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_PCNETWORK, 245, 140, 64, 64, 0, 0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_STATE, 245, 175, 32, 32, 0, 0, 0 },
  { TEXT_CreateIndirect, "DHCP", ID_TEXT_DHCP, 235, 11, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Enabled", ID_TEXT_DHCP_STATE, 270, 15, 70, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "IP Address:", ID_TEXT_IPADDR, 235, 27, 150, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, " Not assigned", ID_TEXT_IPADDR_VALUE, 232, 43, 150, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Subnet mask:", ID_TEXT_SUBNETMASK, 232, 59, 150, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, " Not assigned", ID_TEXT_SUBNETMASK_VALUE, 232, 75, 150, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "GW Address:", ID_TEXT_GWADDR, 235, 91, 150, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, " Not assigned", ID_TEXT_GWADDR_VALUE, 232, 107, 150, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Settings", ID_BUTTON_SETTINGS, 17, 150, 90, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Start", ID_BUTTON_START, 122, 150, 90, 40, 0, 0x0, 0 },
};

/* _aDialog_VNC_SETTINGS */
static const GUI_WIDGET_CREATE_INFO _aDialog_VNC_SETTINGS[] = {
  { FRAMEWIN_CreateIndirect, "VNC Server Settings", ID_FRAMEWIN_SETTINGS, 0, 0, 210, 180, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "DHCP:", ID_CHECKBOX_DHCP, 7, 13, 102, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "IP Address:", ID_TEXT_IPADDRESS, 3, 40, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, " Subnet Mask:", ID_TEXT_SNMASK, 3, 65, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Getaway Address:", ID_TEXT_GWADDRESS, 3, 90, 96, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_IPADDRESS_3, 95, 40, 25, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_IPADDRESS_2, 120, 40, 25, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_IPADDRESS_1, 145, 40, 25, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_IPADDRESS_0, 170, 40, 25, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_SNMASK_3, 95, 65, 25, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_SNMASK_2, 120, 65, 25, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_SNMASK_1, 145, 65, 25, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_SNMASK_0, 170, 65, 25, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_GWADDRESS_3, 95, 90, 25, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_GWADDRESS_2, 120, 90, 25, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_GWADDRESS_1, 145, 90, 25, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_GWADDRESS_0, 170, 90, 25, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_BUTTON_SETTINGS_CANCEL, 147, 115, 40, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_BUTTON_SETTINGS_OK, 102, 115, 40, 30, 0, 0x0, 0 },
};

/* Bitmap data for arrow keys */
static GUI_CONST_STORAGE GUI_COLOR _aColorsArrow[] = {
  0xFFFFFF, 0x000000
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _PalArrow = {
  2,	/* number of entries */
  1, 	/* No transparency */
  &_aColorsArrow[0]
};

static GUI_CONST_STORAGE unsigned char _acArrowRight[] = {
  ____XX__, ________,
  ____XXXX, ________,
  XXXXXXXX, XX______,
  ____XXXX, ________,
  ____XX__, ________
};

static GUI_CONST_STORAGE unsigned char _acArrowLeft[] = {
  ____XX__, ________,
  __XXXX__, ________,
  XXXXXXXX, XX______,
  __XXXX__, ________,
  ____XX__, ________
};

static GUI_CONST_STORAGE GUI_BITMAP _bmArrowRight = {
  10,           /* XSize */
  5,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  _acArrowRight, /* Pointer to picture data (indices) */
  &_PalArrow     /* Pointer to palette */
};

static GUI_CONST_STORAGE GUI_BITMAP _bmArrowLeft = {
  10,           /* XSize */
  5,            /* YSize */
  2,            /* BytesPerLine */
  1,            /* BitsPerPixel */
  _acArrowLeft,  /* Pointer to picture data (indices) */
  &_PalArrow     /* Pointer to palette */
};

/* Array of keys */
static int _aKey[] = {GUI_KEY_DELETE, GUI_KEY_TAB, GUI_KEY_LEFT, GUI_KEY_RIGHT};

/* Dialog resource of numpad */
static const GUI_WIDGET_CREATE_INFO _aDialogNumPad[] = {
/*  Function                 Text      Id                 Px   Py   Dx   Dy */
  { WINDOW_CreateIndirect,   0,        0,             223, 83,  95, 130},
  { BUTTON_CreateIndirect,   "7",      GUI_ID_USER +  7,   5,   5,  25,  20},
  { BUTTON_CreateIndirect,   "8",      GUI_ID_USER +  8,  35,   5,  25,  20},
  { BUTTON_CreateIndirect,   "9",      GUI_ID_USER +  9,  65,   5,  25,  20},
  { BUTTON_CreateIndirect,   "4",      GUI_ID_USER +  4,   5,  30,  25,  20},
  { BUTTON_CreateIndirect,   "5",      GUI_ID_USER +  5,  35,  30,  25,  20},
  { BUTTON_CreateIndirect,   "6",      GUI_ID_USER +  6,  65,  30,  25,  20},
  { BUTTON_CreateIndirect,   "1",      GUI_ID_USER +  1,   5,  55,  25,  20},
  { BUTTON_CreateIndirect,   "2",      GUI_ID_USER +  2,  35,  55,  25,  20},
  { BUTTON_CreateIndirect,   "3",      GUI_ID_USER +  3,  65,  55,  25,  20},
  { BUTTON_CreateIndirect,   "0",      GUI_ID_USER +  0,   5,  80,  25,  20},
  { BUTTON_CreateIndirect,   ".",      GUI_ID_USER + 10,  35,  80,  25,  20},
  { BUTTON_CreateIndirect,   "Del",    GUI_ID_USER + 11,  65,  80,  25,  20},
  { BUTTON_CreateIndirect,   "Tab",    GUI_ID_USER + 12,   5, 105,  25,  20},
  { BUTTON_CreateIndirect,   0,        GUI_ID_USER + 13,  35, 105,  25,  20},
  { BUTTON_CreateIndirect,   0,        GUI_ID_USER + 14,  65, 105,  25,  20},
};

WM_HWIN hNumPad = 0;
WM_HWIN hWinVNC = 0;
WM_HWIN hWinVNC_Settings = 0;
uint8_t TCPIP_Initialized = 0;
uint8_t VNC_Initialized = 0;
uint8_t link_status = 0;
uint8_t start_button = VNC_START;
VNCSettingsTypeDef  VNCSettings;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Callback function of the numpad.
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialogNumPad(WM_MESSAGE * pMsg) {
  GUI_RECT r;
  int i, NCode, Id, Pressed = 0;
  WM_HWIN hDlg, hItem;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_PAINT:
    WM_GetClientRect(&r);
    GUI_SetColor(0x000000);
    GUI_DrawRect(r.x0, r.y0, r.x1, r.y1);          /* Draw rectangle around it */
    /* Draw the bright sides */
    GUI_SetColor(0xffffff);
    GUI_DrawHLine(r.y0 + 1, r.x0 + 1, r.x1 - 2);   /* Draw top line */
    GUI_DrawVLine(r.x0 + 1, r.y0 + 1, r.y1 - 2);
    /* Draw the dark sides */
    GUI_SetColor(0x555555);
    GUI_DrawHLine(r.y1-1, r.x0 + 1, r.x1 - 1);
    GUI_DrawVLine(r.x1-1, r.y0 + 1, r.y1 - 2);
    break;
  case WM_INIT_DIALOG:
    for (i = 0; i < GUI_COUNTOF(_aDialogNumPad) - 1; i++) {
      hItem = WM_GetDialogItem(hDlg, GUI_ID_USER + i);
      BUTTON_SetFocussable(hItem, 0);                       /* Set all buttons non focussable */
      switch (i) {
      case 13:
        BUTTON_SetBitmapEx(hItem, 0, &_bmArrowLeft, 7, 7);  /* Set bitmap for arrow left button (unpressed) */
        BUTTON_SetBitmapEx(hItem, 1, &_bmArrowLeft, 7, 7);  /* Set bitmap for arrow left button (pressed) */
        break;
      case 14:
        BUTTON_SetBitmapEx(hItem, 0, &_bmArrowRight, 7, 7); /* Set bitmap for arrow right button (unpressed) */
        BUTTON_SetBitmapEx(hItem, 1, &_bmArrowRight, 7, 7); /* Set bitmap for arrow right button (pressed) */
        break;
      }
    }
    hItem = WM_GetDialogItem(hDlg, GUI_ID_USER + 12);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
    NCode = pMsg->Data.v;                 /* Notification code */
    switch (NCode) {
    case WM_NOTIFICATION_CLICKED:
      Pressed = 1;
    case WM_NOTIFICATION_RELEASED:
      if ((Id >= GUI_ID_USER) && (Id <= (GUI_ID_USER + GUI_COUNTOF(_aDialogNumPad) - 1))) {
        int Key;
        if (Id < GUI_ID_USER + 11) {
          char acBuffer[10];
          BUTTON_GetText(pMsg->hWinSrc, acBuffer, sizeof(acBuffer)); /* Get the text of the button */
          Key = acBuffer[0];
        } else {
          Key = _aKey[Id - GUI_ID_USER - 11];                        /* Get the text from the array */
        }
        GUI_SendKeyMsg(Key, Pressed);                                /* Send a key message to the focused window */
      }
      break;
    }
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Callback routine of the VNC dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog_VNC(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    /* Initialization of 'Multi Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_VNC);
    MULTIEDIT_SetReadOnly(hItem, 1);
    MULTIEDIT_SetBkColor(hItem, 1, 0);
    MULTIEDIT_SetTextColor(hItem, 1, 0xFFFFFF);

    /* Initialization of 'Image 1' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_PCNETWORK);
    IMAGE_SetBitmap(hItem, &bmpc_network64);

    /* Initialization of 'Image 2' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_STATE);
    IMAGE_SetBitmap(hItem, &bmDelete32);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DHCP);
    TEXT_SetFont(hItem, GUI_FONT_13B_1);
    TEXT_SetTextColor(hItem, GUI_BLACK);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DHCP_STATE);
    TEXT_SetFont(hItem, GUI_FONT_8_ASCII);
    TEXT_SetTextColor(hItem, GUI_BLACK);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_IPADDR);
    TEXT_SetFont(hItem, GUI_FONT_13B_1);
    TEXT_SetTextColor(hItem, GUI_BLACK);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_IPADDR_VALUE);
    TEXT_SetFont(hItem, GUI_FONT_8_ASCII);
    TEXT_SetTextColor(hItem, GUI_BLACK);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SUBNETMASK);
    TEXT_SetFont(hItem, GUI_FONT_13B_1);
    TEXT_SetTextColor(hItem, GUI_BLACK);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SUBNETMASK_VALUE);
    TEXT_SetFont(hItem, GUI_FONT_8_ASCII);
    TEXT_SetTextColor(hItem, GUI_BLACK);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_GWADDR);
    TEXT_SetFont(hItem, GUI_FONT_13B_1);
    TEXT_SetTextColor(hItem, GUI_BLACK);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_GWADDR_VALUE);
    TEXT_SetFont(hItem, GUI_FONT_8_ASCII);
    TEXT_SetTextColor(hItem, GUI_BLACK); 
    
    /* Restore VNC settings */
    _VNCServer_RestoreSettings();
    
    if (VNCSettings.maskaddr.b.addr3 != 255)
    {
      VNCSettings.dhcp_use.b.dhcp = 1;
      VNCSettings.ipaddr.b.addr0 = 0;
      VNCSettings.ipaddr.b.addr1 = 0;
      VNCSettings.ipaddr.b.addr2 = 0;
      VNCSettings.ipaddr.b.addr3 = 0;
      VNCSettings.maskaddr.b.addr0 = 0;
      VNCSettings.maskaddr.b.addr1 = 0;
      VNCSettings.maskaddr.b.addr2 = 0;
      VNCSettings.maskaddr.b.addr3 = 0;
      VNCSettings.gwaddr.b.addr0 = 0;
      VNCSettings.gwaddr.b.addr1 = 0;
      VNCSettings.gwaddr.b.addr2 = 0;
      VNCSettings.gwaddr.b.addr3 = 0;
      
      /* Save VNC settings */
      _VNCServer_SaveSettings();
    }
    else
    {
      _VNCApp_GetIPAddress(IP_ADDRESS, VNCSettings.ipaddr.b.addr3, VNCSettings.ipaddr.b.addr2, VNCSettings.ipaddr.b.addr1, VNCSettings.ipaddr.b.addr0);
      _VNCApp_GetIPAddress(SUBNET_MASK, VNCSettings.maskaddr.b.addr3, VNCSettings.maskaddr.b.addr2, VNCSettings.maskaddr.b.addr1, VNCSettings.maskaddr.b.addr0);
      _VNCApp_GetIPAddress(GW_ADDRESS, VNCSettings.gwaddr.b.addr3, VNCSettings.gwaddr.b.addr2, VNCSettings.gwaddr.b.addr1, VNCSettings.gwaddr.b.addr0);
    }     
    
    if (VNCSettings.dhcp_use.b.dhcp == 0)
    {
       _VNCServer_DisplayIPAddress();
    }
    
    
    break;

  case WM_PAINT:
    
    DrawRect3D(230, 5, 87, 120);  
    DrawRect3D(2, 130, 225, 80);
    DrawRect3D(230, 130, 87, 80);
    
    break;
      
  case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    
    switch (NCode) 
    {    
      
    case WM_NOTIFICATION_RELEASED:      /* React only if released */
      switch (Id) 
      {
      case ID_BUTTON_SETTINGS: /* Settings */
        if(hWinVNC_Settings == 0)
        {
          hWinVNC_Settings = GUI_CreateDialogBox(_aDialog_VNC_SETTINGS, GUI_COUNTOF(_aDialog_VNC_SETTINGS), _cbDialog_VNC_SETTINGS, hWinVNC, 0, 10);
          
          hNumPad = GUI_CreateDialogBox(_aDialogNumPad, 
                                        GUI_COUNTOF(_aDialogNumPad), 
                                        _cbDialogNumPad, pMsg->hWin, 0, 0); 
        }
        break;	  
      case ID_BUTTON_START: /* Start */
        
        if(hWinVNC_Settings == 0)
        {
          if (start_button == VNC_START) 
          {          
            /* change start button to hide */
            start_button = VNC_HIDE;
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_START);
            BUTTON_SetText(hItem, (const char*) "Hide");
            
            /* Change connection png pic from disconnected to connecting */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_STATE);
            IMAGE_SetBitmap(hItem, &bmhourglass32);
            
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_VNC);
            MULTIEDIT_AddText(hItem, (const char *) "TCP/IP stack initialization ... \n\r");
            
            TCPIP_Initialized = 1;
            
            /* Initilaize the LwIP stack */
            NetworkInit(VNCSettings.dhcp_use.b.dhcp);
          }
          else /* start_button == VNC_HIDE  */
          {
            if(hWinVNC_Settings == 0)
            {        
              WM_HideWindow(pMsg->hWin);
            }
          }
        }
        break;
      }
      break;  
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

static void _cbDialog_VNC_SETTINGS(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG: 
    
    /* Restore VNC settings */
    _VNCServer_RestoreSettings(); 

    /* Initialization of 'VNC Server Settings' */
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 18);

    /* Initialization of 'DHCP' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_DHCP);
    CHECKBOX_SetText(hItem, "Enable DHCP");
    CHECKBOX_SetState(hItem, VNCSettings.dhcp_use.b.dhcp);

    /* Initialization of 'Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_3);
    EDIT_SetDecMode(hItem, VNCSettings.ipaddr.b.addr3, 0, 255, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);

    /* Initialization of 'Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_2);
    EDIT_SetDecMode(hItem, VNCSettings.ipaddr.b.addr2, 0, 255, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);

    /* Initialization of 'Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_1);
    EDIT_SetDecMode(hItem, VNCSettings.ipaddr.b.addr1, 0, 255, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);

    /* Initialization of 'Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_0);
    EDIT_SetDecMode(hItem, VNCSettings.ipaddr.b.addr0, 0, 255, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);

    /* Initialization of 'Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_3);
    EDIT_SetDecMode(hItem, VNCSettings.maskaddr.b.addr3, 0, 255, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);

    /* Initialization of 'Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_2);
    EDIT_SetDecMode(hItem, VNCSettings.maskaddr.b.addr2, 0, 255, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);

    /* Initialization of 'Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_1);
    EDIT_SetDecMode(hItem, VNCSettings.maskaddr.b.addr1, 0, 255, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);

    /* Initialization of 'Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_0);
    EDIT_SetDecMode(hItem, VNCSettings.maskaddr.b.addr0, 0, 255, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);

    /* Initialization of 'Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_3);
    EDIT_SetDecMode(hItem, VNCSettings.gwaddr.b.addr3, 0, 255, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);

    /* Initialization of 'Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_2);
    EDIT_SetDecMode(hItem, VNCSettings.gwaddr.b.addr2, 0, 255, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);

    /* Initialization of 'Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_1);
    EDIT_SetDecMode(hItem, VNCSettings.gwaddr.b.addr1, 0, 255, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES);

    /* Initialization of 'Edit' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_0);
    EDIT_SetDecMode(hItem, VNCSettings.gwaddr.b.addr0, 0, 255, 0, GUI_EDIT_SUPPRESS_LEADING_ZEROES); 
    
    /* If DHCP enabled: disable all Edits*/
    if (VNCSettings.dhcp_use.b.dhcp)
    {
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_3);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_2);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_1);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_0);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_3);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_2);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_1);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_0);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_3);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_2);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_1);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_0);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0); 
    }
    break;
    

  case WM_DELETE:
    hWinVNC_Settings = 0;
    break;
    
  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    switch (NCode) {
    case WM_NOTIFICATION_RELEASED:      /* React only if released */
      switch (Id) {
      case ID_BUTTON_SETTINGS_CANCEL: /* Settings: Cancel */
        /* Delete NumPad VNC Window */
        WM_DeleteWindow(hNumPad);
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      case ID_BUTTON_SETTINGS_OK: /* Settings: OK */       
        
        /* Save Settings */
        hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_DHCP);
        if (CHECKBOX_GetState(hItem)) /* Use DHCP to allocate dynamic IP address */
        {
          if(VNCSettings.dhcp_use.b.dhcp == 0)
          {
            VNCSettings.dhcp_use.b.dhcp = 1;
            VNCSettings.ipaddr.b.addr0 = 0;
            VNCSettings.ipaddr.b.addr1 = 0;
            VNCSettings.ipaddr.b.addr2 = 0;
            VNCSettings.ipaddr.b.addr3 = 0;
            VNCSettings.maskaddr.b.addr0 = 0;
            VNCSettings.maskaddr.b.addr1 = 0;
            VNCSettings.maskaddr.b.addr2 = 0;
            VNCSettings.maskaddr.b.addr3 = 0;
            VNCSettings.gwaddr.b.addr0 = 0;
            VNCSettings.gwaddr.b.addr1 = 0;
            VNCSettings.gwaddr.b.addr2 = 0;
            VNCSettings.gwaddr.b.addr3 = 0;
            
            hItem = WM_GetDialogItem(hWinVNC, ID_TEXT_DHCP_STATE);
            TEXT_SetText(hItem, (char const *)"Enabled");  
            
            hItem = WM_GetDialogItem(hWinVNC, ID_TEXT_IPADDR_VALUE);
            TEXT_SetText(hItem, (char const *)"Not assigned");
            
            hItem = WM_GetDialogItem(hWinVNC, ID_TEXT_SUBNETMASK_VALUE);
            TEXT_SetText(hItem, (char const *)"Not assigned");
            
            hItem = WM_GetDialogItem(hWinVNC, ID_TEXT_GWADDR_VALUE);
            TEXT_SetText(hItem, (char const *)"Not assigned");
            
            if (start_button == VNC_HIDE)
            {
              hItem = WM_GetDialogItem(hWinVNC, ID_MULTIEDIT_VNC);
              MULTIEDIT_AddText(hItem, (const char *) "Network settings has been modified\r\n");
              MULTIEDIT_AddText(hItem, (const char *) "Restart to apply new settings\r\n"); 
            }
          }
        } /* Use static IP address */
        else
        {
          if (VNCSettings.dhcp_use.b.dhcp == 1)
          {
            VNCSettings.dhcp_use.b.dhcp = 0;
          }

          if (start_button == VNC_HIDE)
          {
            hItem = WM_GetDialogItem(hWinVNC, ID_MULTIEDIT_VNC);
            MULTIEDIT_AddText(hItem, (const char *) "Network settings has been modified\r\n");
            MULTIEDIT_AddText(hItem, (const char *) "Restart to apply new settings\r\n"); 
          }
          
          /* Get the new network addresses configuration */
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_3);
          VNCSettings.ipaddr.b.addr3 = EDIT_GetValue(hItem);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_2);
          VNCSettings.ipaddr.b.addr2 = EDIT_GetValue(hItem);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_1);
          VNCSettings.ipaddr.b.addr1 = EDIT_GetValue(hItem);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_0);
          VNCSettings.ipaddr.b.addr0 = EDIT_GetValue(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_3);
          VNCSettings.maskaddr.b.addr3 = EDIT_GetValue(hItem);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_2);
          VNCSettings.maskaddr.b.addr2 = EDIT_GetValue(hItem);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_1);
          VNCSettings.maskaddr.b.addr1 = EDIT_GetValue(hItem);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_0);
          VNCSettings.maskaddr.b.addr0 = EDIT_GetValue(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_3);
          VNCSettings.gwaddr.b.addr3 = EDIT_GetValue(hItem);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_2);
          VNCSettings.gwaddr.b.addr2 = EDIT_GetValue(hItem);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_1);
          VNCSettings.gwaddr.b.addr1 = EDIT_GetValue(hItem);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_0);
          VNCSettings.gwaddr.b.addr0 = EDIT_GetValue(hItem);
          
          _VNCApp_GetIPAddress(IP_ADDRESS, VNCSettings.ipaddr.b.addr3, VNCSettings.ipaddr.b.addr2, VNCSettings.ipaddr.b.addr1, VNCSettings.ipaddr.b.addr0);
          _VNCApp_GetIPAddress(SUBNET_MASK, VNCSettings.maskaddr.b.addr3, VNCSettings.maskaddr.b.addr2, VNCSettings.maskaddr.b.addr1, VNCSettings.maskaddr.b.addr0);
          _VNCApp_GetIPAddress(GW_ADDRESS, VNCSettings.gwaddr.b.addr3, VNCSettings.gwaddr.b.addr2, VNCSettings.gwaddr.b.addr1, VNCSettings.gwaddr.b.addr0);
        }
        
        /********** Save Settings ***********/
        _VNCServer_SaveSettings(); 
        
        /* Delete NumPad VNC Window */
        WM_DeleteWindow(hNumPad);
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;
    case WM_NOTIFICATION_VALUE_CHANGED:
      switch (Id) {
      case ID_CHECKBOX_DHCP:
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_DHCP);
          if (CHECKBOX_IsChecked(hItem))
          {
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_3);
            WM_DisableWindow(hItem);
            EDIT_SetFocussable(hItem, 0);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_2);
            WM_DisableWindow(hItem);
            EDIT_SetFocussable(hItem, 0);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_1);
            WM_DisableWindow(hItem);
            EDIT_SetFocussable(hItem, 0);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_0);
            WM_DisableWindow(hItem);
            EDIT_SetFocussable(hItem, 0);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_3);
            WM_DisableWindow(hItem);
            EDIT_SetFocussable(hItem, 0);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_2);
            WM_DisableWindow(hItem);
            EDIT_SetFocussable(hItem, 0);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_1);
            WM_DisableWindow(hItem);
            EDIT_SetFocussable(hItem, 0);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_0);
            WM_DisableWindow(hItem);
            EDIT_SetFocussable(hItem, 0);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_3);
            WM_DisableWindow(hItem);
            EDIT_SetFocussable(hItem, 0);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_2);
            WM_DisableWindow(hItem);
            EDIT_SetFocussable(hItem, 0);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_1);
            WM_DisableWindow(hItem);
            EDIT_SetFocussable(hItem, 0);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_0);
            WM_DisableWindow(hItem);
            EDIT_SetFocussable(hItem, 0);
          }
          else
          {
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_3);
            WM_EnableWindow(hItem);
            EDIT_SetFocussable(hItem, 1);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_2);
            WM_EnableWindow(hItem);
            EDIT_SetFocussable(hItem, 1);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_1);
            WM_EnableWindow(hItem);
            EDIT_SetFocussable(hItem, 1);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IPADDRESS_0);
            WM_EnableWindow(hItem);
            EDIT_SetFocussable(hItem, 1);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_3);
            WM_EnableWindow(hItem);
            EDIT_SetFocussable(hItem, 1);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_2);
            WM_EnableWindow(hItem);
            EDIT_SetFocussable(hItem, 1);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_1);
            WM_EnableWindow(hItem);
            EDIT_SetFocussable(hItem, 1);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_SNMASK_0);
            WM_EnableWindow(hItem);
            EDIT_SetFocussable(hItem, 1);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_3);
            WM_EnableWindow(hItem);
            EDIT_SetFocussable(hItem, 1);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_2);
            WM_EnableWindow(hItem);
            EDIT_SetFocussable(hItem, 1);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_1);
            WM_EnableWindow(hItem);
            EDIT_SetFocussable(hItem, 1);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GWADDRESS_0);
            WM_EnableWindow(hItem);
            EDIT_SetFocussable(hItem, 1);
          }
        }
        break;
      }
      break;
      
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  VNC window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  if (hWinVNC == 0)
  {
    hWinVNC = GUI_CreateDialogBox(_aDialog_VNC, GUI_COUNTOF(_aDialog_VNC), _cbDialog_VNC, hWin, xpos, ypos);
  }
  else
  {
    WM_ShowWindow(hWinVNC);
  }  
}

/**
  * @brief  Start VNC server
  * @param  None
  * @retval None
  */
static void VNCServer_Start(void)
{
  WM_HWIN hItem;
  
  hItem = WM_GetDialogItem(hWinVNC, ID_MULTIEDIT_VNC);
  MULTIEDIT_AddText(hItem, (const char *) "VNC Server initialization ...\r\n");
  
  /* Start VNC */
  if (GUI_VNC_X_StartServer(0, 0))
  {
    
    VNC_Initialized = 1;
    hItem = WM_GetDialogItem(hWinVNC, ID_MULTIEDIT_VNC);
    MULTIEDIT_AddText(hItem, (const char *) "The VNC Server is started \r\n"); 
    
    /* Change connection png pic from connecting to connected */
    hItem = WM_GetDialogItem(hWinVNC, ID_IMAGE_STATE);
    IMAGE_SetBitmap(hItem, &bmok32);
    
  }
  else /* Failed to start VNC server */
  {
    hItem = WM_GetDialogItem(hWinVNC, ID_MULTIEDIT_VNC);
    MULTIEDIT_AddText(hItem, (const char *) "Failed: The VNC server cannot start\r\n");
    
    /* Change connection png pic from connecting to disconnected */
    hItem = WM_GetDialogItem(hWinVNC, ID_IMAGE_STATE);
    IMAGE_SetBitmap(hItem, &bmDelete32);      
  }
}

/**
  * @brief  Display assigned IP, Mask and GW addresses on the VNC dialog
  * @param  None
  * @retval None
  */
static void _VNCServer_DisplayIPAddress(void)
{
  WM_HWIN hItem;
  uint8_t iptxt[30];
  
  hItem = WM_GetDialogItem(hWinVNC, ID_TEXT_DHCP_STATE);
  if (VNCSettings.dhcp_use.b.dhcp)
    TEXT_SetText(hItem, (char const *)"Enabled");
  else
    TEXT_SetText(hItem, (char const *)"Disabled");
  
  sprintf((char*)iptxt, " %d.%d.%d.%d\n", VNCSettings.ipaddr.b.addr3, VNCSettings.ipaddr.b.addr2, VNCSettings.ipaddr.b.addr1, VNCSettings.ipaddr.b.addr0);
  hItem = WM_GetDialogItem(hWinVNC, ID_TEXT_IPADDR_VALUE);
  TEXT_SetText(hItem, (char const *)iptxt);
  
  sprintf((char*)iptxt, " %d.%d.%d.%d\n", VNCSettings.maskaddr.b.addr3, VNCSettings.maskaddr.b.addr2, VNCSettings.maskaddr.b.addr1, VNCSettings.maskaddr.b.addr0);
  hItem = WM_GetDialogItem(hWinVNC, ID_TEXT_SUBNETMASK_VALUE);
  TEXT_SetText(hItem, (char const *)iptxt);
  
  sprintf((char*)iptxt, " %d.%d.%d.%d\n", VNCSettings.gwaddr.b.addr3, VNCSettings.gwaddr.b.addr2, VNCSettings.gwaddr.b.addr1, VNCSettings.gwaddr.b.addr0);
  hItem = WM_GetDialogItem(hWinVNC, ID_TEXT_GWADDR_VALUE);
  TEXT_SetText(hItem, (char const *)iptxt);
}

/**
  * @brief  Save VNC settings to backup memory 
  * @param  None
  * @retval None
  */
static void _VNCServer_SaveSettings(void)
{
  *((IP_AddressTypeDef *)ETHERNET_SAVE_PARAM) = VNCSettings.ipaddr; 
  *((IP_AddressTypeDef *)(ETHERNET_SAVE_PARAM + 4)) = VNCSettings.maskaddr; 
  *((IP_AddressTypeDef *)(ETHERNET_SAVE_PARAM + 8)) = VNCSettings.gwaddr; 
  *((DHCPEnableTypeDef *)(ETHERNET_SAVE_PARAM + 12)) = VNCSettings.dhcp_use; 
}

/**
  * @brief  Restore VNC settings from backup memory 
  * @param  None
  * @retval None
  */
static void _VNCServer_RestoreSettings(void)
{
   VNCSettings.ipaddr =  *((IP_AddressTypeDef *)ETHERNET_SAVE_PARAM);
   VNCSettings.maskaddr =  *((IP_AddressTypeDef *)(ETHERNET_SAVE_PARAM + 4));
   VNCSettings.gwaddr =  *((IP_AddressTypeDef *)(ETHERNET_SAVE_PARAM + 8));
   VNCSettings.dhcp_use =  *((DHCPEnableTypeDef *)(ETHERNET_SAVE_PARAM + 12));
}

/**
  * @brief  Process notification received from VNC Application
  * @param  IP: notification ID
  * @retval None
  */
void _VNCServer_Notify(uint8_t ID)
{
  WM_HWIN hItem;
  
  switch (ID)
  {
  case NOTIFY_SERVER_NETIF_UP:
    {
      hItem = WM_GetDialogItem(hWinVNC, ID_MULTIEDIT_VNC);
      MULTIEDIT_AddText(hItem, (const char *) "The network cable is connected \n\r");
      
      /* update the link status*/
      link_status = 1;
      
      if (VNCSettings.dhcp_use.b.dhcp == 0)
      {
        /* Display IP address */ 
        _VNCServer_DisplayIPAddress();
        
        if (VNC_Initialized == 0)
        {
          VNCServer_Start();
        }
      }
      break;
    }
  case NOTIFY_SERVER_NETIF_DOWN:
    {
      hItem = WM_GetDialogItem(hWinVNC, ID_MULTIEDIT_VNC);
      MULTIEDIT_AddText(hItem, (const char *) "The network cable is not connected \n\r");
      
      /* Change connection png pic from connecting to disconnected */
      hItem = WM_GetDialogItem(hWinVNC, ID_IMAGE_STATE);
      IMAGE_SetBitmap(hItem, &bmDelete32);
      
      /* update the link status*/
      link_status = 0;
      
      break;
    }
  case NOTIFY_SERVER_DHCP_START:
    {
      hItem = WM_GetDialogItem(hWinVNC, ID_MULTIEDIT_VNC);
      MULTIEDIT_AddText(hItem, (const char *) "Starting DHCP client ...\n\r");
      
      break;
    } 
  case NOTIFY_SERVER_DHCP_WAIT_ADDRESS:
    {
      hItem = WM_GetDialogItem(hWinVNC, ID_MULTIEDIT_VNC);
      MULTIEDIT_AddText(hItem, (const char *) "Looking for a DHCP sever ...\n\r");
      
      /* Change connection png pic from connected to connecting */
      hItem = WM_GetDialogItem(hWinVNC, ID_IMAGE_STATE);
      IMAGE_SetBitmap(hItem, &bmhourglass32);
      
      break;
    } 
  case NOTIFY_SERVER_DHCP_ADDRESS_ASSIGNED:
    {
      hItem = WM_GetDialogItem(hWinVNC, ID_MULTIEDIT_VNC);
      MULTIEDIT_AddText(hItem, (const char *) "IP Address assigned by a DHCP server\n\r");
      
      /* Display IP address */ 
      _VNCServer_DisplayIPAddress();
      
      if (VNC_Initialized == 0)
      {
        VNCServer_Start();
      }
      break;
    }
  case NOTIFY_SERVER_DHCP_TIMEOUT:
    {
      hItem = WM_GetDialogItem(hWinVNC, ID_MULTIEDIT_VNC);
      MULTIEDIT_AddText(hItem, (const char *) "DHCP timeout !!\n\r");
      MULTIEDIT_AddText(hItem, (const char *) "A default IP address is assigned:\n\r");
      
      /* Display IP address */ 
      _VNCServer_DisplayIPAddress();
      
      if (VNC_Initialized == 0)
      {
        VNCServer_Start();
      }
      
      break;
    }
  default: break;
  }
}

/**
  * @brief  Gets IP, MASK or GW address from VNC app
  * @param  type: type of address can be (IP_ADDRESS, SUBNET_MASK, GW_ADDRESS) 
  * @param  addrx: x: 3 -> 0 specify the byte of the address
  * @retval None
  */
void _VNCServer_GetAssignedAddress(uint8_t type, uint8_t addr3, uint8_t addr2, uint8_t addr1, uint8_t addr0)
{
  switch (type)
  {
  case IP_ADDRESS:
    VNCSettings.ipaddr.b.addr3 = addr3;
    VNCSettings.ipaddr.b.addr2 = addr2;
    VNCSettings.ipaddr.b.addr1 = addr1;
    VNCSettings.ipaddr.b.addr0 = addr0;
    break;
  case SUBNET_MASK:
    VNCSettings.maskaddr.b.addr3 = addr3;
    VNCSettings.maskaddr.b.addr2 = addr2;
    VNCSettings.maskaddr.b.addr1 = addr1;
    VNCSettings.maskaddr.b.addr0 = addr0;
    break;
  case GW_ADDRESS:
    VNCSettings.gwaddr.b.addr3 = addr3;
    VNCSettings.gwaddr.b.addr2 = addr2;
    VNCSettings.gwaddr.b.addr1 = addr1;
    VNCSettings.gwaddr.b.addr0 = addr0;
    break;
  }
}
/**
  * @}
  */

/**
  * @}
  */
