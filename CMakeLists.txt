cmake_minimum_required(VERSION 3.14)

set(stm32f4_internal_dir ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

function(unspun_add_stm32f4_hal target_name hal_config_folder hal_sources hal_includes)

    set(DRIVER_FOLDER ${stm32f4_internal_dir}/Drivers/STM32F4xx_HAL_Driver)
    set(MIDDLEWARE_FOLDER ${stm32f4_internal_dir}/Middlewares)
    set(CMSIS_FOLDER ${stm32f4_internal_dir}/Drivers/CMSIS)
    set(CMSIS_DEVICE_FOLDER ${CMSIS_FOLDER}/Device/ST/STM32F4xx)
    set(HAL_CONFIG_FILE ${hal_config_folder}/stm32f4xx_hal_conf.h)

    # Each project must implement its own stm32f4xx_hal_conf.h and set the variable hal_config_folder
    # before including this subdirectory. A template for this file can be found here: 
    # Drivers\STM32F4xx_HAL_Driver\Inc\stm32f4xx_hal_conf_template.h
    message(DEBUG "HAL_CONFIG_FILE=${HAL_CONFIG_FILE}")

    # Parse stm32f4xx_hal_conf.h for all the enabled modules
    if(EXISTS ${HAL_CONFIG_FILE})
        set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${HAL_CONFIG_FILE})
        file(READ ${HAL_CONFIG_FILE} conf)
        string(REGEX MATCHALL "\n[\t ]*#define[\t ]*HAL_[^\r\n\t ]*_ENABLED" out "${conf}")
        foreach(item ${out})
            string(REGEX MATCH "HAL_.*_ENABLED" module ${item})
            message(DEBUG "set ${module}=true")
            set(${module} true)
        endforeach()
    else()
        message(FATAL_ERROR "HAL_CONFIG_FILE must be defined as an existing path, cannot find path: ${HAL_CONFIG_FILE}")
    endif()

    # HAL_MODULE_ENABLED must be defined in stm32f4xx_hal_conf.h to build any of the HAL
    if(DEFINED HAL_MODULE_ENABLED)
        # check for core HAL dependencies 
        if (NOT DEFINED HAL_CORTEX_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_MODULE_ENABLED requires HAL_CORTEX_MODULE_ENABLED")
        endif()
        if (NOT DEFINED HAL_RCC_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_MODULE_ENABLED requires HAL_RCC_MODULE_ENABLED")
        endif()
        if (NOT DEFINED HAL_FLASH_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_FLASH_MODULE_ENABLED requires HAL_FLASH_MODULE_ENABLED")
        endif()
    else()
        message(FATAL_ERROR "STM32F4XX target has been included but HAL_MODULE_ENABLED is not set")
    endif()

    # Start target for STM32F413xx family of chips
    add_library(${target_name} 
        STATIC
        ${DRIVER_FOLDER}/Src/stm32f4xx_hal.c
        ${DRIVER_FOLDER}/Src/stm32f4xx_ll_utils.c 
    )

    # Add external source file
    target_sources(${target_name}
        PRIVATE
        ${hal_sources}
    )
    
    # Add sources depending on which modules are enabled
    # ---------START MODULE DEFINITIONS--------------
    if(DEFINED HAL_ADC_MODULE_ENABLED)
        message(STATUS "Adding HAL_ADC_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
        #message(FATAL_ERROR "HAL_ADC_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_adc_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_adc.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_adc.c
        )
    endif()

    if(DEFINED HAL_CRYP_MODULE_ENABLED)
        message(STATUS "Adding HAL_CRYP_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_cryp_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_cryp.c
        )
    endif()

    if(DEFINED HAL_CAN_MODULE_ENABLED)
        message(STATUS "Adding HAL_CAN_MODULE_ENABLED")
        # Check dependent modules
        if(DEFINED HAL_CAN_LEGACY_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_CAN_MODULE_ENABLED cannot be used with HAL_CAN_LEGACY_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_can.c
        )
    endif()

    if(DEFINED HAL_CRC_MODULE_ENABLED)
        message(STATUS "Adding HAL_CRC_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_crc.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_crc.c
        )
    endif()

    if(DEFINED HAL_DAC_MODULE_ENABLED)
        message(STATUS "Adding HAL_DAC_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_DAC_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_dac_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_dac.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_dac.c
        )
    endif()

    if(DEFINED HAL_DCMI_MODULE_ENABLED)
        message(STATUS "Adding HAL_DCMI_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_dcmi_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_dcmi.c
        )
    endif()

    if(DEFINED HAL_DMA2D_MODULE_ENABLED)
        message(STATUS "Adding HAL_DMA2D_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_dma2d.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_dma2d.c
        )
    endif()

    if(DEFINED HAL_ETH_MODULE_ENABLED)
        message(STATUS "Adding HAL_ETH_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_eth.c
        )
    endif()

    if(DEFINED HAL_NAND_MODULE_ENABLED)
        message(STATUS "Adding HAL_NAND_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_NOR_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_NAND_MODULE_ENABLED requires HAL_NOR_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_nand.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_fmc.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_fsmc.c
        )
    endif()

    if(DEFINED HAL_NOR_MODULE_ENABLED)
        message(STATUS "Adding HAL_NOR_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_nor.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_fmc.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_fsmc.c
        )
    endif()

    if(DEFINED HAL_PCCARD_MODULE_ENABLED)
        message(STATUS "Adding HAL_PCCARD_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_NOR_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_PCCARD_MODULE_ENABLED requires HAL_NOR_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_pccard.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_fmc.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_fsmc.c
        )
    endif()

    if(DEFINED HAL_SRAM_MODULE_ENABLED)
        message(STATUS "Adding HAL_SRAM_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_SRAM_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_sram.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_fmc.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_fsmc.c
        )
    endif()

    if(DEFINED HAL_SDRAM_MODULE_ENABLED)
        message(STATUS "Adding HAL_SDRAM_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_sdram.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_fmc.c
        )
    endif()

    if(DEFINED HAL_HASH_MODULE_ENABLED)
        message(STATUS "Adding HAL_HASH_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_hash_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_hash.c
        )
    endif()

    if(DEFINED HAL_I2C_MODULE_ENABLED)
        message(STATUS "Adding HAL_I2C_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_I2C_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_i2c_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_i2c.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_i2c.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_rcc.c
        )
    endif()

    if(DEFINED HAL_I2S_MODULE_ENABLED)
        message(STATUS "Adding HAL_I2S_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_I2S_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_i2s_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_i2s.c
        )
    endif()

    if(DEFINED HAL_IWDG_MODULE_ENABLED)
        message(STATUS "Adding HAL_IWDG_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_iwdg.c
        )
    endif()

    if(DEFINED HAL_LTDC_MODULE_ENABLED)
        message(STATUS "Adding HAL_LTDC_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_ltdc_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_ltdc.c
        )
    endif()

    if(DEFINED HAL_RNG_MODULE_ENABLED)
        message(STATUS "Adding HAL_RNG_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_rng.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_rng.c
        )
    endif()

    if(DEFINED HAL_RTC_MODULE_ENABLED)
        message(STATUS "Adding HAL_RTC_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_rtc_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_rtc.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_rtc.c
        )
    endif()

    if(DEFINED HAL_SAI_MODULE_ENABLED)
        message(STATUS "Adding HAL_SAI_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_SAI_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_sai_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_sai.c
        )
    endif()

    if(DEFINED HAL_SD_MODULE_ENABLED)
        message(STATUS "Adding HAL_SD_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_SD_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_sd.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_sdmmc.c
        )
    endif()

    if(DEFINED HAL_MMC_MODULE_ENABLED)
        message(STATUS "Adding HAL_MMC_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_MMC_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_mmc.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_sdmmc.c
        )
    endif()

    if(DEFINED HAL_SPI_MODULE_ENABLED)
        message(STATUS "Adding HAL_SPI_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_SPI_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_spi.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_spi.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_rcc.c
        )
    endif()

    if(DEFINED HAL_TIM_MODULE_ENABLED)
        message(STATUS "Adding HAL_TIM_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_TIM_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_tim_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_tim.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_tim.c
        )
    endif()

    if(DEFINED HAL_UART_MODULE_ENABLED)
        message(STATUS "Adding HAL_UART_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_UART_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_uart.c
        )
    endif()

    if(DEFINED HAL_USART_MODULE_ENABLED)
        message(STATUS "Adding HAL_USART_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_USART_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_usart.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_usart.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_rcc.c
        )
    endif()

    if(DEFINED HAL_IRDA_MODULE_ENABLED)
        message(STATUS "Adding HAL_IRDA_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_IRDA_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_irda.c
        )
    endif()

    if(DEFINED HAL_SMARTCARD_MODULE_ENABLED)
        message(STATUS "Adding HAL_SMARTCARD_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_SMARTCARD_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_smartcard.c
        )
    endif()

    if(DEFINED HAL_SMBUS_MODULE_ENABLED)
        message(STATUS "Adding HAL_SMBUS_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_smbus.c
        )
    endif()

    if(DEFINED HAL_WWDG_MODULE_ENABLED)
        message(STATUS "Adding HAL_WWDG_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_wwdg.c
        )
    endif()

    if(DEFINED HAL_PCD_MODULE_ENABLED)
        message(STATUS "Adding HAL_PCD_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_pcd_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_pcd.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_usb.c
        )
    endif()

    if(DEFINED HAL_HCD_MODULE_ENABLED)
        message(STATUS "Adding HAL_HCD_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_hcd.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_usb.c
        )
    endif()

    if(DEFINED HAL_DSI_MODULE_ENABLED)
        message(STATUS "Adding HAL_DSI_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_dsi.c
        )
    endif()

    if(DEFINED HAL_QSPI_MODULE_ENABLED)
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_QSPI_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        message(STATUS "Adding HAL_QSPI_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_qspi.c
        )
    endif()

    if(DEFINED HAL_CEC_MODULE_ENABLED)
        message(STATUS "Adding HAL_CEC_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_cec.c
        )
    endif()

    if(DEFINED HAL_FMPI2C_MODULE_ENABLED)
        message(STATUS "Adding HAL_FMPI2C_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_FMPI2C_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_fmpi2c_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_fmpi2c.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_fmpi2c.c
        )
    endif()

    if(DEFINED HAL_SPDIFRX_MODULE_ENABLED)
        message(STATUS "Adding HAL_SPDIFRX_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_spdifrx.c
        )
    endif()

    if(DEFINED HAL_DFSDM_MODULE_ENABLED)
        message(STATUS "Adding HAL_DFSDM_MODULE_ENABLED")
        # Check dependent modules
        if(NOT DEFINED HAL_DMA_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_DFSDM_MODULE_ENABLED requires HAL_DMA_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_dfsdm.c
        )
    endif()

    if(DEFINED HAL_LPTIM_MODULE_ENABLED)
        message(STATUS "Adding HAL_LPTIM_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_lptim.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_lptim.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_rcc.c
        )
    endif()

    if(DEFINED HAL_GPIO_MODULE_ENABLED)
        message(STATUS "Adding HAL_GPIO_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_gpio.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_gpio.c
        )
    endif()

    if(DEFINED HAL_EXTI_MODULE_ENABLED)
        message(STATUS "Adding HAL_EXTI_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_exti.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_exti.c
        )
    endif()

    if(DEFINED HAL_DMA_MODULE_ENABLED)
        message(STATUS "Adding HAL_DMA_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_dma_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_dma.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_dma.c
        )
    endif()

    if(DEFINED HAL_RCC_MODULE_ENABLED)
        message(STATUS "Adding HAL_RCC_MODULE_ENABLED")
        if (NOT DEFINED HAL_GPIO_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_RCC_MODULE_ENABLED requires HAL_GPIO_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_rcc_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_rcc.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_rcc.c
        )
    endif()

    if(DEFINED HAL_FLASH_MODULE_ENABLED)
        message(STATUS "Adding HAL_FLASH_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_flash_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_flash_ramfunc.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_flash.c
        )
    endif()

    if(DEFINED HAL_PWR_MODULE_ENABLED)
        message(STATUS "Adding HAL_PWR_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_pwr_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_pwr.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_ll_pwr.c
        )
    endif()

    if(DEFINED HAL_CORTEX_MODULE_ENABLED)
        message(STATUS "Adding HAL_CORTEX_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_cortex.c
        )
    endif()

    if(DEFINED HAL_FMPSMBUS_MODULE_ENABLED)
        message(STATUS "Adding HAL_FMPSMBUS_MODULE_ENABLED")
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_fmpsmbus_ex.c
            ${DRIVER_FOLDER}/Src/stm32f4xx_hal_fmpsmbus.c
        )
    endif()

    if(DEFINED HAL_CAN_LEGACY_MODULE_ENABLED)
        message(WARNING "Adding HAL_CAN_LEGACY_MODULE_ENABLED, consider moving to HAL_CAN_MODULE_ENABLED")
        # Check dependent modules
        if(DEFINED HAL_CAN_MODULE_ENABLED)
            message(FATAL_ERROR "HAL_CAN_LEGACY_MODULE_ENABLED cannot be used with HAL_CAN_MODULE_ENABLED")
        endif()
        target_sources(${target_name}
            PRIVATE
            ${DRIVER_FOLDER}/Src/Legacy/stm32f4xx_hal_can.c
        )
        target_include_directories(${target_name}
            PUBLIC
            ${DRIVER_FOLDER}/Inc/Legacy
        )
    endif()

    # optional modules
    if(ARGN)
        # USB Device, CDC class
        if("ENABLE_USB_DEVICE_CDC" IN_LIST ARGN)
            message(STATUS "Adding USB CDC device library")
            target_sources(${target_name}
                PRIVATE
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
                )
            target_include_directories(${target_name}
                PUBLIC
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Device_Library/Core/Inc
            )
            target_sources(${target_name}
                PRIVATE
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
            )
            target_include_directories(${target_name}
                PUBLIC
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Device_Library/Class/CDC/Inc
            )  
        endif()

        # USB Device, HDC class + FatFs library
        if("ENABLE_FATFS_USB_DISKIO" IN_LIST ARGN)
            message(STATUS "Adding USB MSC host library")
            target_sources(${target_name}
                PRIVATE
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
                )
            target_include_directories(${target_name}
                PUBLIC
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Host_Library/Core/Inc
            )
            target_sources(${target_name}
                PRIVATE
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c
            )
            target_include_directories(${target_name}
                PUBLIC
                ${MIDDLEWARE_FOLDER}/ST/STM32_USB_Host_Library/Class/MSC/Inc
            )

            message(STATUS "Adding FatFs library")
            target_sources(${target_name}
                PRIVATE
                ${MIDDLEWARE_FOLDER}/Third_Party/FatFs/src/diskio.c
                ${MIDDLEWARE_FOLDER}/Third_Party/FatFs/src/ff_gen_drv.c
                ${MIDDLEWARE_FOLDER}/Third_Party/FatFs/src/ff.c
                )
            target_include_directories(${target_name}
                PUBLIC
                ${MIDDLEWARE_FOLDER}/Third_Party/FatFs/src
            )
        endif()
    endif()
    # ---------END MODULE DEFINITIONS--------------

    # Expose driver include folder to targets linking against this library
    target_include_directories(${target_name}
        PUBLIC
        ${DRIVER_FOLDER}/Inc
        ${CMSIS_FOLDER}/Include
        ${CMSIS_DEVICE_FOLDER}/Include
        ${hal_config_folder}
        ${hal_includes}
    )

    # Adding compile definitions as public since they are required for any targets that use
    # the public header files
    target_compile_definitions(${target_name}
        PUBLIC
        USE_HAL_DRIVER 
        STM32F413xx
    )

    # Adding compile options to the STM32CubeF4
    # Current version of c++ throws warning that |= operator is deprecated
    # This compile option suppresses that warning
    target_compile_options(${target_name}
        PUBLIC
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-volatile>
    )


    # To build this library offset into flash, set STM32F4XX_HAL_VECTOR_OFFSET variable equal
    # to the desired offset using 0xFFFF notation
    if(DEFINED STM32F4XX_HAL_VECTOR_OFFSET)
        message(STATUS "Using vector table offset of ${STM32F4XX_HAL_VECTOR_OFFSET}")
        target_compile_definitions(${target_name}
            PUBLIC
            VECT_TAB_OFFSET=${STM32F4XX_HAL_VECTOR_OFFSET}
        )
    endif()

endfunction()
