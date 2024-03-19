#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t taskHandle1 = NULL;
TaskHandle_t taskHandle2 = NULL;

void task1(void *args)
{
    int count = 0;
    while(1)
    {
        count++;
        printf("Task 1 count: %d\n", count);
        if (count == 3)
        {
            printf("Suspending Task 2 from Task 1\n");
            vTaskSuspend(taskHandle2);
        }
        if (count == 10)
        {
            printf("Resuming Task 2 from Task 1\n");
            vTaskResume(taskHandle2);
        }
        if (count == 15)
        {
            printf("Deleting Task 2 from Task 1\n");
            vTaskDelete(taskHandle2);
        }
        if (count == 20)
        {
            printf("Deleting Task 1 from Task 1\n");
            vTaskDelete(taskHandle1);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task2(void *args)
{
    int count = 0;
    while(1)
    {
        count++;
        printf("Task 2 count: %d\n", count);
        if (count == 20)
        {
            printf("Deleting Task 2\n");
            vTaskDelete(taskHandle2);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(task1, "task1", 4096, NULL, 10, &taskHandle1);
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &taskHandle2, 1);
}
