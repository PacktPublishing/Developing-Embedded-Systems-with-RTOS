#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void led_task_1(void *pvParameter) {

    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    
    while(1)
    {
        // blink LED
        gpio_set_level(GPIO_NUM_2, 1);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        gpio_set_level(GPIO_NUM_2, 0);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    xTaskCreate(&led_task_1, "led_task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}
