#include "delay.h"
#include "lcd.h"
#include "gpio.h"

/// Initialize the high speed clock.
void hs_clock_init();

/// Initialize LCD A.
lcd_t lcd_a_init();

/// Main entrypoint for the firmware.
int main() {
	// Initialize the high speed clock.
	hs_clock_init();
	
	// Initialize the delay system.
	delay_init();
	
	// Initialize LCD A.
	lcd_t lcd_a = lcd_a_init();
	
	// Write a message to the LCD.
	lcd_print(&lcd_a, "Hello, world!");
	
	while (1) {}
	return 0;
}

void hs_clock_init() {
	// Enable the high speed oscillator.
	RCC->CR |= RCC_CR_HSEON;
	while ((RCC->CR & RCC_CR_HSERDY) == 0) {}
		
	// Set the power scale.
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;

	// Configure the flash latency.
	FLASH->ACR =
		FLASH_ACR_ICEN |
		FLASH_ACR_DCEN |
		FLASH_ACR_PRFTEN |
    FLASH_ACR_LATENCY_5WS;
	while ((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_5WS) {}

	// Configure the prescalars.
	RCC->CFGR &= ~(RCC_CFGR_HPRE  |
	  RCC_CFGR_PPRE1 |
	  RCC_CFGR_PPRE2);
		
	RCC->CFGR |=
		RCC_CFGR_HPRE_DIV1  |  // AHB  = 168 MHz
		RCC_CFGR_PPRE1_DIV4 |  // APB1 = 42 MHz
		RCC_CFGR_PPRE2_DIV2;   // APB2 = 84 MHz

	// Configure the PLL.
	RCC->CR &= ~RCC_CR_PLLON;
  while (RCC->CR & RCC_CR_PLLRDY) {}

	RCC->PLLCFGR = 0;

	// PLLM = 8
	RCC->PLLCFGR |= 8U << 0;
	// PLLN = 336
	RCC->PLLCFGR |= 336U << 6;
	// PLLP = 2  -> PLLP bits = 00
	// (already 0 from clearing, so nothing to do)
	// PLL source = HSE
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	// PLLQ = 7  -> 48 MHz for USB, etc.
	RCC->PLLCFGR |= 7U << 24;

	// 6. Enable PLL and wait
	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) == 0) {}

	// 7. Switch SYSCLK to PLL
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;   // select PLL as system clock

	// Wait until PLL is used as system clock
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
	
  // Update the CMSIS core clock.
	SystemCoreClock = 168000000U;
}

lcd_t lcd_a_init() {
	// Enable the clock for block C.
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	
	gpio_configure_out(GPIOC, 6);
	gpio_configure_out(GPIOC, 7);
	gpio_configure_out(GPIOC, 8);
	gpio_configure_out(GPIOC, 9);
	gpio_configure_out(GPIOC, 10);
	gpio_configure_out(GPIOC, 11);
	
	// Setup the LCD.
	lcd_t lcd_a;
	lcd_a.rs_block = GPIOC;
	lcd_a.rs_pin = 6;
	lcd_a.en_block = GPIOC;
	lcd_a.en_pin = 7;
	lcd_a.d4_block = GPIOC;
	lcd_a.d4_pin = 8;
	lcd_a.d5_block = GPIOC;
	lcd_a.d5_pin = 9;
	lcd_a.d6_block = GPIOC;
	lcd_a.d6_pin = 10;
	lcd_a.d7_block = GPIOC;
	lcd_a.d7_pin = 11;

	lcd_init(&lcd_a);
	
	return lcd_a;
}