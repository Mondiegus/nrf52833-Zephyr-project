#ifndef GPIO_H
#define GPIO_H

#include <drivers/gpio.h>

#define NUMBER_OF_LEDS 4
#define BUTTON_NUM 4
#define SW1_NODE DT_ALIAS(sw1)
#if !DT_NODE_HAS_STATUS(SW1_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif

typedef enum
{
    LED1,
    LED2,
    LED3,
    LED4,
    BUTTON,
} leds_t;

void gpio_init();
void switch_all_LEDS(bool value);
void tk_gpio_set(leds_t choosed_led, bool value);
bool tk_gpio_get(leds_t choosed_led);

#endif