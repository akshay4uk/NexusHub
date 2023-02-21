#define SOLAR_VOLTAGE_PIN A0
#define BATTERY_VOLTAGE_PIN A1
#define LOAD_VOLTAGE_PIN A2
#define BATTERY_SOC_PIN A3
#define GENERATOR_RUNNING_PIN 2
#define RELAY_PIN 13

#define MAX_BATTERY_SOC 100
#define MIN_BATTERY_SOC 50


void setup() {
  pinMode(GENERATOR_RUNNING_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  int solarVoltage = analogRead(SOLAR_VOLTAGE_PIN);
  int batteryVoltage = analogRead(BATTERY_VOLTAGE_PIN);
  int loadVoltage = analogRead(LOAD_VOLTAGE_PIN);
  int batterySOC = analogRead(BATTERY_SOC_PIN);
  int generatorRunning = digitalRead(GENERATOR_RUNNING_PIN);

  if (solarVoltage > batteryVoltage + loadVoltage && batterySOC < MAX_BATTERY_SOC && !generatorRunning) {
    // If solar voltage is higher than the sum of battery voltage and load voltage and battery SOC is less than MAX_BATTERY_SOC and generator is not running, turn on relay to charge battery and/or divert excess electricity to load
    digitalWrite(RELAY_PIN, HIGH);
  } else if (batterySOC > MIN_BATTERY_SOC || generatorRunning) {
    // If battery SOC is higher than MIN_BATTERY_SOC or generator is running, turn off relay
    digitalWrite(RELAY_PIN, LOW);
  }
}
#include <Adafruit_INA219.h>

// Initialize the INA219 sensor
Adafruit_INA219 ina219;

void setup() {
  Serial.begin(9600);
  ina219.begin();
}

void loop() {
  float shuntVoltage = 0;
  float busVoltage = 0;
  float current_mA = 0;
  float loadVoltage = 0;

  // Read the values from the INA219 sensor
  shuntVoltage = ina219.getShuntVoltage_mV();
  busVoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadVoltage = busVoltage + (shuntVoltage / 1000);

  // Print the values to the serial monitor
  Serial.print("Bus Voltage: ");
  Serial.print(busVoltage);
  Serial.println(" V");
  Serial.print("Shunt Voltage: ");
  Serial.print(shuntVoltage);
  Serial.println(" mV");
  Serial.print("Load Voltage: ");
  Serial.print(loadVoltage);
  Serial.println(" V");
  Serial.print("Current: ");
  Serial.print(current_mA);
  Serial.println(" mA");

  // Wait 1 second before repeating
  delay(1000);
}
