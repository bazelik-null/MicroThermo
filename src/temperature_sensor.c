//
// Created by niko on 18.10.2025.
//

#include <string.h>
#include <temperature_sensor.h>

#include "uart_helper.h"
#include "drivers/ds18b20.h"

Sensor_Config sensor_config;

void Temperature_Sensor_setup(Temperature_Sensor* sensor)
{
	// Init config
	Sensor_Config_init(&sensor_config);

	// Init variables
	sensor->is_error_state_active = false;
	sensor->temperature_inside = 0;
	sensor->temperature_outside = 0;

	memset(sensor->sensor0ROM, 0, sizeof(sensor->sensor0ROM)); // Initialize all elements to 0
	memset(sensor->sensor1ROM, 0, sizeof(sensor->sensor1ROM)); // Initialize all elements to 0

	OWTargetSetup(0x28); // Scan for devices with 0x28 as family code (first byte)

	// Iterate and register each device
	int device = 0;
	int device_count = 0;
	do
	{
		device = OWNext();
		if (device)
		{
			if (device_count == 0) // remember the first sensor
				memcpy(sensor->sensor0ROM, ROM_NO, 8);
			else if (device_count == 1) // remember the second sensor
				memcpy(sensor->sensor1ROM, ROM_NO, 8);

			device_count++;
		}
	} while (device);

	// Configure DS18B20 sensors to convert using 10 bit resolution
	if (DSconfigure(sensor->sensor0ROM) != DS_OK)
	{
		Temperature_Sensor_raise_error(sensor, "[ERROR]: Temperature sensor configuration failed.");
	}
	if (DSconfigure(sensor->sensor1ROM) != DS_OK)
	{
		Temperature_Sensor_raise_error(sensor, "[ERROR]: Temperature sensor configuration failed.");
	}
}

void Temperature_Sensor_check_temperature(const Temperature_Sensor* sensor)
{
	// Activate conditions
	if (sensor->temperature_inside < sensor_config.MIN_TEMP_INSIDE || sensor->temperature_outside < sensor_config.MIN_TEMP_OUTSIDE) {
		// Activate relay
		gpio_set_level(sensor_config.RELAY_PIN, GPIO_INTR_HIGH_LEVEL);
		gpio_set_level(sensor_config.RELAY_LED_PIN, GPIO_INTR_HIGH_LEVEL);
	}

	// Deactivate conditions
	else if (sensor->temperature_inside > sensor_config.MAX_TEMP_INSIDE && sensor->temperature_outside > sensor_config.MAX_TEMP_OUTSIDE) {
		// Deactivate relay
		gpio_set_level(sensor_config.RELAY_PIN, GPIO_INTR_LOW_LEVEL);
		gpio_set_level(sensor_config.RELAY_LED_PIN, GPIO_INTR_LOW_LEVEL);
	}
}

void Temperature_Sensor_raise_error(Temperature_Sensor* sensor, const char* error)
{
	// Deactivate relay
	gpio_set_level(sensor_config.RELAY_PIN, GPIO_INTR_LOW_LEVEL);
	gpio_set_level(sensor_config.RELAY_LED_PIN, GPIO_INTR_LOW_LEVEL);

	// Activate error LED
	gpio_set_level(sensor_config.ERROR_LED_PIN, GPIO_INTR_HIGH_LEVEL);

	if (sensor_config.IS_DEBUG)
	{
		uart_write(error);
	}

	// Activate error flag
	sensor->is_error_state_active = true;
}

void Temperature_Sensor_update(Temperature_Sensor* sensor)
{
	// Read temperature from the first sensor
	if (DSreadTemperature(sensor->sensor0ROM, &sensor->temperature_inside) != DS_OK)
	{
		Temperature_Sensor_raise_error(sensor, "[ERROR]: Temperature cannot be read.");
	}

	// Read temperature from the second sensor
	if (DSreadTemperature(sensor->sensor1ROM, &sensor->temperature_outside) != DS_OK)
	{
		Temperature_Sensor_raise_error(sensor, "[ERROR]: Temperature cannot be read.");
	}

	// If temperature is under or above 50C something is certainly wrong
	if (sensor->temperature_inside < -50 || sensor->temperature_outside < -50 || sensor->temperature_inside > 50 || sensor->temperature_outside > 50)
	{
		Temperature_Sensor_raise_error(sensor, "[ERROR]: Temperature sensor malfunction detected.");
	}
}