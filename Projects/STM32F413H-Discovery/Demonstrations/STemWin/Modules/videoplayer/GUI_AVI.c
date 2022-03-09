/**
  ******************************************************************************
  * @file    GUI_AVI.c 
  * @author  MCD Application Team
  * @brief   This file provides routines for AVI decoding.
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
#include <string.h>

#include "GUI_Private.h"
#include "GUI_AVI.h"

#if GUI_WINSUPPORT
  #include "WM.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/			
U8* const AVI_VIDS_FLAG_TBL[2]={(U8 *)"00dc",(U8 *)"01dc"};
U8* const AVI_AUDS_FLAG_TBL[2]={(U8 *)"00wb",(U8 *)"01wb"};

int status = 0;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Search AVI ID
  * @param  None 
  * @retval ID
  */
U16 _AVI_SearchID(U8* buf,U16 size,U8 *id)
{
  U16 i;
  size-=4;
  for(i=0;i<size;i++)
  {
    if(buf[i] == id[0])
      if(buf[i+1] == id[1])
        if(buf[i+2] == id[2])	
          if(buf[i+3] == id[3])
            return i;	
  }
  return 0;		
}

/**
  * @brief  Get AVI file information
  * @param  havi: AVI handle
  * @param  buf:   
  * @retval AVI status
  */
AVISTATUS _AVI_GetStreamInfo(GUI_AVI_HANDLE havi, U8* buf)
{
  AVI_CONTEXT * pavi;
  
  if (havi) {
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {  
      
      pavi->aviInfo.StreamID = MAKEWORD (buf+2);		
      pavi->aviInfo.StreamSize = MAKEDWORD (buf+4);	
      
      if(pavi->aviInfo.StreamSize%2)
        pavi->aviInfo.StreamSize++;
      
      if((pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG) || (pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG))
      {
        GUI_UNLOCK_H(pavi);
        return AVI_OK;
      }
      
    } GUI_UNLOCK_H(pavi);
  }
  return AVI_STREAM_ERR;	
}

/**
  * @brief  Initialize AVI
  * @param  havi: AVI handle
  * @param  buf:
  * @param  size: AVI file size     
  * @retval AVI status
  */
