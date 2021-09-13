
#include <tk_gpio.h>
#include <tk_button.h>

static struct gpio_callback button_cb_data;
static struct gpio_dt_spec gpios_t[5] = {
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(led2), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_ALIAS(led3), gpios, {0}),
    GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios, {0})};

void switch_all_LEDS(bool value)
{
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++)
    {
        gpio_pin_set(gpios_t[i].port, gpios_t[i].pin, value);
    }
}

void gpio_init()
{
    /* Make sure button is ready */
    if (!device_is_ready(gpios_t[BUTTON_NUM].port))
    {
        printk("Error: button device %s is not ready\n",
               gpios_t[BUTTON_NUM].port->name);
        return;
    }

    /* Configure button as input */
    int button_ret = gpio_pin_configure_dt(&gpios_t[BUTTON_NUM], GPIO_INPUT);
    if (button_ret)
    {
        printk("Error %d: failed to configure %s pin %d\n", button_ret, gpios_t[BUTTON_NUM].port->name, gpios_t[BUTTON_NUM].pin);
        return;
    }
    else
    {
        printk("Set up Button\n");
    }

    /* Configure button interrupt based on device tree */
    button_ret = gpio_pin_interrupt_configure_dt(&gpios_t[BUTTON_NUM], GPIO_INT_EDGE_BOTH);
    if (button_ret)
    {
        printk("Error %d: failed to configure interrupt on %s pin %d\n", button_ret, gpios_t[BUTTON_NUM].port->name, gpios_t[BUTTON_NUM].pin);
        return;
    }
    else
    {
        gpio_init_callback(&button_cb_data, button_pressed, BIT(gpios_t[BUTTON_NUM].pin));
        gpio_add_callback(gpios_t[BUTTON_NUM].port, &button_cb_data);
    }

    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++)
    {
        int led_ret = gpio_pin_configure_dt(&gpios_t[i], GPIO_OUTPUT);
        if (led_ret)
        {
            printk("Error %d: failed to configure LED device %s pin %d\n",
                   led_ret, gpios_t[i].port->name, gpios_t[i].pin);
            gpios_t[i].port = NULL;
        }
        else
        {
            printk("Set up LED at %s pin %d\n", gpios_t[i].port->name, gpios_t[i].pin);
        }
    }
}

void tk_gpio_set(leds_t choosed_led, bool value)
{
    gpio_pin_set(gpios_t[choosed_led].port, gpios_t[choosed_led].pin, value);
}

bool tk_gpio_get(leds_t choosedGPIO)
{
    return gpio_pin_get(gpios_t[choosedGPIO].port, gpios_t[choosedGPIO].pin);
}