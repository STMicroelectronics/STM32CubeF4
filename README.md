# STM32CubeF4 MCU Firmware Package

![latest tag](https://img.shields.io/github/v/tag/STMicroelectronics/STM32CubeF4.svg?color=brightgreen)

**STM32Cube** is an STMicroelectronics original initiative to ease the developers life by reducing efforts, time and cost.

**STM32Cube** covers the overall STM32 products portfolio. It includes a comprehensive embedded software platform delivered for each STM32 series.
   * The CMSIS modules (core and device) corresponding to the ARM(tm) core implemented in this STM32 product.
   * The STM32 HAL-LL drivers : an abstraction drivers layer, the API ensuring maximized portability across the STM32 portfolio
   * The BSP drivers of each evaluation, demonstration or nucleo board provided for this STM32 series.
   * A consistent set of middleware libraries such as RTOS, USB, FatFS, graphics, touch sensing library...
   * A full set of software projects (basic examples, applications and/or demonstrations) for each board provided by this STM32 series

The **STM32CubeF4 MCU Package** projects are directly running on the STM32F4 series boards. You can find in each Projects/*Board name* directories a set of software projects (Applications/Demonstration/Examples).

In this FW Package, the modules **Middlewares/ST/TouchGFX** **Middlewares/ST/STemWin** **Middlewares/ST/STM32_Audio** are not directly accessible. They must be downloaded from a ST server, the respective URL are available in a readme.txt file inside each module.

## Release note

Details about the content of this release are available in the release note [here](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeF4/blob/master/Release_Notes.html).

## Boards available

  * STM32F4
    * [STM32F4-Discovery](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/stm32f4discovery.html)
    * [STM32F401-Discovery](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-mpu-eval-tools/stm32-mcu-mpu-eval-tools/stm32-discovery-kits/32f401cdiscovery.html)
    * [STM32F401RE-Nucleo](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f401re.html)
    * [STM32F410xx-Nucleo](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f410rb.html)
    * [STM32F411E-Discovery](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/32f411ediscovery.html)
    * [STM32F411RE-Nucleo](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f411re.html)
    * [STM32F412G-Discovery](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/32f412gdiscovery.html)
    * [STM32F412ZG-Nucleo](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f412zg.html)
    * [STM32F413H-Discovery](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/32f413hdiscovery.html)
    * [STM32F413ZH-Nucleo](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f413zh.html)
    * [STM32F429I-Discovery](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/32f429idiscovery.html)
    * [STM32F429ZI-Nucleo](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f429zi.html)
    * [STM32F446ZE-Nucleo](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f446ze.html)
    * [STM32429I_EVAL](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-eval-boards/stm32429i-eval.html)
    * [STM32439I_EVAL](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-eval-boards/stm32439i-eval.html)
    * [STM3240G_EVAL](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-eval-boards/stm3240g-eval.html)
    * [STM3241G_EVAL](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-eval-boards/stm3241g-eval.html)
    * [STM32446E_EVAL](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-eval-boards/stm32446e-eval.html)
    * [STM32446E-Nucleo](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f446re.html)
    * [STM32469I_EVAL](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-eval-boards/stm32479i-eval.html)
    * [STM32469I-Discovery](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/32f469idiscovery.html)

## Troubleshooting

**Caution** : The issues and the pull-requests are **strictly limited** to submit problems or suggestions related to the software delivered in this repository.

**For any other question** related to the product, the hardware performance or characteristics, the tools, the environment, you can submit it to the **ST Community** on the STM32 MCUs related [page](https://community.st.com/s/group/0F90X000000AXsASAW/stm32-mcus).
