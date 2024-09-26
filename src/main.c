/********************************************************************************************* */
//    Eduboard2 ESP32-S3 Template with BSP
//    Author: Martin Burger
//    Juventus Technikerschule
//    Version: 1.0.0
//    
//    This is the ideal starting point for a new Project. BSP for most of the Eduboard2
//    Hardware is included under components/eduboard2.
//    Hardware support can be activated/deactivated in components/eduboard2/eduboard2_config.h
/********************************************************************************************* */
#include "eduboard2.h"
#include "memon.h"

#include "math.h"

#define TAG "TEST_EVENTGROUP_HS2024"

#define TAG_TASK1   "TASK1"
#define TAG_TASK2   "TASK2"
#define TAG_TASK3   "TASK3"

#define UPDATETIME_MS 100

#define BIT_TEST1       1 << 0
#define BIT_TEST2       1 << 1
#define BIT_TEST3       1 << 2
#define BIT_TEST4       1 << 3
#define BIT_TEST5       1 << 4
#define BIT_TEST6       1 << 5
#define BIT_TEST7       1 << 6
#define BIT_TEST8       1 << 7
#define ALL_BITS        0xFF
EventGroupHandle_t testeventgroup = NULL;

void testtask1(void* param) {
    ESP_LOGI(TAG_TASK1, "Task1 Starting");
    testeventgroup = xEventGroupCreate();
    for(;;) {
        if(xEventGroupGetBits(testeventgroup) & BIT_TEST1) {
            ESP_LOGI(TAG_TASK1, "Received BIT_TEST1");
            xEventGroupClearBits(testeventgroup, BIT_TEST1);
        }
        EventBits_t myBits = xEventGroupGetBits(testeventgroup);
        if(myBits & BIT_TEST2) {
            ESP_LOGI(TAG_TASK1, "Received BIT_TEST2, happens only accidental!");
        }
        if(myBits & BIT_TEST3) {
            ESP_LOGI(TAG_TASK1, "Received BIT_TEST3, happens only accidental!");
        }
        // ESP_LOGI(TAG_TASK1, "Task1 Running");
        vTaskDelay(300/portTICK_PERIOD_MS);
    }
}

void testtask2(void* param) {
    ESP_LOGI(TAG_TASK2, "Task2 Starting");
    while(testeventgroup == NULL) {
        vTaskDelay(1);
    }
    for(;;) {
        xEventGroupWaitBits(testeventgroup, BIT_TEST2 | BIT_TEST3, true, false, portMAX_DELAY);
        ESP_LOGI(TAG_TASK2, "Task2 Running");
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
}

void testtask3(void* param) {
    ESP_LOGI(TAG_TASK3, "Task3 Starting");
    while(testeventgroup == NULL) {
        vTaskDelay(1);
    }
    for(;;) {
        if(button_get_state(SW0, true) == SHORT_PRESSED) {
            xEventGroupSetBits(testeventgroup, BIT_TEST1);
        }
        if(button_get_state(SW1, true) == SHORT_PRESSED) {
            xEventGroupSetBits(testeventgroup, BIT_TEST2);
        }
        if(button_get_state(SW2, true) == SHORT_PRESSED) {
            xEventGroupSetBits(testeventgroup, BIT_TEST3);
        }
        if(button_get_state(SW3, true) == SHORT_PRESSED) {
            xEventGroupClearBits(testeventgroup, ALL_BITS);
        }
        // ESP_LOGI(TAG_TASK3, "Task3 Running");
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}



void app_main()
{
    //Initialize Eduboard2 BSP
    eduboard2_init();

    
    
    xTaskCreate(testtask1, "TestTask1", 2048, NULL, 10, NULL);
    xTaskCreate(testtask2, "TestTask1", 2048, NULL, 10, NULL);
    xTaskCreate(testtask3, "TestTask1", 2048, NULL, 10, NULL);
    for(;;) {
        vTaskDelay(2000/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "Hello Eduboard");
    }
}