#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_http_server.h"
#include "driver/ledc.h"

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_HIGH_SPEED_MODE
#define LEDC_OUTPUT_R 26
#define LEDC_OUTPUT_G 25
#define LEDC_OUTPUT_B 33
#define LEDC_CHANNEL_R LEDC_CHANNEL_0
#define LEDC_CHANNEL_G LEDC_CHANNEL_1
#define LEDC_CHANNEL_B LEDC_CHANNEL_2
#define LEDC_DUTY_RES LEDC_TIMER_8_BIT
#define LEDC_FREQUENCY 5000
#define LEDC_DUTY 0
#define LEDC_HPOINT 0

int led_r = 0, led_g = 0, led_b = 0;

ledc_timer_config_t ledc_timer = {
    .speed_mode = LEDC_MODE,
    .timer_num = LEDC_TIMER,
    .duty_resolution = LEDC_DUTY_RES,
    .freq_hz = LEDC_FREQUENCY,
    .clk_cfg = LEDC_AUTO_CLK,
};

ledc_channel_config_t ledc_channel_r = {
    .speed_mode = LEDC_MODE,
    .channel = LEDC_CHANNEL_R,
    .timer_sel = LEDC_TIMER,
    .intr_type = LEDC_INTR_DISABLE,
    .gpio_num = LEDC_OUTPUT_R,
    .duty = LEDC_DUTY,
    .hpoint = LEDC_HPOINT,
};

ledc_channel_config_t ledc_channel_g = {
    .speed_mode = LEDC_MODE,
    .channel = LEDC_CHANNEL_G,
    .timer_sel = LEDC_TIMER,
    .intr_type = LEDC_INTR_DISABLE,
    .gpio_num = LEDC_OUTPUT_G,
    .duty = LEDC_DUTY,
    .hpoint = LEDC_HPOINT,
};

ledc_channel_config_t ledc_channel_b = {
    .speed_mode = LEDC_MODE,
    .channel = LEDC_CHANNEL_B,
    .timer_sel = LEDC_TIMER,
    .intr_type = LEDC_INTR_DISABLE,
    .gpio_num = LEDC_OUTPUT_B,
    .duty = LEDC_DUTY,
    .hpoint = LEDC_HPOINT,
};

void update_leds()
{
    // workaround for common anode RGB LED
    uint32_t inverted_duty_r = 256 - led_r;
    uint32_t inverted_duty_g = 256 - led_g;
    uint32_t inverted_duty_b = 256 - led_b;
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_R, inverted_duty_r);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_R);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_G, inverted_duty_g);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_G);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_B, inverted_duty_b);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_B);
}

esp_err_t api_get_handler(httpd_req_t *req)
{
    char* buf;
    size_t buf_len;
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            char param[32];
            led_r = httpd_query_key_value(buf, "r", param, sizeof(param)) == ESP_OK ? atoi(param) : 0;
            led_g = httpd_query_key_value(buf, "g", param, sizeof(param)) == ESP_OK ? atoi(param) : 0;
            led_b = httpd_query_key_value(buf, "b", param, sizeof(param)) == ESP_OK ? atoi(param) : 0;
            update_leds();
        }
        free(buf);
    }

    httpd_resp_set_hdr(req, "Content-Type", "application/json");

    char res[64];
    sprintf(res, "{ \"r\": %d, \"g\": %d, \"b\": %d }", led_r, led_g, led_b);
    httpd_resp_send(req, res, strlen(res));
    return ESP_OK;
}

void web_server_init()
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t uri = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = api_get_handler,
        };
        httpd_register_uri_handler(server, &uri);
    }
}

void app_main(void)
{
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    example_connect();
    web_server_init();
    ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel_r);
    ledc_channel_config(&ledc_channel_g);
    ledc_channel_config(&ledc_channel_b);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    update_leds();
}
