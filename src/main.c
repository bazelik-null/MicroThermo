//
// Created by niko on 18.10.2025.
//

#include "temperature_sensor.h"

#include "drivers/onewire.h"

#include <string.h>

#include <esp_system.h>
#include <esp_task.h>

#include <driver/gpio.h>
#include <driver/uart.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <uart_helper.h>

Temperature_Sensor temperature_sensor;

void debug() {
  char buffer[50];  // Buffer for formatted strings

  uart_write("----------");
  uart_write("[INFO]: Temperature report:");
  uart_write("");

  snprintf(buffer, sizeof(buffer), "[DEBG]: Current temperature inside: %.2f", temperature_sensor.temperature_inside);
  uart_write(buffer);

  snprintf(buffer, sizeof(buffer), "[DEBG]: Current temperature outside: %.2f", temperature_sensor.temperature_outside);
  uart_write(buffer);

  uart_write("");

  snprintf(buffer, sizeof(buffer), "[DEBG]: Min temperature inside: %.2f", sensor_config.MIN_TEMP_INSIDE);
  uart_write(buffer);

  snprintf(buffer, sizeof(buffer), "[DEBG]: Min temperature outside: %.2f", sensor_config.MIN_TEMP_OUTSIDE);
  uart_write(buffer);

  uart_write("");

  snprintf(buffer, sizeof(buffer), "[DEBG]: Max temperature inside: %.2f", sensor_config.MAX_TEMP_INSIDE);
  uart_write(buffer);

  snprintf(buffer, sizeof(buffer), "[DEBG]: Max temperature outside: %.2f", sensor_config.MAX_TEMP_OUTSIDE);
  uart_write(buffer);

  uart_write("----------");
}

// Loop
void app_loop() {
  if ( temperature_sensor.is_error_state_active ) { return; } // If error is present stop code execution

  Temperature_Sensor_update(&temperature_sensor);
  Temperature_Sensor_check_temperature(&temperature_sensor);

  if (sensor_config.IS_DEBUG)
  {
    debug();
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // 1s
  }
  else
  {
    vTaskDelay(60000 / portTICK_PERIOD_MS); // 60s
  }
}

// Entry point
[[noreturn]]
void app_main() {
  if (sensor_config.IS_DEBUG) {
    uart_init();
  }

  gpio_set_direction(sensor_config.RELAY_PIN, GPIO_MODE_OUTPUT);
  gpio_set_direction(sensor_config.RELAY_LED_PIN, GPIO_MODE_OUTPUT);
  gpio_set_direction(sensor_config.ERROR_LED_PIN, GPIO_MODE_OUTPUT);

  OWInit();

  Temperature_Sensor_setup(&temperature_sensor);

  if (sensor_config.IS_DEBUG) {
    const char* msg = "[INFO]: Set up complete";
    uart_write_bytes(UART_NUM, msg, strlen(msg));
  }

  while (true)
  {
    app_loop();
  }
}