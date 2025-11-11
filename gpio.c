#include "gpio.h"

void gpio_configure_out(GPIO_TypeDef *block, uint8_t pin) {
	block->MODER = (block->MODER & ~(0b11 << pin * 2)) | (0b01 << pin * 2); // General purpose output.
	block->OTYPER &= ~(0b1 << pin); // Push-pull.
}

void gpio_write(GPIO_TypeDef *block, uint8_t pin, bool value) {
	if (value)
		block->ODR |= 0b1 << pin;
	else
		block->ODR &= ~(0b1 << pin);
}