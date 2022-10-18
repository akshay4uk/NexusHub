#ifndef MYDHT_H
#define MYDHT_H

//GSM module
#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(36, 37);
#include <String.h>

//TFT LCD device
#include "Adafruit_GFX.h"// Hardware-specific library
#include <TouchScreen.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

#define TS_MINX 122
#define TS_MINY 111
#define TS_MAXX 942
#define TS_MAXY 890

#define YP A3
#define XM A2
#define YM 9
#define XP 8
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
//TFT pins designation
#define LCD_RD  A0
#define LCD_WR  A1
#define LCD_CD  A2
#define LCD_CS  A3
#define LCD_RESET A4
// Assign human-readable names to some common 16-bit color values:
#define BLACK  0x0000
#define BLUE  0x001F
#define RED   0xF800
#define GREEN  0x07E0
#define CYAN  0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE  0xFFFF
// End of TFT statement

// DS1307 RTC && SD card
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
RTC_DS1307 rtc;
File data_DHT;
int k = 1;

// DS18B20: water temperature
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 48
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float w_temp;
int Air_p=10;

// PH && EC
#include "DFRobot_EC.h"
#include "DFRobot_PH.h"
#include <EEPROM.h>
#define EC_PIN A14
#define PH_PIN A13
DFRobot_EC ec;
DFRobot_PH ph;
float ecVoltage, phVoltage, phValue, ecValue, temperature, ph_acid = 0,
ph_base = 0, ec_wat = 0, ec_nut = 0, ecdata, phdata, ec_mean_value , ph_mean_value;

float pH_max, pH_min, EC_max, WT_max, WT_min, h_max, h_min, t_max, t_min, EC_min;
unsigned long timepoint = 0, currentTime = 0, previousTime = 0, cycle = 120000;
int cst = 2000;

// DHT21: Air temperature and humidity
#include <DHT.h>
#define dht_pin 49
#define DHTtype DHT22
DHT dht(dht_pin, DHTtype);
float h, t;

// Isolation pins
#define ec_isol 38
#define ec_isol_On digitalWrite(ec_isol, HIGH)
#define ec_isol_Off digitalWrite(ec_isol,LOW)

#define ec_gnd 39
#define ec_gnd_On digitalWrite(ec_gnd,HIGH)
#define ec_gnd_Off digitalWrite(ec_gnd,LOW)

#define sensor 23
#define motion digitalRead(sensor)==HIGH

// ACTUATORS

// Peristaltic pumps and the mixer
#define acid_p 25
#define acidOn digitalWrite(acid_p,LOW)
#define acidOff digitalWrite(acid_p,HIGH)

#define base_p 26
#define baseOn digitalWrite(base_p,LOW)
#define baseOff digitalWrite(base_p,HIGH)

#define wat_p 27
#define watOn digitalWrite(wat_p,LOW)
#define watOff digitalWrite(wat_p,HIGH)

#define nut_p 28
#define nutOn digitalWrite(nut_p,LOW)
#define nutOff digitalWrite(nut_p,HIGH)

#define mixer 29
#define mixerOn digitalWrite(mixer,LOW)
#define mixerOff digitalWrite(mixer,HIGH)


// MQ Gas Sensors
#define redLed 12
#define greenLed 11
#define buzzer 10
#define smokeA0 A5
int sensorThres = 400;
#define normal digitalWrite(redLed,LOW)
#define normal_1 digitalWrite(greenLed,HIGH)
#define normal_buzzer digitalWrite(buzzer,LOW)
#define danger digitalWrite(redLed,HIGH)
#define danger_1 digitalWrite(greenLed,LOW)
#define danger_buzzer digitalWrite(buzzer,HIGH)

#endif
