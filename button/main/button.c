#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

#define LED_PIN 2
#define BTN_PIN 4

int led_state = 0;

void app_main(void)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BTN_PIN, GPIO_MODE_INPUT);

    while (1)
    {
        led_state = gpio_get_level(BTN_PIN);
        gpio_set_level(LED_PIN, led_state);
    }
}
