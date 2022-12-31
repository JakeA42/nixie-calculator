/*
 * clocks.c
 * Based on the arduino core for SAMD21
 * Created: 2021-07-31 8:57:47 PM
 *  
 */ 

#include "clocks.h"
#include <sam.h>

#define VARIANT_MAINOSC (32768ul)
#define VARIANT_MCK  (48000000ul)

#define GENERIC_CLOCK_GENERATOR_MAIN      (0u)
#define GENERIC_CLOCK_GENERATOR_XOSC32K   (1u)
#define GENERIC_CLOCK_GENERATOR_OSC32K    (1u)
#define GENERIC_CLOCK_GENERATOR_OSCULP32K (2u) /* Initialized at reset for WDT */
#define GENERIC_CLOCK_GENERATOR_OSC8M     (3u)
// Constants for Clock multiplexers
#define GENERIC_CLOCK_MULTIPLEXER_DFLL48M (0u)

void ClocksInit() {
	/* Set 1 Flash Wait State for 48MHz, cf tables 20.9 and 35.27 in SAMD21 Datasheet */
	NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val ;

	/* Turn on the digital interface clock */
	PM->APBAMASK.reg |= PM_APBAMASK_GCLK ;

	/* Enable XOSC32K clock (External on-board 32.768Hz oscillator) */
	SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_STARTUP( 0x6u ) | /* cf table 15.10 of product datasheet in chapter 15.8.6 */
	SYSCTRL_XOSC32K_XTALEN | SYSCTRL_XOSC32K_EN32K ;
	SYSCTRL->XOSC32K.bit.ENABLE = 1 ; /* separate call, as described in chapter 15.6.3 */
	while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_XOSC32KRDY) == 0 )
	{
		/* Wait for oscillator stabilization */
	}

	/* Software reset the module to ensure it is re-initialized correctly */
	GCLK->CTRL.reg = GCLK_CTRL_SWRST ;
	while ( (GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) )
	{
		/* Wait for reset to complete */
	}

	/* Put XOSC32K as source of Generic Clock Generator 1 */
	GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_XOSC32K ) ; // Generic Clock Generator 1
	while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
	{
		/* Wait for synchronization */
	}

	/* Write Generic Clock Generator 1 configuration */
	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_OSC32K ) | // Generic Clock Generator 1
	GCLK_GENCTRL_SRC_XOSC32K | // Selected source is External 32KHz Oscillator
	//                  GCLK_GENCTRL_OE | // Output clock to a pin for tests
	GCLK_GENCTRL_GENEN ;
	while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
	{
		/* Wait for synchronization */
	}

	/* Put Generic Clock Generator 1 as source for Generic Clock Multiplexer 0 (DFLL48M reference) */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID( GENERIC_CLOCK_MULTIPLEXER_DFLL48M ) | // Generic Clock Multiplexer 0
	GCLK_CLKCTRL_GEN_GCLK1 | // Generic Clock Generator 1 is source
	GCLK_CLKCTRL_CLKEN ;
	while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
	{
		/* Wait for synchronization */
	}

	/* Enable DFLL48M clock */
	SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE;
	while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
	{
		/* Wait for synchronization */
	}
	SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_CSTEP( 31 ) | // Coarse step is 31, half of the max value
	SYSCTRL_DFLLMUL_FSTEP( 511 ) | // Fine step is 511, half of the max value
	SYSCTRL_DFLLMUL_MUL( (VARIANT_MCK + VARIANT_MAINOSC/2) / VARIANT_MAINOSC ) ; // External 32KHz is the reference
	while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
	{
		/* Wait for synchronization */
	}

	/* Write full configuration to DFLL control register */
	SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_MODE | /* Enable the closed loop mode */
	SYSCTRL_DFLLCTRL_WAITLOCK |
	SYSCTRL_DFLLCTRL_QLDIS ; /* Disable Quick lock */
	while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
	{
		/* Wait for synchronization */
	}

	/* Enable the DFLL */
	SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_ENABLE ;
	while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKC) == 0 ||
	(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKF) == 0 )
	{
		/* Wait for locks flags */
	}
	while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
	{
		/* Wait for synchronization */
	}

	/* Switch Generic Clock Generator 0 to DFLL48M. CPU will run at 48MHz. */
	GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_MAIN ) ; // Generic Clock Generator 0
	while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
	{
		/* Wait for synchronization */
	}

	/* Write Generic Clock Generator 0 configuration */
	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_MAIN ) | // Generic Clock Generator 0
	GCLK_GENCTRL_SRC_DFLL48M | // Selected source is DFLL 48MHz
	//                  GCLK_GENCTRL_OE | // Output clock to a pin for tests
	GCLK_GENCTRL_IDC | // Set 50/50 duty cycle
	GCLK_GENCTRL_GENEN ;
	while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
	{
		/* Wait for synchronization */
	}
}


