#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "core2forAWS.h"
#include "sk6812_test.h"

static xSemaphoreHandle lock;
static uint8_t stop_show = true;

void sk6812ShowTask(void *arg) {
    lock = xSemaphoreCreateMutex();
    uint8_t stop_show_stash;
    while (1) {
        Core2ForAWS_Sk6812_Clear();
        Core2ForAWS_Sk6812_Show();

        while (1) {
            xSemaphoreTake(lock, portMAX_DELAY);
            stop_show_stash = stop_show;
            xSemaphoreGive(lock);
            if (stop_show_stash == false) {
                break;
            }
            vTaskDelay(200);
        }

        for (uint8_t i = 0; i < 10; i++) {
            Core2ForAWS_Sk6812_SetColor(i, 0xff9900);
            Core2ForAWS_Sk6812_Show();
            vTaskDelay(100);
        }

        for (uint8_t i = 0; i < 10; i++) {
            Core2ForAWS_Sk6812_SetColor(i, 0x000000);
            Core2ForAWS_Sk6812_Show();
            vTaskDelay(100);
        }

        Core2ForAWS_Sk6812_SetSideColor(SK6812_SIDE_LEFT, 0x232f3e);
        Core2ForAWS_Sk6812_SetSideColor(SK6812_SIDE_RIGHT, 0xffffff);
        Core2ForAWS_Sk6812_Show();

        for (uint8_t i = 40; i > 0; i--) {
            Core2ForAWS_Sk6812_SetBrightness(i);
            Core2ForAWS_Sk6812_Show();
            vTaskDelay(25);
        }

        Core2ForAWS_Sk6812_SetBrightness(20);
    }
    vTaskDelete(NULL);
}

void sk6812TaskSuspend() {
    xSemaphoreTake(lock, portMAX_DELAY);
    stop_show = true;
    xSemaphoreGive(lock);
}

void sk6812TaskResume() {
    xSemaphoreTake(lock, portMAX_DELAY);
    stop_show = false;    
    xSemaphoreGive(lock);
}
