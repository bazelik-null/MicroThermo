//
// Created by niko on 18.10.2025.
//

#include <Arduino.h>
#include <DallasTemperature.h>
#include <temperatureSensor.h>

Config config;

void TemperatureSensor::setup()
{
	// Initialize variables
	temperatureInside = 0;
	temperatureOutside = 0;

	// Update variables
	update({}, {});
}


void TemperatureSensor::checkTemperature() const
{
	// Activate conditions
	if (temperatureInside < config.MIN_TEMP_INSIDE || temperatureOutside < config.MIN_TEMP_OUTSIDE) {
		// Activate relay
		digitalWrite(config.RELAY_PIN, HIGH);
		digitalWrite(config.RELAY_LED_PIN, HIGH);
	}

	// Deactivate conditions
	else if (temperatureInside > config.MAX_TEMP_INSIDE && temperatureOutside > config.MAX_TEMP_OUTSIDE) {
		// Deactivate relay
		digitalWrite(config.RELAY_PIN, LOW);
		digitalWrite(config.RELAY_LED_PIN, LOW);
	}
}

void TemperatureSensor::raiseError()
{
	// Deactivate relay
	digitalWrite(config.RELAY_PIN, LOW);
	digitalWrite(config.RELAY_LED_PIN, LOW);

	// Activate error LED
	digitalWrite(config.ERROR_LED_PIN, HIGH);

	if (config.IS_DEBUG)
	{
		Serial.println("[ERROR]: Temperature sensor malfunction detected. Logs:");
		Serial.print("[DEBG]: Current temperature inside: ");
		Serial.println(temperatureInside);
		Serial.print("[DEBG]: Current temperature outside: ");
		Serial.println(temperatureOutside);
	}

	// Activate error flag
	errorStateActive = true;
}

void TemperatureSensor::update(DallasTemperature sensorsInside, DallasTemperature sensorsOutside)
{
	sensorsInside.requestTemperatures();
	sensorsOutside.requestTemperatures();

	temperatureInside = sensorsInside.getTempCByIndex(0);
	temperatureOutside = sensorsOutside.getTempCByIndex(0);

	// If temperature is under or above 50C something is certainly wrong
	if (temperatureInside < -50 || temperatureOutside < -50 || temperatureInside > 50 || temperatureOutside > 50)
	{
		raiseError();
	}
}