AVISTATUS _AVI_Init(GUI_AVI_HANDLE havi, U8 *buf, U16 size)		 
{
  U16 offset;
  U8 *tbuf;
  AVISTATUS   res = AVI_OK;
  AVI_HEADER *aviheader;
  LIST_HEADER *listheader;
  AVIH_HEADER *avihheader; 
  STRH_HEADER *strhheader; 
  
  STRF_BMPHEADER *bmpheader; 
  STRF_WAVHEADER *wavheader; 
  
  AVI_CONTEXT * pavi;
  
  if (havi) {
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {  
      
      tbuf=buf;
      aviheader=(AVI_HEADER*)buf; 
      if(aviheader->RiffID != AVI_RIFF_ID)
      {
        GUI_UNLOCK_H(pavi);
        return AVI_RIFF_ERR;
      }
      
      if(aviheader->AviID != AVI_AVI_ID)
      {
        GUI_UNLOCK_H(pavi);
        return AVI_AVI_ERR;	
      }
      
      buf+=sizeof(AVI_HEADER);					  
      listheader=(LIST_HEADER*)(buf);
      
      if(listheader->ListID != AVI_LIST_ID)
      {
        GUI_UNLOCK_H(pavi);
        return AVI_LIST_ERR;
      }
      
      if(listheader->ListType != AVI_HDRL_ID)
      {
        GUI_UNLOCK_H(pavi);
        return AVI_HDRL_ERR;
      }
      
      buf+=sizeof(LIST_HEADER);					  
      avihheader=(AVIH_HEADER*)(buf);                                   
      if(avihheader->BlockID != AVI_AVIH_ID)
      {
        GUI_UNLOCK_H(pavi);
        return AVI_AVIH_ERR;
      }
      
      pavi->aviInfo.SecPerFrame=avihheader->SecPerFrame;			  
      pavi->aviInfo.TotalFrame=avihheader->TotalFrame;				  
      buf+=avihheader->BlockSize+8;					  
      listheader=(LIST_HEADER*)(buf); 
      
      if(listheader->ListID != AVI_LIST_ID)
      {
        GUI_UNLOCK_H(pavi);
        return AVI_LIST_ERR;
      }
      
      if(listheader->ListType != AVI_STRL_ID)
      {
        GUI_UNLOCK_H(pavi);
        return AVI_STRL_ERR;
      }
      
      strhheader=(STRH_HEADER*)(buf+12);                                
      if(strhheader->BlockID != AVI_STRH_ID)
      {
        GUI_UNLOCK_H(pavi);
        return AVI_STRH_ERR;
      }
      
      if(strhheader->StreamType == AVI_VIDS_STREAM)			  
      {
        if(strhheader->Handler != AVI_FORMAT_MJPG)
        {
          GUI_UNLOCK_H(pavi);
          return AVI_FORMAT_ERR;
        }
        
        pavi->aviInfo.VideoFLAG=(U8*)AVI_VIDS_FLAG_TBL[0];			
        pavi->aviInfo.AudioFLAG=(U8*)AVI_AUDS_FLAG_TBL[1];			
        bmpheader=(STRF_BMPHEADER*)(buf+12+strhheader->BlockSize+8);
        if(bmpheader->BlockID != AVI_STRF_ID)
        {
          GUI_UNLOCK_H(pavi);
          return AVI_STRF_ERR;
        }
        
        pavi->aviInfo.Width=bmpheader->bmiHeader.Width;
        pavi->aviInfo.Height=bmpheader->bmiHeader.Height; 
        buf+=listheader->BlockSize+8;					
        listheader=(LIST_HEADER*)(buf);
        if(listheader->ListID != AVI_LIST_ID)
        {
          pavi->aviInfo.SampleRate=0;					
          pavi->aviInfo.Channels=0;					
          pavi->aviInfo.AudioType=0;					
          
        }else
        {			
          if(listheader->ListType != AVI_STRL_ID)
          {
            GUI_UNLOCK_H(pavi);
            return AVI_STRL_ERR;	  
          }
          
          strhheader=(STRH_HEADER*)(buf+12);
          if(strhheader->BlockID != AVI_STRH_ID)
          {
            GUI_UNLOCK_H(pavi);
            return AVI_STRH_ERR;
          }
          
          if(strhheader->StreamType != AVI_AUDS_STREAM)
          {
            GUI_UNLOCK_H(pavi);
            return AVI_FORMAT_ERR;
          }
          
          wavheader=(STRF_WAVHEADER*)(buf+12+strhheader->BlockSize+8);
          if(wavheader->BlockID != AVI_STRF_ID)
          {
            GUI_UNLOCK_H(pavi);
            return AVI_STRF_ERR;
          }
          
          pavi->aviInfo.SampleRate=wavheader->SampleRate;			
          pavi->aviInfo.Channels=wavheader->Channels;	
          pavi->aviInfo.AudioType=wavheader->FormatTag;		
        }
      }else if(strhheader->StreamType == AVI_AUDS_STREAM)		 		
      { 
        pavi->aviInfo.VideoFLAG=(U8*)AVI_VIDS_FLAG_TBL[1];					
        pavi->aviInfo.AudioFLAG=(U8*)AVI_AUDS_FLAG_TBL[0];					
        wavheader=(STRF_WAVHEADER*)(buf+12+strhheader->BlockSize+8);
        if(wavheader->BlockID != AVI_STRF_ID)
        {
          GUI_UNLOCK_H(pavi);
          return AVI_STRF_ERR;
        }
        
        pavi->aviInfo.SampleRate=wavheader->SampleRate;				
        pavi->aviInfo.Channels=wavheader->Channels;				
        pavi->aviInfo.AudioType=wavheader->FormatTag;				
        buf+=listheader->BlockSize+8;					
        listheader=(LIST_HEADER*)(buf);
        if(listheader->ListID != AVI_LIST_ID)
        {
          GUI_UNLOCK_H(pavi);
          return AVI_LIST_ERR;	
        }
        
        if(listheader->ListType != AVI_STRL_ID)
        {
          GUI_UNLOCK_H(pavi);
          return AVI_STRL_ERR;	
        }
        
        strhheader=(STRH_HEADER*)(buf+12);
        if(strhheader->BlockID != AVI_STRH_ID)
        {
          GUI_UNLOCK_H(pavi);
          return AVI_STRH_ERR;
        }
        
        if(strhheader->StreamType != AVI_VIDS_STREAM)
        {
          GUI_UNLOCK_H(pavi);
          return AVI_FORMAT_ERR;
        }
        
        bmpheader=(STRF_BMPHEADER*)(buf+12+strhheader->BlockSize+8);
        if(bmpheader->BlockID != AVI_STRF_ID)
        {
          GUI_UNLOCK_H(pavi);
          return AVI_STRF_ERR;	
        }
        
        if(bmpheader->bmiHeader.Compression != AVI_FORMAT_MJPG)
        {
          GUI_UNLOCK_H(pavi);
          return AVI_FORMAT_ERR;
        }
        
        pavi->aviInfo.Width=bmpheader->bmiHeader.Width;
        pavi->aviInfo.Height=bmpheader->bmiHeader.Height; 	
      }
      offset=_AVI_SearchID(tbuf,size,(U8 *)"movi");					
      if(offset == 0)
      {
        GUI_UNLOCK_H(pavi);
        return AVI_MOVI_ERR;
      }
      
      if(pavi->aviInfo.SampleRate)
      {
        tbuf+=offset;
        offset=_AVI_SearchID(tbuf,size,pavi->aviInfo.AudioFLAG);			
        if(offset == 0)
        {
          GUI_UNLOCK_H(pavi);
          return AVI_STREAM_ERR;
        }
        
        tbuf+=offset+4;
        pavi->aviInfo.AudioBufSize=*((U16*)tbuf);						
      }
    }GUI_UNLOCK_H(pavi);
  }
  return res;
}

