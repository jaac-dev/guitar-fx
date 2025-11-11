#pragma once

#include <stm32f407xx.h>

#include <stdint.h>
#include <stdbool.h>

/// Defines the pull-up/pull-down resistor type for input pin configuration.
typedef enum {
	PULL_UP = 0,
	PULL_DOWN = 1,
	NONE = 2,
} gpio_pupd_t;

/// Enable the GPIO pin in the given block as an output.
void gpio_configure_out(GPIO_TypeDef *block, uint8_t pin);

/// Enable the GPIO pin in the given block as an input.
void gpio_configure_in(GPIO_TypeDef *block, uint8_t pin, gpio_pupd_t pupd);

/// Writes a value to the given GPIO pin on the given block.
void gpio_write(GPIO_TypeDef *block, uint8_t pin, bool value);