#include <Time.h>
#include <TimeLib.h>

// Pin definition for the shut-off valve
#define SHUT_OFF_VALVE_PIN 12

// Start and end times for the shut-off valve to be turned off
const int startHour = 22;
const int endHour = 6;

void setup() {
  // Set the pin mode for the shut-off valve
  pinMode(SHUT_OFF_VALVE_PIN, OUTPUT);

  // Initialize the serial communication
  Serial.begin(9600);
}

void loop() {
  // Get the current time
  time_t now = time(nullptr);
  tm *ltm = localtime(&now);

  // Check if it's within the start and end hours
  if (ltm->tm_hour >= startHour || ltm->tm_hour < endHour) {
    // Turn off the shut-off valve
    digitalWrite(SHUT_OFF_VALVE_PIN, LOW);
    Serial.println("Shut-off valve turned off.");
  } else {
    // Turn on the shut-off valve
    digitalWrite(SHUT_OFF_VALVE_PIN, HIGH);
    Serial.println("Shut-off valve turned on.");
  }

  // Delay for one minute
  delay(60000);
}
