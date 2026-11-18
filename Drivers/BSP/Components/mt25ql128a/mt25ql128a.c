/**
 ******************************************************************************
 * @file    MT25QL128A.c
 * @modify  MCD Application Team
 * @brief   This file provides the MT25QL128A QSPI drivers.
 ******************************************************************************
 * MT25QL128A action :
 *   Quad IO protocol (QP) and Dual IO protocol (DP) bits of Enhanced Volatile
 *   Configuration Register (EVCR):
 *     QP = 0: Operates in Quad IO protocol (accepts 4-4-4 commands)
 *     QP = 1 and DP = 0: Operates in Dual IO protocol (accepts 2-2-2 commands)
 *     QP = 1 and DP = 1: Operates in Single IO protocol (accepts 1-1-1, 1-1-2,
 *                                               1-2-2, 1-1-4, 1-4-4 commands)
 *   Enter QPI mode by issuing ENABLE_QSPI_CMD(0x35) command from 1-1-1 mode
 *   Exit QPI mode by issuing RESET_QSPI_CMD(0xF5) command from 4-4-4 mode
 *   Enter DPI mode by configuring QP = 1 and DP = 0 in EVCR
 *   Exit DPI mode by configuring QP = 1 and DP = 1 in EVCR
 *
 *   Memory Read commands support STR(Single Transfer Rate) &
 *   DTR(Double Transfer Rate) modes
 *
 *   Dual flash mode is supported: register read and write is double size
 *
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "MT25QL128A.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup MT25QL128A MT25QL128A
  * @{
  */

/** @defgroup MT25QL128A_Exported_Functions MT25QL128A Exported Functions
  * @{
  */

/**
  * @brief  Get Flash information
  * @param  pInfo pointer to information structure
  * @retval error status
  */
int32_t MT25QL128A_GetFlashInfo(MT25QL128A_Info_t *pInfo)
{
  /* Configure the structure with the memory configuration */
  pInfo->FlashSize              = MT25QL128A_FLASH_SIZE;
  pInfo->EraseSectorSize        = MT25QL128A_SECTOR_64K;
  pInfo->EraseSectorsNumber     = (MT25QL128A_FLASH_SIZE/MT25QL128A_SECTOR_64K);
  pInfo->EraseSubSectorSize     = MT25QL128A_SUBSECTOR_32K;
  pInfo->EraseSubSectorNumber   = (MT25QL128A_FLASH_SIZE/MT25QL128A_SUBSECTOR_32K);
  pInfo->EraseSubSector1Size    = MT25QL128A_SUBSECTOR_4K;
  pInfo->EraseSubSector1Number  = (MT25QL128A_FLASH_SIZE/MT25QL128A_SUBSECTOR_4K);
  pInfo->ProgPageSize           = MT25QL128A_PAGE_SIZE;
  pInfo->ProgPagesNumber        = (MT25QL128A_FLASH_SIZE/MT25QL128A_PAGE_SIZE);

  return MT25QL128A_OK;
};

/**
  * @brief  Polling WIP(Write In Progress) bit become to 0
  *         SPI/DPI/QPI; 1-0-1/2-0-2/4-0-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  DualFlash Dual flash mode state
  * @retval error status
  */
