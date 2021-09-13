#include <app_event_handler.h>
#include <tk_gpio.h>
#include <tk_button.h>

static cur_action_t action = HANDLER_NOTHING;

static void handle_event(button_event_t button_event);

static void expiry_function_2s()
{
    if (action == HANDLER_DOUBLE_PRESS)
    {
        handle_event(BUTTON_EVT_SINGLE_PRESS);
    }
}

static void expiry_function_3s()
{
    if (action == HANDLER_TRIPLE_PRESS)
    {
        handle_event(BUTTON_EVT_DOUBLE_PRESS);
    }
}

static void expiry_function_10s()
{
    handle_event(BUTTON_EVT_10SEC_HOLD);
}

static void expiry_function_30s()
{
    handle_event(BUTTON_EVT_30_SEC_HOLD);
}

K_TIMER_DEFINE(timer_10s, expiry_function_10s, NULL);
K_TIMER_DEFINE(timer_30s, expiry_function_30s, NULL);
K_TIMER_DEFINE(timer_2s, expiry_function_2s, NULL);
K_TIMER_DEFINE(timer_3s, expiry_function_3s, NULL);

static void handle_event(button_event_t button_event)
{
    action = HANDLER_NOTHING;
    k_timer_stop(&timer_2s);
    k_msgq_put(&my_msgq, &button_event, K_NO_WAIT);
}

static void choose_behaviour(button_state_t button_s)
{
    switch (button_s)
    {
    case DOWN:
        // Timers need to be started up then button is pushed
        k_timer_start(&timer_30s, K_SECONDS(30), K_NO_WAIT);
        k_timer_start(&timer_10s, K_SECONDS(10), K_NO_WAIT);

        if (action == HANDLER_NOTHING)
        {
            action = HANDLER_FIRST_PUSH;

            // Started at since the moment when button is pushed for the first time
            k_timer_start(&timer_2s, K_SECONDS(2), K_MSEC(200));
            k_timer_start(&timer_3s, K_SECONDS(3), K_NO_WAIT);
        }
        break;

    case UP:
        // Timers need to be stopped then button is not pushed
        k_timer_stop(&timer_30s);
        k_timer_stop(&timer_10s);

        switch (action)
        {
        case HANDLER_FIRST_PUSH:
            action = HANDLER_DOUBLE_PRESS;
            break;

        case HANDLER_DOUBLE_PRESS:
            action = HANDLER_TRIPLE_PRESS;
            break;

        case HANDLER_TRIPLE_PRESS:
            k_timer_stop(&timer_3s);
            handle_event(BUTTON_EVT_TRIPLE_PRESS);
            break;

        default:
            // Nothing to do
            break;
        }
        break;

    default:
        // Nothing to do
        break;
    }
}

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    // For debouncing timing
    static int32_t saved_time = 0;
    // To decide in which position the button is
    static button_state_t button_s = UP;

    // Debouncing feature
    if (k_uptime_get_32() - saved_time > 20)
    {
        saved_time = k_uptime_get_32();
        button_s = !button_s;
        choose_behaviour(button_s);
    }
}