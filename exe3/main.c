#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "data.h"

#define QUEUE_LENGTH 64
#define QUEUE_ITEM_SIZE sizeof(int)
#define FILTER_SIZE 5

QueueHandle_t queueData;

void taskFeedData(void *params) {
    vTaskDelay(pdMS_TO_TICKS(400)); 

    int signalLength = sizeof(sine_wave_four_cycles) / sizeof(sine_wave_four_cycles[0]);

    for (int i = 0; i < signalLength; i++) {
        xQueueSend(queueData, &sine_wave_four_cycles[i], portMAX_DELAY);
    }
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void taskProcessData(void *params) {
    int buffer[FILTER_SIZE] = {0};
    int newData = 0;

    while (true) {
        if (xQueueReceive(queueData, &newData, pdMS_TO_TICKS(100))) {

            for (int i = 0; i < FILTER_SIZE - 1; i++) {
                buffer[i] = buffer[i + 1];
            }
            buffer[FILTER_SIZE - 1] = newData;

            int sum = 0;
            for (int i = 0; i < FILTER_SIZE; i++) {
                sum += buffer[i];
            }
            float average = sum / (float)FILTER_SIZE;

            printf("Média móvel: %f\n", average);

            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

int main() {
    stdio_init_all();

    queueData = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);

    xTaskCreate(taskFeedData, "FeedData", 4096, NULL, 1, NULL);
    xTaskCreate(taskProcessData, "ProcessData", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true) {
    }
}
