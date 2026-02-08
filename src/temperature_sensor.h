//
// Created by niko on 18.10.2025.
//

#ifndef THERMOSTAT_TEMPERATURESENSOR_H
#define THERMOSTAT_TEMPERATURESENSOR_H

#include <soc/gpio_num.h>
#include "drivers/onewire.h"

typedef struct Temperature_Sensor
{
	bool is_error_state_active;
	float temperature_inside;
	float temperature_outside;
	uint8_t sensor0ROM[8];
	uint8_t sensor1ROM[8];
} Temperature_Sensor;

void Temperature_Sensor_setup(Temperature_Sensor* sensor);
void Temperature_Sensor_update(Temperature_Sensor* sensor);
void Temperature_Sensor_check_temperature(const Temperature_Sensor* sensor);
void Temperature_Sensor_raise_error(Temperature_Sensor* sensor, const char* error);

typedef struct Sensor_Config
{
	float MIN_TEMP_INSIDE;
	float MIN_TEMP_OUTSIDE;
	float MAX_TEMP_INSIDE;
	float MAX_TEMP_OUTSIDE;
	gpio_num_t RELAY_PIN;
	gpio_num_t RELAY_LED_PIN;
	gpio_num_t ERROR_LED_PIN;
	bool IS_DEBUG;
} Sensor_Config;

extern Sensor_Config sensor_config;

inline void Sensor_Config_init(Sensor_Config* config) {
	config->MIN_TEMP_INSIDE = 20.0f;
	config->MIN_TEMP_OUTSIDE = -10.0f;
	config->MAX_TEMP_INSIDE = 25.0f;
	config->MAX_TEMP_OUTSIDE = 2.0f;
	config->RELAY_PIN = GPIO_NUM_2;
	config->RELAY_LED_PIN = GPIO_NUM_3;
	config->ERROR_LED_PIN = GPIO_NUM_4;
	config->IS_DEBUG = true;
}

#endif //THERMOSTAT_TEMPERATURESENSOR_H