/**
  * @brief  Get Data
  * @param    
  * @retval 
  */
static int _GetData(void * p, const U8 ** ppData, unsigned NumBytes, U32 Off) {
  U8 * pSrc;

  pSrc = (U8 *)p;
  GUI_MEMCPY((U8 *)*ppData, pSrc + Off, NumBytes);
  return NumBytes;
}

/**
  * @brief  Stop AVI
  * @param  havi: AVI handle    
  * @retval 
  */
static int _Stopavi(GUI_AVI_HANDLE havi) {
  AVI_CONTEXT * pavi;
  int r;
  
  r = 1;
  pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {
    if (pavi->hTimer) {
      GUI_TIMER_Delete(pavi->hTimer);
      pavi->hTimer = 0;
      r = 0;
    }
    if (pavi->pfNotify) {
      pavi->pfNotify(havi, GUI_AVI_NOTIFICATION_STOP, pavi->CurrentImage);
    }
  } GUI_UNLOCK_H(pavi);
  return r;
}

/**
  * @brief  Decode AVI frame
  * @param  havi: AVI handle    
  * @retval 
  */
static int _DecodeFrame(GUI_AVI_HANDLE havi) {
  AVI_CONTEXT * pavi;
  int offset;
  const U8  *pData;

  
  pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {
    pavi->CurrentImage++;

    if (pavi->CurrentImage  ==  pavi->aviInfo.TotalFrame) {
      if (pavi->DoLoop) {
        pavi->CurrentImage = 0;
        
        if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->VideoBufferSize, 0)  !=  pavi->VideoBufferSize) 
        {
          return 1; 
        }
        _AVI_Init(havi, (U8 *)pavi->pVideoBuffer, pavi->VideoBufferSize); 
        offset = _AVI_SearchID((U8 *)pavi->pVideoBuffer,pavi->VideoBufferSize,(U8 *)"movi");
        _AVI_GetStreamInfo(havi, (U8 *)pavi->pVideoBuffer + offset + 4); 
        
        if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->aviInfo.StreamSize + 8, offset + 12)  !=  (pavi->aviInfo.StreamSize + 8)) 
        {
         return 1;          
        }
      } else {
        return 1; 
      }
    }
    else /* Not the end of the file */
    {   
      if(pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG)
      {
        if(pavi->aviInfo.StreamSize > pavi->VideoBufferSize )
        {
          return 1;  
        }        
        if(pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->aviInfo.StreamSize + 8, 0xFFFFFFFF)  !=  pavi->aviInfo.StreamSize + 8)
        {
          return 1;            
        }
        _AVI_GetStreamInfo(havi, (U8 *)pavi->pVideoBuffer + pavi->aviInfo.StreamSize);
      }
      
      if(pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG)
      {
        do{
          if(pavi->aviInfo.StreamSize >= pavi->AudioBufferSize)
          {
            return 1;  
          }

          if(pavi->AudioWdPtr > (pavi->AudioBufferSize -  pavi->aviInfo.StreamSize))
          {
            pavi->AudioWdPtr = 0;
          }
          
          pData = &pavi->pAudioBuffer[pavi->AudioWdPtr]; 
          if(pavi->pfGetData(pavi->pParam, &pData, pavi->aviInfo.StreamSize + 8, 0xFFFFFFFF)  !=  pavi->aviInfo.StreamSize + 8)
          {
            return 1; 
          }
     
          pavi->AudioWdPtr += pavi->aviInfo.StreamSize;
          
          _AVI_GetStreamInfo(havi,  (U8 *)&pData[pavi->aviInfo.StreamSize]);
          
        } while (pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG);
        

      }
    }
  } GUI_UNLOCK_H(pavi);
  return 0; 
}

