#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "driver/ledc.h"

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_HIGH_SPEED_MODE
#define LEDC_OUTPUT 2
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_12_BIT
#define LEDC_FREQUENCY 5000
#define LEDC_DUTY 0
#define LEDC_HPOINT 0

#define ADC_UNIT ADC_UNIT_1
#define ADC_CHANNEL ADC_CHANNEL_4
#define ADC_BITWIDTH ADC_BITWIDTH_12
#define ADC_ATTEN_DB ADC_ATTEN_DB_12

ledc_timer_config_t ledc_timer = {
    .speed_mode = LEDC_MODE,
    .timer_num = LEDC_TIMER,
    .duty_resolution = LEDC_DUTY_RES,
    .freq_hz = LEDC_FREQUENCY,
    .clk_cfg = LEDC_AUTO_CLK,
};

ledc_channel_config_t ledc_channel = {
    .speed_mode = LEDC_MODE,
    .channel = LEDC_CHANNEL,
    .timer_sel = LEDC_TIMER,
    .intr_type = LEDC_INTR_DISABLE,
    .gpio_num = LEDC_OUTPUT,
    .duty = LEDC_DUTY,
    .hpoint = LEDC_HPOINT,
};

adc_oneshot_unit_handle_t handle;

adc_oneshot_unit_init_cfg_t init_cfg = {
    .unit_id = ADC_UNIT,
};

adc_oneshot_chan_cfg_t chan_cfg = {
    .bitwidth = ADC_BITWIDTH,
    .atten = ADC_ATTEN_DB,
};

int raw_value;

void app_main(void)
{
    ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel);
    adc_oneshot_new_unit(&init_cfg, &handle);
    adc_oneshot_config_channel(handle, ADC_CHANNEL, &chan_cfg);

    while (1) {
        adc_oneshot_read(handle, ADC_CHANNEL, &raw_value);
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, raw_value);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
