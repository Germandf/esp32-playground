#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include "nvs.h"

esp_err_t error;
nvs_handle_t storage;
int32_t value;

void print_status(const char* success_message)
{
    error != ESP_OK ? 
        printf("Error: %s\n", esp_err_to_name(error)) : 
        printf("%s\n", success_message);
}

void app_main(void)
{
    error = nvs_flash_init();
    print_status("NVS Flash initialized");

    error = nvs_open("storage", NVS_READWRITE, &storage);
    print_status("NVS handle opened");

    error = nvs_set_i32(storage, "int", 10);
    print_status("Integer value set");

    error = nvs_commit(storage);
    print_status("Changes committed");

    error = nvs_get_i32(storage, "int", &value);
    print_status("Integer value retrieved");

    printf("Value: %ld\n", value);

    nvs_close(storage);
    printf("NVS handle closed\n");
}