/**
  * @brief  timer callback
  * @param  pTM: pointer to the timer   
  * @retval 
  */
static void _OnTimer(GUI_TIMER_MESSAGE * pTM) {
  AVI_CONTEXT * pavi;
  GUI_AVI_HANDLE havi;
  int r, Period;
  
  havi = pTM->Context;
  pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {

    if (pavi->pfNotify) {
      pavi->pfNotify(havi, GUI_AVI_NOTIFICATION_PREDRAW, pavi->CurrentImage);
    }
    
    /* Handle Video frame */
    WM_Deactivate(); 
    
    WM_SetWindowPos(WM_HBKWIN, 0, 0, LCD_GetXSize(), LCD_GetYSize() );    
    if (status == 0)
    {
      r = pavi->pfDraw(pavi->pVideoBuffer, pavi->VideoBufferSize,pavi->xPos , pavi->yPos);
    }
    else
    {
      r = pavi->pfDraw(pavi->pVideoBuffer, pavi->VideoBufferSize,pavi->xPos + LCD_GetXSize()/2 , pavi->yPos);
    }    
    
    WM_Activate();
    WM_SetWindowPos(WM_HBKWIN, -LCD_GetXSize()/2, 0, 2 * LCD_GetXSize() + 20, 2 * LCD_GetYSize() );

    if (pavi->pfNotify) {
      pavi->pfNotify(havi, GUI_AVI_NOTIFICATION_POSTDRAW, pavi->CurrentImage);
    }

    if (r)
    {    
      pavi->CurrentImage = 0;      
      pavi->pfNotify(havi, GUI_AVI_NOTIFICATION_EOF, pavi->aviInfo.TotalFrame);
      GUI_UNLOCK_H(pavi);            
      return; 
    }    
     /* Go to Next Frame */
    if (_DecodeFrame(havi)) 
    {
      pavi->CurrentImage = 0;      
      pavi->pfNotify(havi, GUI_AVI_NOTIFICATION_EOF, pavi->aviInfo.TotalFrame);
      GUI_UNLOCK_H(pavi);           
      return;
    }  

    /* Skip images if require more time */
    while ((U32)GUI_GetTime() > pavi->TimeNextFrame) {
      pavi->TimeNextFrame += pavi->aviInfo.SecPerFrame/ 1000;
      
      if (_DecodeFrame(havi)) 
      {
        pavi->CurrentImage = 0;      
        pavi->pfNotify(havi, GUI_AVI_NOTIFICATION_EOF, pavi->aviInfo.TotalFrame);        
        GUI_UNLOCK_H(pavi);
        return;
      } 
    }
    
    Period = pavi->TimeNextFrame - GUI_GetTime();
    if (Period < 0) {
      Period = 0;
    }
    
    GUI_TIMER_SetPeriod(pTM->hTimer, Period);
    GUI_TIMER_Restart(pTM->hTimer);
  } GUI_UNLOCK_H(pavi);
}


