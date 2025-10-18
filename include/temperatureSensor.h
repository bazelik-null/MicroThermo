//
// Created by niko on 18.10.2025.
//

#ifndef THERMOSTAT_TEMPERATURESENSOR_H
#define THERMOSTAT_TEMPERATURESENSOR_H
#include "DallasTemperature.h"

class TemperatureSensor
{
	public:
		void setup();

		void update(DallasTemperature sensorsInside, DallasTemperature sensorsOutside);
		void checkTemperature() const;

		void raiseError();
		bool errorStateActive;

		float temperatureInside;
		float temperatureOutside;
};

struct Config
{
	const float MIN_TEMP_INSIDE  = 20;  // Temperature to activate relay inside
	const float MIN_TEMP_OUTSIDE = -10; // Temperature to activate relay outside

	const float MAX_TEMP_INSIDE  = 25;  // Temperature to deactivate relay inside
	const float MAX_TEMP_OUTSIDE = 2;   // Temperature to deactivate relay outside

	const int   RELAY_PIN		 = 2;
	const int   RELAY_LED_PIN    = 3;
	const int   ERROR_LED_PIN    = 4;

	const bool  IS_DEBUG         = true;
};

extern Config config;

#endif //THERMOSTAT_TEMPERATURESENSOR_H