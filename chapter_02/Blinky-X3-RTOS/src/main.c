#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define NUMBER_OF_TASKS (3)

typedef struct {
    gpio_num_t pin;
    uint32_t   on_time;
    uint32_t   off_time;
} task_parameters_t;

typedef struct {
    TaskFunction_t function;
    char *name;
    uint32_t   stack_size;
    UBaseType_t   priority;
    task_parameters_t parameters;
} task_create_parameters_t;

void led_task(void *pvParameter) {

    task_parameters_t *my_params = (task_parameters_t *) pvParameter;

    gpio_set_direction(my_params->pin, GPIO_MODE_OUTPUT);
    while(1)
    {
        // blink LED
        gpio_set_level(my_params->pin, 1);
        vTaskDelay(my_params->on_time / portTICK_PERIOD_MS);
        gpio_set_level(my_params->pin, 0);
        vTaskDelay(my_params->off_time / portTICK_PERIOD_MS);
    }
}

task_create_parameters_t task_params[NUMBER_OF_TASKS] = {
    { &led_task, "LED_TASK_2", configMINIMAL_STACK_SIZE, 1, { GPIO_NUM_26, 1000, 1000 } },
    { &led_task, "LED_TASK_1", configMINIMAL_STACK_SIZE, 1, { GPIO_NUM_27, 500, 500 } },
    { &led_task, "LED_TASK_3", configMINIMAL_STACK_SIZE, 1, { GPIO_NUM_25, 2000, 2000 } }
};

void app_main() {
    
    int i = 0;

    printf("Starting app_main\n");

    for( i = 0; i < NUMBER_OF_TASKS; i++ )
    {
        xTaskCreate( task_params[i].function, task_params[i].name, task_params[i].stack_size, 
                 (void *)&(task_params[i].parameters), task_params[i].priority, NULL );
    }

}
