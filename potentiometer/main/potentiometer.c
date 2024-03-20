#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"

adc_oneshot_unit_handle_t handle;
adc_oneshot_unit_init_cfg_t init_cfg = {
    .unit_id = ADC_UNIT_1,
};
adc_oneshot_chan_cfg_t chan_cfg = {
    .bitwidth = ADC_BITWIDTH_12,
    .atten = ADC_ATTEN_DB_12,
};

int raw_value;

void app_main(void)
{
    adc_oneshot_new_unit(&init_cfg, &handle);
    adc_oneshot_config_channel(handle, ADC_CHANNEL_4, &chan_cfg);

    while (1) {
        adc_oneshot_read(handle, ADC_CHANNEL_4, &raw_value);
        printf("ADC Raw Value: %d\n", raw_value);
        vTaskDelay(pdMS_TO_TICKS(250));
    }

    adc_oneshot_del_unit(handle);
}