int32_t MT25QL128A_AutoPollingMemReady(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode, MT25QL128A_DualFlash_t DualFlash)
{
  QSPI_CommandTypeDef      s_command;
  QSPI_AutoPollingTypeDef  s_config;

  /* Configure automatic polling mode to wait for memory ready */
  s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = MT25QL128A_READ_STATUS_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  /* At least one dummy cycle is required when using dual or quad mode to receive data */
  s_command.DummyCycles       = (Mode == MT25QL128A_QPI_MODE) ? 2U : (Mode == MT25QL128A_DPI_MODE) ? 4U : 0U;
  s_command.DataMode          = (Mode == MT25QL128A_QPI_MODE) ? QSPI_DATA_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_DATA_2_LINES : QSPI_DATA_1_LINE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  s_config.Match           = 0;
  s_config.Mask            = (DualFlash == MT25QL128A_DUALFLASH_ENABLE) ? ((MT25QL128A_SR_WIP << 8) | MT25QL128A_SR_WIP) : MT25QL128A_SR_WIP;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = (DualFlash == MT25QL128A_DUALFLASH_ENABLE) ? 2U : 1U;
  s_config.Interval        = MT25QL128A_AUTOPOLLING_INTERVAL_TIME;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_QSPI_AutoPolling(Ctx, &s_command, &s_config, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/* Read/Write Array Commands (3/4 Byte Address Command Set) *********************/
/**
  * @brief  Reads an amount of data from the QSPI memory on STR mode.
  *         SPI/DPI/QPI; 1-1-1/1-1-2/1-2-2/1-1-4/1-4-4/2-2-2/4-4-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start address
  * @param  Size Size of data to read
  * @retval QSPI memory status
  */
int32_t MT25QL128A_ReadSTR(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode, MT25QL128A_AddressSize_t AddressSize, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the read command */
  switch(Mode)
  {
  case MT25QL128A_SPI_1I2O_MODE :          /* 1-1-2 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_1I2O_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_1I2O_FAST_READ_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DummyCycles     = 8;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_SPI_2IO_MODE :           /* 1-2-2 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_2IO_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_2IO_FAST_READ_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_2_LINES;
    s_command.DummyCycles     = 8;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_SPI_1I4O_MODE :          /* 1-1-4 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_1I4O_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_1I4O_FAST_READ_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DummyCycles     = 8;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_SPI_4IO_MODE :           /* 1-4-4 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_4IO_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_4IO_FAST_READ_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_4_LINES;
    s_command.DummyCycles     = 10;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_DPI_MODE :               /* 2-2-2 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_2_LINES;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_2IO_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_2IO_FAST_READ_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_2_LINES;
    s_command.DummyCycles     = 8;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_QPI_MODE :               /* 4-4-4 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_4IO_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_4IO_FAST_READ_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_4_LINES;
    s_command.DummyCycles     = 10;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_SPI_MODE :               /* 1-1-1 commands, Power on H/W default setting */
  default :
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_FAST_READ_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DummyCycles     = 8;
    s_command.DataMode        = QSPI_DATA_1_LINE;
    break;
  }

  s_command.AddressSize       = (AddressSize == MT25QL128A_3BYTES_SIZE) ? QSPI_ADDRESS_24_BITS : QSPI_ADDRESS_32_BITS;
  s_command.Address           = ReadAddr;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.NbData            = Size;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(Ctx, pData, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @brief  Reads an amount of data from the QSPI memory on DTR mode.
  *         SPI/DPI/QPI; 1-1-1/1-1-2/1-2-2/1-1-4/1-4-4/2-2-2/4-4-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start addressS
  * @param  Size Size of data to read
  * @note   Address size is forced to 3 Bytes when the 4 Bytes address size
  *         command is not available for the specified interface mode
  * @retval QSPI memory status
  */
int32_t MT25QL128A_ReadDTR(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode, MT25QL128A_AddressSize_t AddressSize, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the read command */
  switch(Mode)
  {
  case MT25QL128A_SPI_1I2O_MODE :          /* 1-1-2 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = MT25QL128A_1I2O_FAST_READ_DTR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DummyCycles     = 6;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_SPI_2IO_MODE :           /* 1-2-2 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_2IO_FAST_READ_DTR_CMD : MT25QL128A_4_BYTE_ADDR_2IO_FAST_READ_DTR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_2_LINES;
    s_command.DummyCycles     = 6;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_SPI_1I4O_MODE :          /* 1-1-4 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = MT25QL128A_1I4O_FAST_READ_DTR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DummyCycles     = 6;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_SPI_4IO_MODE :           /* 1-4-4 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_4IO_FAST_READ_DTR_CMD : MT25QL128A_4_BYTE_ADDR_4IO_FAST_READ_DTR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_4_LINES;
    s_command.DummyCycles     = 8;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_DPI_MODE :               /* 2-2-2 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_2_LINES;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_2IO_FAST_READ_DTR_CMD : MT25QL128A_4_BYTE_ADDR_2IO_FAST_READ_DTR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_2_LINES;
    s_command.DummyCycles     = 6;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_QPI_MODE :               /* 4-4-4 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_4IO_FAST_READ_DTR_CMD : MT25QL128A_4_BYTE_ADDR_4IO_FAST_READ_DTR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_4_LINES;
    s_command.DummyCycles     = 8;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_SPI_MODE :               /* 1-1-1 commands, Power on H/W default setting */
  default :
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_FAST_READ_DTR_CMD : MT25QL128A_4_BYTE_ADDR_FAST_READ_DTR_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DummyCycles     = 6;
    s_command.DataMode        = QSPI_DATA_1_LINE;
    break;
  }

  s_command.AddressSize       = (AddressSize == MT25QL128A_3BYTES_SIZE) ? QSPI_ADDRESS_24_BITS : QSPI_ADDRESS_32_BITS;
  s_command.Address           = ReadAddr;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.NbData            = Size;
  s_command.DdrMode           = QSPI_DDR_MODE_ENABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_HALF_CLK_DELAY;    /* DTR mode used */
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(Ctx, pData, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @brief  Writes an amount of data to the QSPI memory.
  *         SPI/DPI/QPI; 1-1-1/1-1-2/1-2-2/1-1-4/1-4-4/2-2-2/4-4-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @param  pData Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size Size of data to write. Range 1 ~ MT25QL128A_PAGE_SIZE
  * @note   Address size is forced to 3 Bytes when the 4 Bytes address size
  *         command is not available for the specified interface mode
  * @retval QSPI memory status
  */
int32_t MT25QL128A_PageProgram(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode, MT25QL128A_AddressSize_t AddressSize, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the program command */
  switch(Mode)
  {
  case MT25QL128A_SPI_1I2O_MODE :              /* 1-1-2 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = MT25QL128A_DUAL_INPUT_FAST_PROG_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_SPI_2IO_MODE :               /* 1-2-2 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = MT25QL128A_EXTENDED_DUAL_INPUT_FAST_PROG_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_2_LINES;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_SPI_1I4O_MODE :              /* 1-1-4 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_QUAD_INPUT_FAST_PROG_CMD : MT25QL128A_4_BYTE_ADDR_QUAD_INPUT_FAST_PROG_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_SPI_4IO_MODE :               /* 1-4-4 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_EXTENDED_QUAD_INPUT_FAST_PROG_CMD : MT25QL128A_4_BYTE_ADDR_QUAD_INPUT_EXTENDED_FAST_PROG_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_4_LINES;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_DPI_MODE :                   /* 2-2-2 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_2_LINES;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_EXTENDED_DUAL_INPUT_FAST_PROG_CMD : MT25QL128A_4_BYTE_ADDR_PAGE_PROG_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_2_LINES;
    s_command.DataMode        = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_QPI_MODE :                   /* 4-4-4 commands */
    s_command.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_EXTENDED_QUAD_INPUT_FAST_PROG_CMD : MT25QL128A_4_BYTE_ADDR_QUAD_INPUT_EXTENDED_FAST_PROG_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_4_LINES;
    s_command.DataMode        = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_SPI_MODE :                   /* 1-1-1 commands, Power on H/W default setting */
  default :
    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_PAGE_PROG_CMD : MT25QL128A_4_BYTE_ADDR_PAGE_PROG_CMD;
    s_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    s_command.DataMode        = QSPI_DATA_1_LINE;
    break;
  }

  s_command.AddressSize       = (AddressSize == MT25QL128A_3BYTES_SIZE) ? QSPI_ADDRESS_24_BITS : QSPI_ADDRESS_32_BITS;
  s_command.Address           = WriteAddr;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.NbData            = Size;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  /* Transmission of the data */
  if (HAL_QSPI_Transmit(Ctx, pData, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @brief  Erases the specified block of the QSPI memory.
  *         MT25QL128A support 4K, 32K, 64K size block erase commands.
  *         SPI/DPI/QPI; 1-1-0/2-2-0/4-4-0
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @param  BlockAddress Block address to erase
  * @param  BlockSize Block size to erase
  * @retval QSPI memory status
  */
int32_t MT25QL128A_BlockErase(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode, MT25QL128A_AddressSize_t AddressSize, uint32_t BlockAddress, MT25QL128A_Erase_t BlockSize)
{
  int32_t ret = MT25QL128A_OK;
  QSPI_CommandTypeDef s_command;

  /* Initialize the erase command */
  switch(BlockSize)
  {
  case MT25QL128A_ERASE_32K :
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_SUBSECTOR_ERASE_32K_CMD : MT25QL128A_4_BYTE_ADDR_SUBSECTOR_ERASE_32K_CMD;
    break;

  case MT25QL128A_ERASE_64K :
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_SECTOR_ERASE_64K_CMD : MT25QL128A_4_BYTE_ADDR_SECTOR_ERASE_64K_CMD;
    break;

  case MT25QL128A_ERASE_4K :
  default :
    s_command.Instruction     = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_SUBSECTOR_ERASE_4K_CMD : MT25QL128A_4_BYTE_ADDR_SUBSECTOR_ERASE_4K_CMD;
    break;
  }

  s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
  s_command.AddressMode       = (Mode == MT25QL128A_QPI_MODE) ? QSPI_ADDRESS_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_ADDRESS_2_LINES : QSPI_ADDRESS_1_LINE;
  s_command.AddressSize       = (AddressSize == MT25QL128A_3BYTES_SIZE) ? QSPI_ADDRESS_24_BITS : QSPI_ADDRESS_32_BITS;
  s_command.Address           = BlockAddress;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if(HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    ret = MT25QL128A_ERROR;
  }

  return ret;
}

/**
  * @brief  Whole chip erase.
  *         SPI/DPI/QPI; 1-0-0/2-0-0/4-0-0
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @retval error status
  */
int32_t MT25QL128A_ChipErase(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the erase command */
  s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = MT25QL128A_BULK_ERASE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @brief  Enable memory mapped mode for the QSPI memory on STR mode.
  *         SPI/DPI/QPI; 1-1-1/1-1-2/1-2-2/1-1-4/1-4-4/2-2-2/4-4-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @retval QSPI memory status
  */
int32_t MT25QL128A_EnableMemoryMappedModeSTR(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode, MT25QL128A_AddressSize_t AddressSize)
{
  QSPI_CommandTypeDef      s_command;
  QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

  switch(Mode)
  {
  case MT25QL128A_SPI_1I2O_MODE :          /* 1-1-2 commands */
    s_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction        = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_1I2O_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_1I2O_FAST_READ_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_1_LINE;
    s_command.DummyCycles        = 8;
    s_command.DataMode           = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_SPI_2IO_MODE :           /* 1-2-2 commands */
    s_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction        = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_2IO_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_2IO_FAST_READ_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_2_LINES;
    s_command.DummyCycles        = 8;
    s_command.DataMode           = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_SPI_1I4O_MODE :          /* 1-1-4 commands */
    s_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction        = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_1I4O_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_1I4O_FAST_READ_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_1_LINE;
    s_command.DummyCycles        = 8;
    s_command.DataMode           = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_SPI_4IO_MODE :           /* 1-4-4 commands */
    s_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction        = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_4IO_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_4IO_FAST_READ_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_4_LINES;
    s_command.DummyCycles        = 10;
    s_command.DataMode           = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_DPI_MODE :               /* 2-2-2 commands */
    s_command.InstructionMode    = QSPI_INSTRUCTION_2_LINES;
    s_command.Instruction        = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_2IO_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_2IO_FAST_READ_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_2_LINES;
    s_command.DummyCycles        = 8;
    s_command.DataMode           = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_QPI_MODE :               /* 4-4-4 commands */
    s_command.InstructionMode    = QSPI_INSTRUCTION_4_LINES;
    s_command.Instruction        = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_4IO_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_4IO_FAST_READ_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_4_LINES;
    s_command.DummyCycles        = 10;
    s_command.DataMode           = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_SPI_MODE :               /* 1-1-1 commands, Power on H/W default setting */
  default :
    s_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction        = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_FAST_READ_CMD : MT25QL128A_4_BYTE_ADDR_FAST_READ_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_1_LINE;
    s_command.DummyCycles        = 8;
    s_command.DataMode           = QSPI_DATA_1_LINE;
    break;
  }

  /* Configure the command for the read instruction */
  s_command.AddressSize       = (AddressSize == MT25QL128A_3BYTES_SIZE) ? QSPI_ADDRESS_24_BITS : QSPI_ADDRESS_32_BITS;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
  s_mem_mapped_cfg.TimeOutPeriod     = 0;

  if (HAL_QSPI_MemoryMapped(Ctx, &s_command, &s_mem_mapped_cfg) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @brief  Enable memory mapped mode for the QSPI memory on DTR mode.
  *         SPI/DPI/QPI; 1-1-1/1-1-2/1-2-2/1-1-4/1-4-4/2-2-2/4-4-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @note   Address size is forced to 3 Bytes when the 4 Bytes address size
  *         command is not available for the specified interface mode
  * @retval QSPI memory status
  */
int32_t MT25QL128A_EnableMemoryMappedModeDTR(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode, MT25QL128A_AddressSize_t AddressSize)
{
  QSPI_CommandTypeDef      s_command;
  QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

  switch(Mode)
  {
  case MT25QL128A_SPI_1I2O_MODE :          /* 1-1-2 read commands */
    s_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction        = MT25QL128A_1I2O_FAST_READ_DTR_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_1_LINE;
    s_command.DummyCycles        = 6;
    s_command.DataMode           = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_SPI_2IO_MODE :           /* 1-2-2 read commands */
    s_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction        = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_2IO_FAST_READ_DTR_CMD : MT25QL128A_4_BYTE_ADDR_2IO_FAST_READ_DTR_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_2_LINES;
    s_command.DummyCycles        = 6;
    s_command.DataMode           = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_SPI_1I4O_MODE :          /* 1-1-4 read commands */
    s_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction        = MT25QL128A_1I4O_FAST_READ_DTR_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_1_LINE;
    s_command.DummyCycles        = 6;
    s_command.DataMode           = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_SPI_4IO_MODE :           /* 1-4-4 read commands */
    s_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction        = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_4IO_FAST_READ_DTR_CMD : MT25QL128A_4_BYTE_ADDR_4IO_FAST_READ_DTR_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_4_LINES;
    s_command.DummyCycles        = 8;
    s_command.DataMode           = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_DPI_MODE :               /* 2-2-2 commands */
    s_command.InstructionMode    = QSPI_INSTRUCTION_2_LINES;
    s_command.Instruction        = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_2IO_FAST_READ_DTR_CMD : MT25QL128A_4_BYTE_ADDR_2IO_FAST_READ_DTR_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_2_LINES;
    s_command.DummyCycles        = 6;
    s_command.DataMode           = QSPI_DATA_2_LINES;
    break;

  case MT25QL128A_QPI_MODE :               /* 4-4-4 commands */
    s_command.InstructionMode    = QSPI_INSTRUCTION_4_LINES;
    s_command.Instruction        = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_4IO_FAST_READ_DTR_CMD : MT25QL128A_4_BYTE_ADDR_4IO_FAST_READ_DTR_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_4_LINES;
    s_command.DummyCycles        = 8;
    s_command.DataMode           = QSPI_DATA_4_LINES;
    break;

  case MT25QL128A_SPI_MODE :               /* 1-1-1 commands, Power on H/W default setting */
  default :
    s_command.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction        = (AddressSize == MT25QL128A_3BYTES_SIZE) ? MT25QL128A_FAST_READ_DTR_CMD : MT25QL128A_4_BYTE_ADDR_FAST_READ_DTR_CMD;
    s_command.AddressMode        = QSPI_ADDRESS_1_LINE;
    s_command.DummyCycles        = 6;
    s_command.DataMode           = QSPI_DATA_1_LINE;
    break;
  }

  /* Configure the command for the read instruction */
  s_command.AddressSize       = (AddressSize == MT25QL128A_3BYTES_SIZE) ? QSPI_ADDRESS_24_BITS : QSPI_ADDRESS_32_BITS;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_ENABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_HALF_CLK_DELAY;    /* DTR mode used */
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
  s_mem_mapped_cfg.TimeOutPeriod     = 0;

  if (HAL_QSPI_MemoryMapped(Ctx, &s_command, &s_mem_mapped_cfg) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}


/* Register/Setting Commands **************************************************/
/**
  * @brief  This function send a Write Enable and wait it is effective.
  *         SPI/DPI/QPI; 1-0-0/2-0-0/4-0-0
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  DualFlash Dual flash mode state
  * @retval error status
  */
int32_t MT25QL128A_WriteEnable(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode, MT25QL128A_DualFlash_t DualFlash)
{
  QSPI_CommandTypeDef     s_command;
  QSPI_AutoPollingTypeDef s_config;

  /* Initialize the write enable command */
  s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = MT25QL128A_WRITE_ENABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  /* Configure automatic polling mode to wait for write enabling */
  s_config.Match           = (DualFlash == MT25QL128A_DUALFLASH_ENABLE) ? ((MT25QL128A_SR_WEN << 8) | MT25QL128A_SR_WEN) : MT25QL128A_SR_WEN;
  s_config.Mask            = (DualFlash == MT25QL128A_DUALFLASH_ENABLE) ? ((MT25QL128A_SR_WEN << 8) | MT25QL128A_SR_WEN) : MT25QL128A_SR_WEN;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = (DualFlash == MT25QL128A_DUALFLASH_ENABLE) ? 2U : 1U;
  s_config.Interval        = MT25QL128A_AUTOPOLLING_INTERVAL_TIME;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  s_command.Instruction    = MT25QL128A_READ_STATUS_REG_CMD;
  s_command.DataMode       = (Mode == MT25QL128A_QPI_MODE) ? QSPI_DATA_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_DATA_2_LINES : QSPI_DATA_1_LINE;

  if (HAL_QSPI_AutoPolling(Ctx, &s_command, &s_config, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @brief  This function reset the (WEN) Write Enable Latch bit.
  *         SPI/DPI/QPI; 1-0-0/2-0-0/4-0-0
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @retval error status
  */
int32_t MT25QL128A_WriteDisable(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode)
{
  QSPI_CommandTypeDef     s_command;

  /* Initialize the read ID command */
  s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = MT25QL128A_WRITE_DISABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @brief  Read Flash Status register value
  *         SPI/DPI/QPI; 1-0-1/2-0-2/4-0-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  DualFlash Dual flash mode state
  * @param  Value Status register value pointer
  * @retval error status
  */
int32_t MT25QL128A_ReadStatusRegister(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode, MT25QL128A_DualFlash_t DualFlash, uint8_t *Value)
{
    QSPI_CommandTypeDef s_command;

    /* Initialize the reading of status register */
    s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = MT25QL128A_READ_STATUS_REG_CMD;
    s_command.AddressMode       = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DummyCycles       = 0;
    s_command.DataMode          = (Mode == MT25QL128A_QPI_MODE) ? QSPI_DATA_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_DATA_2_LINES : QSPI_DATA_1_LINE;
    s_command.NbData            = (DualFlash == MT25QL128A_DUALFLASH_ENABLE) ? 2U : 1U;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    /* Configure the command */
    if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return MT25QL128A_ERROR;
    }

    /* Reception of the data */
    if (HAL_QSPI_Receive(Ctx, Value, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return MT25QL128A_ERROR;
    }

    return MT25QL128A_OK;
}

/**
  * @brief  Read Flash enhanced volatile configuration register value
  *         SPI/DPI/QPI; 1-0-1/2-0-2/4-0-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  DualFlash Dual flash mode state
  * @param  Value Status register value pointer
  * @retval error status
  */
int32_t MT25QL128A_ReadEnhancedVolCfgRegister(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode, MT25QL128A_DualFlash_t DualFlash, uint8_t *Value)
{
    QSPI_CommandTypeDef s_command;

    /* Initialize the reading of configuration register */
    s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = MT25QL128A_READ_ENHANCED_VOL_CFG_REG_CMD;
    s_command.AddressMode       = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DummyCycles       = 0;
    s_command.DataMode          = (Mode == MT25QL128A_QPI_MODE) ? QSPI_DATA_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_DATA_2_LINES : QSPI_DATA_1_LINE;
    s_command.NbData            = (DualFlash == MT25QL128A_DUALFLASH_ENABLE) ? 2U : 1U;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    /* Configure the command */
    if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return MT25QL128A_ERROR;
    }

    /* Reception of the data */
    if (HAL_QSPI_Receive(Ctx, Value, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return MT25QL128A_ERROR;
    }

    return MT25QL128A_OK;
}

/**
  * @brief  Write enhanced volatile configuration register.
  *         SPI/DPI/QPI; 1-0-1/2-0-2/4-0-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  DualFlash Dual flash mode state
  * @param  Value Value for write to enhanced configuration register.
  * @retval QSPI memory status
  */
int32_t MT25QL128A_WriteEnhancedVolCfgRegister(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode, MT25QL128A_DualFlash_t DualFlash, uint8_t *Value)
{
  QSPI_CommandTypeDef s_command;

  /* Update the configuration register with new value */
  s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = MT25QL128A_WRITE_ENHANCED_VOL_CFG_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = (Mode == MT25QL128A_QPI_MODE) ? QSPI_DATA_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_DATA_2_LINES : QSPI_DATA_1_LINE;
  s_command.NbData            = (DualFlash == MT25QL128A_DUALFLASH_ENABLE) ? 2U : 1U;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the write volatile configuration register command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  /* Transmission of the data */
  if (HAL_QSPI_Transmit(Ctx, Value, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @brief  This function put QSPI memory in QPI mode (Quad I/O) from SPI mode.
  *         SPI -> QPI; 1-x-x -> 4-4-4
  *         SPI; 1-0-0
  * @param  Ctx Component object pointer
  * @retval error status
  */
int32_t MT25QL128A_EnterQPIMode(QSPI_HandleTypeDef *Ctx)
{
  QSPI_CommandTypeDef      s_command;

  /* Initialize the QPI enable command */
  /* QSPI memory is supposed to be in SPI mode, so CMD on 1 LINE */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = MT25QL128A_ENABLE_QSPI_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @brief  This function put QSPI memory in SPI mode (Single I/O) from QPI mode.
  *         QPI -> SPI; 4-4-4 -> 1-x-x
  *         QPI; 4-0-0
  * @param  Ctx Component object pointer
  * @retval error status
  */
int32_t MT25QL128A_ExitQPIMode(QSPI_HandleTypeDef *Ctx)
{
  QSPI_CommandTypeDef      s_command;

  /* Initialize the read ID command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = MT25QL128A_RESET_QSPI_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @brief  Flash enter 4 Byte address mode. Effect 3/4 address byte commands only.
  *         SPI/DPI/QPI; 1-0-0/2-0-0/4-0-0
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @retval error status
  */
int32_t MT25QL128A_Enter4BytesAddressMode(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the read ID command */
  s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = MT25QL128A_ENTER_4_BYTE_ADDR_MODE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @brief  Flash exit 4 Byte address mode. Effect 3/4 address byte commands only.
  *         SPI/DPI/QPI; 1-0-0/2-0-0/4-0-0
  * @param  Component object pointer
  * @retval error status
  */
int32_t MT25QL128A_Exit4BytesAddressMode(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the read ID command */
  s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = MT25QL128A_EXIT_4_BYTE_ADDR_MODE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/* ID Commands ****************************************************************/
/**
  * @brief  Read Flash 3 Byte IDs.
  *         Manufacturer ID, Memory type, Memory density
  *         SPI/DPI/QPI; 1-0-1/2-0-2/4-0-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  ID 3 bytes IDs pointer
  * @param  DualFlash Dual flash mode state
  * @retval error status
  */
int32_t MT25QL128A_ReadID(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode, uint8_t *ID, MT25QL128A_DualFlash_t DualFlash)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the read ID command */
  s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = ((Mode == MT25QL128A_QPI_MODE) || (Mode == MT25QL128A_DPI_MODE)) ? MT25QL128A_MULTIPLE_IO_READ_ID_CMD : MT25QL128A_READ_ID_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = (Mode == MT25QL128A_QPI_MODE) ? QSPI_DATA_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_DATA_2_LINES : QSPI_DATA_1_LINE;
  s_command.NbData            = (DualFlash == MT25QL128A_DUALFLASH_ENABLE) ? 6U : 3U;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(Ctx, ID, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/* Reset Commands *************************************************************/
/**
  * @brief  Flash reset enable command
  *         SPI/DPI/QPI; 1-0-0/2-0-0/4-0-0
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @retval error status
  */
int32_t MT25QL128A_ResetEnable(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the reset enable command */
  s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = MT25QL128A_RESET_ENABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @brief  Flash reset memory command
  *         SPI/DPI/QPI; 1-0-0/2-0-0/4-0-0
  * @param  Ctx Component object pointer
  * @param  Mode Interface select
  * @retval error status
  */
int32_t MT25QL128A_ResetMemory(QSPI_HandleTypeDef *Ctx, MT25QL128A_Interface_t Mode)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the reset enable command */
  s_command.InstructionMode   = (Mode == MT25QL128A_QPI_MODE) ? QSPI_INSTRUCTION_4_LINES : (Mode == MT25QL128A_DPI_MODE) ? QSPI_INSTRUCTION_2_LINES : QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = MT25QL128A_RESET_MEMORY_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DummyCycles       = 0;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_QSPI_Command(Ctx, &s_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MT25QL128A_ERROR;
  }

  return MT25QL128A_OK;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
