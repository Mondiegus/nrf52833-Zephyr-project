#include <app_event_handler.h>
#include <tk_gpio.h>
#include <tk_button.h>

LOG_MODULE_REGISTER(app);
struct k_msgq my_msgq;
char __aligned(4) my_msgq_buffer[10 * sizeof(button_event_t)];

K_THREAD_STACK_DEFINE(stack_area, STACKSIZE);
struct k_thread my_thread;

void button_thread()
{
    // button_event_t events;
    button_event_t last;

    while (1)
    {

        if (k_msgq_num_used_get(&my_msgq) > 0)
        {
            k_msgq_get(&my_msgq, &last, K_MSEC(200));
            switch (last)
            {
            case BUTTON_EVT_SINGLE_PRESS:
                tk_gpio_set(LED1, true);
                printk("BUTTON_EVT_SINGLE_PRESS\n");
                break;

            case BUTTON_EVT_DOUBLE_PRESS:
                tk_gpio_set(LED2, true);
                printk("BUTTON_EVT_DOUBLE_PRESS\n");
                break;

            case BUTTON_EVT_TRIPLE_PRESS:
                tk_gpio_set(LED3, true);
                printk("BUTTON_EVT_TRIPLE_PRESS\n");
                break;

            case BUTTON_EVT_10SEC_HOLD:
                tk_gpio_set(LED4, true);
                printk("BUTTON_EVT_10SEC_HOLD\n");
                break;

            case BUTTON_EVT_30_SEC_HOLD:
                switch_all_LEDS(true);
                printk("BUTTON_EVT_30_SEC_HOLD\n");
                break;

            default:
                break;
            }
        }
        else
        {
            switch_all_LEDS(false);
        }

        if (tk_gpio_get(BUTTON))
        {
            printk("Button pushed :) \n Timestamp: %dms\n", k_uptime_get_32());
        }
        k_msleep(SLEEP_TIME_MS);
    }
}

void main(void)
{
    k_msgq_init(&my_msgq, my_msgq_buffer, sizeof(button_event_t), 10);

    gpio_init();

    k_thread_create(&my_thread, stack_area,
                    K_THREAD_STACK_SIZEOF(stack_area),
                    &button_thread,
                    NULL, NULL, NULL,
                    0, 0, K_NO_WAIT);
}
