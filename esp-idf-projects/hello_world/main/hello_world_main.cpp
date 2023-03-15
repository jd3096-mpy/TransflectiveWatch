/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/spi_master.h"

#include <pcf8563.h>
#include <spi_bus.h>
#include <esp32-hal-spi.h>

#include "Adafruit_SharpMem.h"

// #define PARALLEL_LINES 16

// #define LCD_HOST SPI2_HOST

#define SHARP_SCK 6
#define SHARP_MOSI 7
#define SHARP_SS 8
#define SHARP_DISP 10

// Set the size and color depth, e.g. 3 bits for LS013B7DH06 (8 colors 128x128 display)
Adafruit_SharpMem display(&SPI, SHARP_SS, 128, 128, 1, 2000000); // 2100000 ok, 2500000 wrong, 230000 LDO ok

#define BLACK 0
#define WHITE 1

// #define PIN_NUM_MISO ((gpio_num_t)-1)
// #define PIN_NUM_MOSI ((gpio_num_t)7)
// #define PIN_NUM_CLK ((gpio_num_t)6)
// #define PIN_NUM_CS ((gpio_num_t)8)

// #define PIN_NUM_DISP ((gpio_num_t)10)

// uint8_t _sharpmem_vcom;

// #define SHARPMEM_BIT_WRITECMD (0x01) // 0x80 in LSB format
// #define SHARPMEM_BIT_VCOM (0x02)     // 0x40 in LSB format
// #define SHARPMEM_BIT_CLEAR (0x04)    // 0x20 in LSB format


// #define _color_depth_bits 1

// Initialize the display
void lcd_init()
{
}

void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    // int dc=(int)t->user;
    // gpio_set_level(PIN_NUM_DC, dc);
}

void lcd_spi_post_transfer_callback(spi_transaction_t *t)
{
    // int dc=(int)t->user;
    // gpio_set_level(PIN_NUM_DC, dc);
}

void sharp_memory_display()
{

    // Initialize the LCD
    lcd_init();
}

extern "C" void app_main(void)
{
    printf("Hello world!\n");

    // pinMode(SHARP_DISP, OUTPUT);
    // digitalWrite(SHARP_DISP, HIGH);
    SPI.begin(SHARP_SCK, 11, SHARP_MOSI, SHARP_SS);

    // start & clear the display
    display.begin();
    display.clearDisplay();

    delay(500);
    display.refresh();

    printf("DONE!!!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("It is %s chip with %d CPU core(s), WiFi%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    // sharp_memory_display();

    for (int i = 10; i >= 0; i--)
    {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
