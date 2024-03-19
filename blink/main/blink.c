#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN 2
#define DELAY 1000

int led_state = 0;

void app_main(void)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(LED_PIN, led_state);
        led_state = !led_state;
        vTaskDelay(DELAY / portTICK_PERIOD_MS);
    }
}
