//
// Created by niko on 18.10.2025.
//

#include "thermostat.h"

#include "drivers/onewire.h"

#include <string.h>

#include <esp_system.h>
#include <esp_task.h>

#include <driver/gpio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <uart_helper.h>

thermostat_instance_t* temperature_sensor = NULL;

void debug() {
  char buffer[100];  // Buffer for formatted strings

  uart_write("----------");
  uart_write("[INFO]: Temperature report:");
  uart_write("");

  snprintf(buffer, sizeof(buffer), "[DEBG]: Current temperature inside: %.2f", temperature_sensor->temperature_inside);
  uart_write(buffer);
  memset(buffer, 0, sizeof(buffer));

  snprintf(buffer, sizeof(buffer), "[DEBG]: Current temperature outside: %.2f", temperature_sensor->temperature_outside);
  uart_write(buffer);
  memset(buffer, 0, sizeof(buffer));

  uart_write("");

  snprintf(buffer, sizeof(buffer), "[DEBG]: Min temperature inside: %.2f", temperature_sensor->config->MIN_TEMP_INSIDE);
  uart_write(buffer);
  memset(buffer, 0, sizeof(buffer));

  snprintf(buffer, sizeof(buffer), "[DEBG]: Min temperature outside: %.2f", temperature_sensor->config->MIN_TEMP_OUTSIDE);
  uart_write(buffer);
  memset(buffer, 0, sizeof(buffer));

  uart_write("");

  snprintf(buffer, sizeof(buffer), "[DEBG]: Max temperature inside: %.2f", temperature_sensor->config->MAX_TEMP_INSIDE);
  uart_write(buffer);
  memset(buffer, 0, sizeof(buffer));

  snprintf(buffer, sizeof(buffer), "[DEBG]: Max temperature outside: %.2f", temperature_sensor->config->MAX_TEMP_OUTSIDE);
  uart_write(buffer);
  memset(buffer, 0, sizeof(buffer));

  uart_write("----------");
}

// Loop
void app_loop() {
  if ( temperature_sensor->is_error_state_active ) { return; } // If error is present stop code execution

  thermostat_update(temperature_sensor);
  thermostat_check_temperature(temperature_sensor);

  if (temperature_sensor->config->IS_DEBUG)
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
  // Init config
  thermostat_config_t thermostat_config;
  thermostat_config_init(&thermostat_config);

  if (thermostat_config.IS_DEBUG) {
    uart_init();
  }

  gpio_set_direction(thermostat_config.RELAY_PIN, GPIO_MODE_OUTPUT);
  gpio_set_direction(thermostat_config.RELAY_LED_PIN, GPIO_MODE_OUTPUT);
  gpio_set_direction(thermostat_config.ERROR_LED_PIN, GPIO_MODE_OUTPUT);

  OWInit();

  thermostat_setup(temperature_sensor, &thermostat_config);

  if (thermostat_config.IS_DEBUG) {
    uart_write("[INFO]: Set up complete");
  }

  while (true)
  {
    app_loop();
  }
}