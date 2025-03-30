#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/pwm.h"

const int PWM_RED_PIN = 4; 
const int PWM_GREEN_PIN = 6;

void led_task(void *p) {
    gpio_set_function(PWM_RED_PIN, GPIO_FUNC_PWM);
    uint slice_num_red = pwm_gpio_to_slice_num(PWM_RED_PIN);
    pwm_set_clkdiv(slice_num_red, 125);
    pwm_set_wrap(slice_num_red, 100);
    pwm_set_chan_level(slice_num_red, PWM_CHAN_A, 80); 
    pwm_set_enabled(slice_num_red, true);

    gpio_set_function(PWM_GREEN_PIN, GPIO_FUNC_PWM);
    uint slice_num_green = pwm_gpio_to_slice_num(PWM_GREEN_PIN);
    pwm_set_clkdiv(slice_num_green, 125);
    pwm_set_wrap(slice_num_green, 100);
    pwm_set_chan_level(slice_num_green, PWM_CHAN_A, 20); 
    pwm_set_enabled(slice_num_green, true);

    while (true) {
    }
}

int main() {
    stdio_init_all();
    printf("Start RTOS \n");

    xTaskCreate(led_task, "LED_Task 1", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}
