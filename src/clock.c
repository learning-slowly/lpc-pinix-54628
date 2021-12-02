#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <init.h>
#include <drivers/clock_control.h>
#include "fsl_clock.h"
#include "fsl_power.h"


#define BOARD_BOOTCLOCKPLL220M_CORE_CLOCK   220000000U    /*!< Core clock frequency:220000000Hz */

static int BOARD_BootClockPLL220M(const struct device *dev)
{
    /*!< Set up the clock sources */
    /*!< Set up FRO */
    POWER_DisablePD(kPDRUNCFG_PD_FRO_EN);                   /*!< Ensure FRO is on  */
    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);                  /*!< Switch to FRO 12MHz first to ensure we can change voltage without accidentally
                                                                being below the voltage for current speed */
    POWER_SetVoltageForFreq(220000000U);             /*!< Set voltage for the one of the fastest clock outputs: System clock output */
    CLOCK_SetFLASHAccessCyclesForFreq(220000000U);    /*!< Set FLASH wait states for core */

    /*!< Set up SYS PLL */
    const pll_setup_t pllSetup = {
        .pllctrl =  SYSCON_SYSPLLCTRL_SELI(34U) | SYSCON_SYSPLLCTRL_SELP(31U) | SYSCON_SYSPLLCTRL_SELR(0U),
        .pllmdec = (SYSCON_SYSPLLMDEC_MDEC(13243U)),
        .pllndec = (SYSCON_SYSPLLNDEC_NDEC(1U)),
        .pllpdec = (SYSCON_SYSPLLPDEC_PDEC(98U)),
        .pllRate = 220000000U,
        .flags =  PLL_SETUPFLAG_WAITLOCK | PLL_SETUPFLAG_POWERUP
    };
    CLOCK_AttachClk(kFRO12M_to_SYS_PLL);        /*!< Set sys pll clock source*/
    CLOCK_SetPLLFreq(&pllSetup);                 /*!< Configure PLL to the desired value */
    /*!< Need to make sure ROM and OTP has power(PDRUNCFG0[17,29]= 0U) 
         before calling this API since this API is implemented in ROM code */
    CLOCK_SetupFROClocking(96000000U);              /*!< Set up high frequency FRO output to selected frequency */

    /*!< Set up dividers */
    CLOCK_SetClkDiv(kCLOCK_DivAhbClk, 1U, false);                  /*!< Reset divider counter and set divider to value 1 */
    CLOCK_SetClkDiv(kCLOCK_DivUsb0Clk, 0U, true);                  /*!< Reset USB0CLKDIV divider counter and halt it */
    CLOCK_SetClkDiv(kCLOCK_DivUsb0Clk, 2U, false);                  /*!< Set USB0CLKDIV divider to value 2 */

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kSYS_PLL_to_MAIN_CLK);                    /*!< Switch MAIN_CLK to SYS_PLL */
    CLOCK_AttachClk(kFRO_HF_to_USB0_CLK);                    /*!< Switch USB0_CLK to FRO_HF */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);                    /*!< Switch FLEXCOMM0 to FRO12M */
    SYSCON->MAINCLKSELA = ((SYSCON->MAINCLKSELA & ~SYSCON_MAINCLKSELA_SEL_MASK) | SYSCON_MAINCLKSELA_SEL(0U)); /*!< Switch MAINCLKSELA to FRO12M even it is not used for MAINCLKSELB */
	return 0;
}


SYS_INIT(BOARD_BootClockPLL220M, PRE_KERNEL_1, 10);