/**
  * @brief  Creates a movie handle
  * @param  pfGetData: pointer to GetData function
  * @param  pfNotify:  pointer to Notification callback function  
  * @retval AVI status
  */
GUI_AVI_HANDLE GUI_AVI_CreateEx(GUI_GET_DATA_FUNC * pfGetData, GUI_AVI_FUNC * pfNotify) {
  AVI_CONTEXT  * pavi;
  GUI_AVI_HANDLE havi;

  /* Allocate context for drawing operation */
  GUI_LOCK();
  havi = GUI_ALLOC_AllocZero(sizeof(AVI_CONTEXT));
  if (havi  ==  0) {
    GUI_UNLOCK();
    return 0;
  }
  
  pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {

    /*  Save parameters */
    pavi->pfNotify  = pfNotify;
    pavi->pfGetData = pfGetData;
    pavi->pfDraw = (U32(*)(const void * , U32 ,U32, U32))GUI_JPEG_Draw;
  } GUI_UNLOCK_H(pavi);
  GUI_UNLOCK();
  return havi;
}

/**
  * @brief  Starts movies showing
  * @param  havi: AVI handle
  * @param  pParam:  AVI file params 
  * @param  FileSize:  AVI file size   
  * @retval AVI status
  */
int GUI_AVI_Start(GUI_AVI_HANDLE havi, void * pParam, U32 FileSize) {
  AVI_CONTEXT  * pavi;
  U32 offset;
  int r = 1;
  const U8  *pData;
  
  if (havi) {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {

      /*  Save parameters */
      pavi->pParam    = pParam;
      pavi->FileSize  = FileSize;
      pavi->CurrentImage = 0;
      
      if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->VideoBufferSize, 0)  ==  pavi->VideoBufferSize) 
      {
        /* Parse AVI container */
        _AVI_Init(havi, (U8 *)pavi->pVideoBuffer, pavi->VideoBufferSize); 
        offset = _AVI_SearchID((U8 *)pavi->pVideoBuffer,pavi->VideoBufferSize,(U8 *)"movi");
        _AVI_GetStreamInfo(havi, (U8 *)pavi->pVideoBuffer + offset + 4); 
        
        /* Init Hardware */
        if(pavi->pfJpegInit)
        {
          pavi->pfJpegInit();
        }

        /* Read first stream */
        if(pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG)
        {
          if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->aviInfo.StreamSize + 8, offset + 12)  ==  (pavi->aviInfo.StreamSize + 8)) 
          {
            _AVI_GetStreamInfo(havi, (U8 *)pavi->pVideoBuffer + pavi->aviInfo.StreamSize);
          }
        }
       
        pavi->AudioWdPtr  = pavi->AudioBufferSize / 2;
                                                
        if(pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG)
        {
          
          do{
            if(pavi->AudioWdPtr >= pavi->AudioBufferSize - pavi->aviInfo.StreamSize - 8)
            {
              pavi->AudioWdPtr = 0;
            } 
            pData = &pavi->pAudioBuffer[pavi->AudioWdPtr];
            if(pavi->pfGetData(pavi->pParam, &pData, pavi->aviInfo.StreamSize + 8, 0xFFFFFFFF)  ==  pavi->aviInfo.StreamSize + 8)
            {
             pavi->AudioWdPtr += pavi->aviInfo.StreamSize;
              

            }
            _AVI_GetStreamInfo(havi, (U8 *)pData + pavi->aviInfo.StreamSize);
            
          } while (pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG);
        }   
      }

    } GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
  return r;
}

