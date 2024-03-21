#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_http_server.h"

esp_err_t api_get_handler(httpd_req_t *req)
{
    char* resp_str = "Hello, World!";
    httpd_resp_send(req, resp_str, strlen(resp_str));
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
}
