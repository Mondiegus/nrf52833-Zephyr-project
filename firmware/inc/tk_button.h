#ifndef BUTTON_H
#define BUTTON_H

#include <inttypes.h>
#include <sys/util.h>
#include <zephyr.h>

typedef enum
{
    UP,
    DOWN,
} button_state_t;

typedef enum
{
    HANDLER_NOTHING,
    HANDLER_FIRST_PUSH,
    HANDLER_DOUBLE_PRESS,
    HANDLER_TRIPLE_PRESS,
} cur_action_t;

typedef enum
{
    BUTTON_EVT_SINGLE_PRESS,
    BUTTON_EVT_DOUBLE_PRESS,
    BUTTON_EVT_TRIPLE_PRESS,
    BUTTON_EVT_10SEC_HOLD,
    BUTTON_EVT_30_SEC_HOLD,
} button_event_t;

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

#endif