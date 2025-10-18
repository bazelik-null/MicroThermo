//
// Created by niko on 18.10.2025.
//

#include <Arduino.h>
#include <temperatureSensor.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS_INSIDE 12
#define ONE_WIRE_BUS_OUTSIDE 11

OneWire oneWireInside(ONE_WIRE_BUS_INSIDE);
OneWire oneWireOutside(ONE_WIRE_BUS_OUTSIDE);

DallasTemperature sensorsInside(&oneWireInside);
DallasTemperature sensorsOutside(&oneWireOutside);

TemperatureSensor temperatureSensor;

void debug() {
  Serial.println("----------");
  Serial.println("[INFO]: Temperature report:");

  Serial.println("");

  Serial.print("[DEBG]: Current temperature inside: ");
  Serial.println(temperatureSensor.temperatureInside);
  Serial.print("[DEBG]: Current temperature outside: ");
  Serial.println(temperatureSensor.temperatureOutside);

  Serial.println("");

  Serial.print("[DEBG]: Min temperature inside: ");
  Serial.println(config.MIN_TEMP_INSIDE);
  Serial.print("[DEBG]: Min temperature outside: ");
  Serial.println(config.MIN_TEMP_OUTSIDE);

  Serial.println("");

  Serial.print("[DEBG]: Max temperature inside: ");
  Serial.println(config.MAX_TEMP_INSIDE);
  Serial.print("[DEBG]: Max temperature outside: ");
  Serial.println(config.MAX_TEMP_OUTSIDE);
  Serial.println("----------");
}

void setup() {
  if (config.IS_DEBUG) {
    Serial.begin(9600);
  }

  pinMode(config.RELAY_PIN,     OUTPUT);
  pinMode(config.RELAY_LED_PIN, OUTPUT);
  pinMode(config.ERROR_LED_PIN, OUTPUT);

  sensorsInside.begin();
  sensorsOutside.begin();

  temperatureSensor.setup();

  if (config.IS_DEBUG) {
    Serial.println("[INFO]: Set up complete");
  }
}

void loop() {
  if ( temperatureSensor.errorStateActive ) { return; } // If error is present stop code execution

  temperatureSensor.update({}, {});
  temperatureSensor.checkTemperature();

  if (config.IS_DEBUG)
  {
    debug();
    delay(1000);  // 1s
  }
  else
  {
    delay(60000); // 60s
  }
}