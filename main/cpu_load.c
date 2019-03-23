/* Hello World Example adapted to CPU_LOAD

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

static int idle_cnt = 0;

static void    idle_task(void *parm)

{
    while(1==1)
        {
        int64_t now = esp_timer_get_time();     // time anchor
        vTaskDelay(0 / portTICK_RATE_MS);
        int64_t now2 = esp_timer_get_time();
        idle_cnt += (now2 - now) / 1000;        // diff
        }
}

static void    mon_task(void *parm)

{
    while(1==1)
        {
        // Note the trick of saving it on entry, so print time
        // is not added to our timing.
        
        float new_cnt =  (float)idle_cnt;    // Save the count for printing it ...
        idle_cnt = 0;                        // Reset variable
        
        // Compensate for the 100 ms delay artifact: 900 ms = 100%
        float cpu_percent = ((99.9 / 90.) * new_cnt) / 10;
        printf("%.0f%%  ", 100 - cpu_percent); fflush(stdout);
        vTaskDelay(1000 / portTICK_RATE_MS);
        }
}

void app_main()
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("ESP32, %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    xTaskCreate(idle_task, "idle_task", 1024 * 2, NULL,  0, NULL);
    xTaskCreate(mon_task, "mon_task", 1024 * 2, NULL, 10, NULL);
    
    printf("CPU Load Demo.\n");
    int cnt = 0;
    while(true)
        {
        // Every 10 seconds, we put the processor to work. 
        if(++cnt % 10 == 0)
            {
            int cnt_dummy = 0;
            
            printf(" work [[ "); fflush(stdout);
            
            //Make sure the watchdog is not triggered ...
            for(int aa = 0 ; aa < 30000000; aa++)
                {
                cnt_dummy += 22;
                }
                
            printf(" ]] rest "); fflush(stdout);
            }
        vTaskDelay(1000 / portTICK_RATE_MS);
        }
}