/**
  * @brief  Stop movies showing
  * @param  havi: AVI handle  
  * @retval AVI status
  */
int GUI_AVI_Stop(GUI_AVI_HANDLE havi) {
  AVI_CONTEXT  * pavi;
  int r = 1;
  
  if (havi) {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {

      if(pavi->pfJpegDeInit)
      {
        pavi->pfJpegDeInit();
      }
      _Stopavi(havi);
    } GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
  return r;
}

/**
  * @brief  Creates a movie handle
  * @param  pfNotify: pointer to notification callback function 
  * @retval AVI status
  */
GUI_AVI_HANDLE GUI_AVI_Create(GUI_AVI_FUNC * pfNotify) {

  GUI_AVI_HANDLE havi;

  havi = GUI_AVI_CreateEx(_GetData, pfNotify);
  return havi;
}

/**
  * @brief  Starts playing a movie from the current frame
  * @param  havi: AVI handle 
  * @retval AVI status
  */
int GUI_AVI_Play(GUI_AVI_HANDLE havi) {
  AVI_CONTEXT * pavi;
  int r;

  r = 1;
  if (havi) {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {

      /* Initialize timer */
      if (pavi->hTimer  ==  0) {
        pavi->hTimer = GUI_TIMER_Create(_OnTimer, 0, havi, 0);
        if (pavi->hTimer  ==  0) {
           GUI_UNLOCK_H(pavi);
           GUI_UNLOCK();
           GUI_ALLOC_Free(havi);
           return 0;
        }
        if (pavi->pfNotify) {
          pavi->pfNotify(havi, GUI_AVI_NOTIFICATION_START, pavi->CurrentImage);
        }
        pavi->TimeNextFrame = GUI_GetTime();
        r = 0;
      }
    } GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
  return r;
}

/**
  * @brief  Starts playing a movie starting at the first frame.
  * @param  havi: AVI handle 
  * @param  xPos: X-position in screen coordinates to be used
  * @param  yPos: Y-position in screen coordinates to be used
  * @param  DoLoop: - 1 for endless loop, 0 for single loop  
  * @retval AVI status
  */
int GUI_AVI_Show(GUI_AVI_HANDLE havi, int xPos, int yPos, int DoLoop) {
  AVI_CONTEXT * pavi;
  int r;

  r = 1;
  if (havi) {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {

      /* Save parameters */
      pavi->xPos   = xPos;
      pavi->yPos   = yPos;           
      pavi->DoLoop = DoLoop ? 1 : 0;

      /* Start playing */
      r = GUI_AVI_Play(havi);
    } GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
  return r;
}

/**
  * @brief  Selects AVI decoding mode.
  * @param  havi: AVI handle 
  * @param  Mode: decoding mode 
  * @retval AVI status
  */
int GUI_AVI_SelectMode (GUI_AVI_HANDLE havi, int Mode) {
  AVI_CONTEXT * pavi;
  int r = 1;
  if (havi) {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {
      
      if(Mode == 0)
      {
        pavi->pfDraw = (U32(*)(const void * , U32 ,U32, U32))GUI_JPEG_Draw;
      }
      else
      {
        pavi->pfDraw = pavi->pfJpegDraw;        
      }
       r = 0;
    } GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
  return r;
}

/**
  * @brief  register HW JPEG/Audio decoder functions.
  * @param  havi: AVI handle 
  * @retval AVI status
  */
void GUI_AVI_SetDevFunc (GUI_AVI_HANDLE havi, int IdFunc, void (* pDriverFunc)(void))
{
  AVI_CONTEXT * pavi;

  if (havi) {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {
      
      switch (IdFunc)
      {
      case GUI_AVI_JPEG_INIT:
        pavi->pfJpegInit = pDriverFunc;
        break;

      case GUI_AVI_JPEG_DEINIT:
        pavi->pfJpegDeInit = pDriverFunc;
        break;

      case GUI_AVI_JPEG_DRAW:
        pavi->pfJpegDraw = (U32(*)(const void * , U32 ,U32, U32))pDriverFunc;
        break;        

      default:
          break;       
      }
     
    } GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
}

/**
  * @brief  Add Video/Audio decoder Buffer.
  * @param  havi: AVI handle 
  * @retval AVI status
  */
void GUI_AVI_SetBuffers (GUI_AVI_HANDLE havi, U8 *pVidBuff, U16 VidBuffSize, U8 *pAudBuff, U16 AudBuffSize)
{
  AVI_CONTEXT * pavi;

  if (havi) {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {
      pavi->pVideoBuffer = pVidBuff;
      pavi->VideoBufferSize = VidBuffSize;      
      pavi->pAudioBuffer = pAudBuff;
      pavi->AudioBufferSize = AudBuffSize;  
      
    } GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
}

/**
  * @brief  Removes a movie data structure and associated elements like timers
  *         from memory.
  * @param  havi: AVI handle 
  * @retval AVI status
  */
int GUI_AVI_Delete(GUI_AVI_HANDLE havi) {
  AVI_CONTEXT  * pavi;
  GUI_AVI_FUNC * pfNotify;
  int r;

  r = 1;
  if (havi) {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {
      pfNotify = pavi->pfNotify;
      if (pavi->hTimer) {
        GUI_TIMER_Delete(pavi->hTimer);
        pavi->hTimer = 0;
      }
      if(pavi->pfJpegDeInit)
      {
        pavi->pfJpegDeInit();
      }

      if (pfNotify) {
        pfNotify(havi, GUI_AVI_NOTIFICATION_DELETE, 0);
      }   
      r = 0;
    } GUI_UNLOCK_H(pavi);
    GUI_ALLOC_Free(havi);
    GUI_UNLOCK();
  }
  return r;
}

/**
  * @brief  Stops playing. Can be restarted with GUI_AVI_Play()
  * @param  havi: AVI handle 
  * @retval AVI status
  */
int GUI_AVI_Pause(GUI_AVI_HANDLE havi) {
  int r;

  GUI_LOCK();
  r = 1;
  if (havi) {
    r = _Stopavi(havi);
  }
  GUI_UNLOCK();
  
  return r;
}

/**
  * @brief  Jumps to the given frame index
  * @param  havi: AVI handle 
  * @param  Frame: frame index  
  * @retval AVI status
  */
int GUI_AVI_GotoFrame(GUI_AVI_HANDLE havi, U32 Frame) {
  AVI_CONTEXT * pavi;
  int r, pos;
  U16 offset;
  const U8  *pData;
    
  r = 0;
  if (havi) {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {
      
      pos = Frame * (pavi->FileSize/ pavi->aviInfo.TotalFrame);
      
      pavi->CurrentImage = Frame;
            
      if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->VideoBufferSize,  pos)  !=  pavi->VideoBufferSize) 
      {
        GUI_UNLOCK_H(pavi);
        GUI_UNLOCK();
        GUI_ALLOC_Free(havi);
        return 0;
      } 
      
      if(pos  ==  0) 
      {
        offset = _AVI_SearchID((U8 *)pavi->pVideoBuffer, pavi->VideoBufferSize, (U8 *)"movi"); 
      }
      else 
      {
        offset = 0;
      }
      
      offset += _AVI_SearchID((U8 *)pavi->pVideoBuffer + offset,pavi->VideoBufferSize, pavi->aviInfo.VideoFLAG);
      
      _AVI_GetStreamInfo(havi,(U8 *)pavi->pVideoBuffer + offset);	
      
      if(pavi->aviInfo.StreamID  ==  AVI_VIDS_FLAG)
      {
        if (pavi->pfGetData(pavi->pParam, &pavi->pVideoBuffer, pavi->aviInfo.StreamSize + 8,  pos +offset + 8)  ==  (pavi->aviInfo.StreamSize + 8)) 
        {
          _AVI_GetStreamInfo(havi, (U8 *)pavi->pVideoBuffer + pavi->aviInfo.StreamSize);
        }
      }
      
      
      if(pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG)
      {
        do{
          
          if(pavi->AudioWdPtr >= (pavi->AudioBufferSize -  pavi->aviInfo.StreamSize - 8))
          {
            pavi->AudioWdPtr = 0;
          }
          
          pData = &pavi->pAudioBuffer[pavi->AudioWdPtr]; 
          if(pavi->pfGetData(pavi->pParam, &pData, pavi->aviInfo.StreamSize + 8, 0xFFFFFFFF)  ==  pavi->aviInfo.StreamSize + 8)
          {
            pavi->AudioWdPtr += pavi->aviInfo.StreamSize;       
            _AVI_GetStreamInfo(havi, (U8 *)&pData[pavi->aviInfo.StreamSize]);
          }
        } while (pavi->aviInfo.StreamID  ==  AVI_AUDS_FLAG);
        
      }  
      
      if (pavi->pfNotify) {
        pavi->pfNotify(havi, GUI_AVI_NOTIFICATION_PREDRAW, pavi->CurrentImage);
      }
      
      /* Handle Video frame */
      WM_Deactivate();      
      r = pavi->pfDraw(pavi->pVideoBuffer, pavi->VideoBufferSize, pavi->xPos, pavi->yPos);
      WM_Activate();
      
      if (pavi->pfNotify) {
        pavi->pfNotify(havi, GUI_AVI_NOTIFICATION_POSTDRAW, pavi->CurrentImage);
      }
    
      
    } GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
  return r;
}

/**
  * @brief  Get frame index
  * @param  havi: AVI handle  
  * @retval Returns the current frame index.
  */
U32 GUI_AVI_GetFrameIndex(GUI_AVI_HANDLE havi) {
  AVI_CONTEXT * pavi;
  U32 r;

  r = 0xFFFFFFFF;
  if (havi) {
    GUI_LOCK();
    pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {
      r = pavi->CurrentImage;
    } GUI_UNLOCK_H(pavi);
    GUI_UNLOCK();
  }
  return r;
}

/**
  * @brief  Fills a GUI_AVI_INFO structure of a movie file not located in
  *         addressable memory area.
  * @param  havi: AVI handle  
  * @param  pInfo: pointer to the file information  
  * @retval Returns the current frame index.
  */
int GUI_AVI_GetInfo(GUI_AVI_HANDLE havi, GUI_AVI_INFO * pInfo) {
  
  AVI_CONTEXT * pavi;
  
  pavi = (AVI_CONTEXT *)GUI_LOCK_H(havi); {
    pInfo->msPerFrame = pavi->aviInfo.SecPerFrame/ 1000;
    pInfo->NumFrames  = pavi->aviInfo.TotalFrame;
    pInfo->xSize      = pavi->aviInfo.Width;
    pInfo->ySize      = pavi->aviInfo.Height;
  }
  GUI_UNLOCK_H(pavi);
  return 0;
}
