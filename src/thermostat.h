//
// Created by niko on 18.10.2025.
//

#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include <soc/gpio_num.h>

#include "drivers/onewire.h"

typedef struct thermostat_config_t
{
	float MIN_TEMP_INSIDE;
	float MIN_TEMP_OUTSIDE;

	float MAX_TEMP_INSIDE;
	float MAX_TEMP_OUTSIDE;

	gpio_num_t RELAY_PIN;
	gpio_num_t RELAY_LED_PIN;
	gpio_num_t ERROR_LED_PIN;

	bool IS_DEBUG;
} thermostat_config_t;

inline void thermostat_config_init(thermostat_config_t* config) {
	config->MIN_TEMP_INSIDE = 20.0f;
	config->MIN_TEMP_OUTSIDE = -10.0f;

	config->MAX_TEMP_INSIDE = 25.0f;
	config->MAX_TEMP_OUTSIDE = 2.0f;

	config->RELAY_PIN = GPIO_NUM_2;
	config->RELAY_LED_PIN = GPIO_NUM_3;
	config->ERROR_LED_PIN = GPIO_NUM_4;

	config->IS_DEBUG = true;
}

typedef struct thermostat_instance_t
{
	thermostat_config_t* config;
	bool is_error_state_active;

	float temperature_inside;
	float temperature_outside;

	uint8_t sensor0ROM[8];
	uint8_t sensor1ROM[8];
} thermostat_instance_t;

void thermostat_setup(thermostat_instance_t* sensor, thermostat_config_t* config);
void thermostat_update(thermostat_instance_t* sensor);
void thermostat_check_temperature(const thermostat_instance_t* sensor);
void thermostat_raise_error(thermostat_instance_t* sensor, const char* error);

#endif //THERMOSTAT_H
