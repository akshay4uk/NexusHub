#include "MyLib.h"
#include "Preference.h"

void MyLib_::begin(){
  gprsSerial.begin(9600);
  Serial.begin(9600);
  sensors.begin();
  ph.begin();
  ec.begin();
  dht.begin();
  rtc.begin();
  SD.begin(53);
  

  // SD card verification
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  while (!SD.begin(53)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    // while (1);
  }
  Serial.println("card initialized.");

  // Creation of a csv file
  data_DHT = SD.open("Data_DHT.csv", FILE_WRITE);
  data_DHT.println("N, Date, Time, Humidity, Temperature, Water temp, pH, EC, ec_watTime, ec_nutTime, ph_acidTime, ph_baseTime");
  data_DHT.close();


  // TFT LCD screen initialization
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(3); // This settings works for me with correct orientation
  tft.setFont(&FreeSerif9pt7b);
  tft.fillScreen(RED);
  tft.setCursor(50, 40);
  tft.setTextSize(3);
  tft.println("Welcome \n     to your  \n   hydroponic \n       farm");
  delay(2000);

  tft.fillScreen(WHITE);
  tft.setCursor(5, 25);
  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.print("Please select a plant:");

  tft.fillRect(50, 50, 210, 40, RED);
  tft.drawRect(50, 50, 210, 40, BLACK);
  tft.setCursor(80, 75);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("COLLARD GREEN");

  tft.fillRect(50, 130, 210, 40, RED);
  tft.drawRect(50, 130, 210, 40, BLACK);
  tft.setCursor(110, 155);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("CORIANDER");

  tft.setCursor(0, 200);
  tft.setTextColor(RED);
  tft.setTextSize(1);
  tft.print("You have 15 seconds to select. COLLARD GREEN will be selected by default.");

  float value = millis();
  while (millis() - value < 20000U) {
    TSPoint p = ts.getPoint();
    if (p.z > ts.pressureThreshhold) { // Check touch validity
      p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
      p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
      if (p.x > 50 && p.x < 260) {
        if (p.y > 50 && p.y < 90) {
          pinMode(XM, OUTPUT);
          pinMode(YP, OUTPUT);
          tft.fillScreen(WHITE);
          tft.setTextColor(BLACK);
          tft.setCursor(0, 40);
          tft.setTextSize(2);
          tft.print(" COLLARD GREEN");
          tft.setCursor(0, 80);
          tft.setTextSize(1);
          tft.print("20 < Temp < 25      80 < Hum < 85  \n\n     6 < pH < 7        2.5 < EC < 3 \n\n     20 < Water_Temp < 30");
          pH_max = 7, pH_min = 6, EC_min = 2.5, EC_max = 3, WT_min = 20,
          WT_max = 30, h_max = 85, h_min = 80, t_min = 20, t_max = 25;
          break;
        }
        else if (p.y > 130 && p.y < 170) {
          pinMode(XM, OUTPUT);
          pinMode(YP, OUTPUT);
          tft.fillScreen(WHITE);
          tft.setCursor(0, 40);
          tft.setTextColor(BLACK);
          tft.setTextSize(2);
          tft.print(" CORIANDER");
          tft.setCursor(0, 80);
          tft.setTextSize(1);
          tft.print("20 < Temp < 25      80 < Hum < 85 \n\n    6 < pH < 7        2.5 < EC < 3 \n\n     20 < Water_Temp < 30");
          pH_max = 7, pH_min = 6, EC_min = 2.5, EC_max = 3, WT_min = 20,
          WT_max = 30, h_max = 85, h_min = 80, t_min = 20, t_max = 25;
          break;
        }
      }
    }
    else if ( millis() - value > 15000U) {
      tft.begin(0x9341);
      tft.setRotation(3);
      tft.fillScreen(WHITE);
      tft.setCursor(30, 130);
      tft.setTextColor(BLACK);
      tft.setTextSize(3);
      tft.print(" Time is up ");
      pH_max = 7, pH_min = 6, EC_min = 2.5, EC_max = 3, WT_min = 20,
      WT_max = 30, h_max = 85, h_min = 80, t_min = 20, t_max = 25;
      break;
    }
  }

  // Actuators statement
  pinMode(sensor,INPUT_PULLUP);
  pinMode(Air_p, OUTPUT);
  pinMode(mixer, OUTPUT);
  mixerOff;
  pinMode(acid_p, OUTPUT);
  acidOff;
  pinMode(base_p, OUTPUT);
  baseOff;
  pinMode(wat_p, OUTPUT);
  watOff;
  pinMode(nut_p, OUTPUT);
  nutOff;
  pinMode(ec_isol, OUTPUT);
  pinMode(ec_gnd, OUTPUT);
  ec_isol_Off;
  ec_gnd_On;
}

// Sensing function
// Water temperature sensing
float MyLib_::wt_sensor() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

// EC sensing process
float MyLib_::ec_sensor() {
  int i = 0;
  ec_mean_value = 0;
  float ec_calcul = 0;
  while (i != 10) {
    if (millis() - timepoint > 1000U) {
      timepoint = millis();
      temperature = wt_sensor();// read your temperature sensor to execute temperature compensation
      ecVoltage = analogRead(EC_PIN) / 1024.0 * 5000; // read the voltage
      ecValue =  ec.readEC(ecVoltage, temperature); // convert voltage to EC with temperature compensation
      Serial.print("temperature");
      Serial.print(temperature, 1);
      Serial.print(" °C \t EC: ");
      Serial.print(ecValue, 1);
      Serial.println(" ms/cm");
      i++;
      ec_calcul += ecValue;
    }
  }
  ec_isol_On;
  ec_gnd_Off;
  ec_mean_value = ec_calcul / 10;
  Serial.print("EC_Mean Value: ");
  Serial.println (ec_mean_value);
  return ec_mean_value;
}

//pH sensing process
float MyLib_::ph_sensor() {
  int  j = 0;
  ph_mean_value = 0;
  float ph_calcul = 0;
  while (j != 10) {
    if (millis() - timepoint > 1000U) {
      timepoint = millis();
      temperature = wt_sensor();// read your temperature sensor to execute temperature compensation
      phVoltage = analogRead(PH_PIN) / 1024.0 * 5000; // read the voltage
      phValue = ph.readPH(phVoltage, temperature); // convert voltage to pH with temperature compensation
      Serial.print("temperature");
      Serial.print(temperature, 1);
      Serial.print(" °C \t pH: ");
      Serial.println(phValue, 1);
      j++;
      ph_calcul += phValue;
    }
  }
  ec_isol_Off;
  ec_gnd_On;
  ph_mean_value = ph_calcul / 10;
  Serial.print("PH_Mean Value: ");
  Serial.println (ph_mean_value);
  return ph_mean_value;
}

// Adjustment functions

// EC and pH correction
void MyLib_::water_quality() {
   if(motion){
      delay(240000);
      ph_acid = 0, ph_base = 0, ec_wat = 0, ec_nut = 0;
    }
  ecValue =  ec_sensor();
  delay(3000);
  phValue = ph_sensor();
  ecdata = ecValue;
  phdata = phValue;
  while (ecValue < EC_min or ecValue > EC_max or phValue < pH_min or phValue > pH_max) {
    if (ecValue < EC_min or ecValue > EC_max) {
      if (ecValue < EC_min) {
        nutOn;
        delay(cst);// Based on the Pipe size we can arrange the Time conductivity value lesser than the minimum value or Ec value greater than the EC maximum value , We need to turn on the pump through the solenoid valves.
        nutOff;
        ec_nut = ec_nut + cst;
      }
      else { //If In case above the condition is not true need to turn on the water flow 
        watOn;
        delay(cst);// delay time based on th pump size (need to think about size of the pump time will vary.)
        watOff;
        ec_wat = ec_wat + cst;
      }
    }
    else {
      if (phValue < pH_min) {
        baseOn; //In case PH value greater than PH minimum value need to turn on the base.
        delay(cst);
        baseOff; // After some time need to turn off the Base.
        ph_base = ph_base + cst; //Update the value 
      }
      else  { //In case Ph value is greater than the minimum set value of PH turn on the aCID 
        acidOn;
        delay(cst);
        acidOff;
        ph_acid = ph_acid + cst;
      }
    }
    if(motion){
      delay(240000);
      ph_acid = 0, ph_base = 0, ec_wat = 0, ec_nut = 0;
      break;
    }
    mixerOn;
    delay(20000);
    mixerOff;
    ecValue =  ec_sensor(); // convert voltage to EC with temperature compensation
    delay(3000);
    phValue = ph_sensor(); // convert voltage to pH with temperature compensation
  }
  datalog();
}

// Data logging and display
void MyLib_::datalog() {
  currentTime = millis();
  if (currentTime - previousTime >= cycle) {
    previousTime = currentTime;
    // Air temperature and humidity
    h = dht.readHumidity();
    t = dht.readTemperature();

    // Data logging on SD card
    card();
    // Display data on TFT LCD screen
    touch();
    // Display data on serial monitor
    serial();
    // Saving data online
    online();

    ph_acid = 0, ph_base = 0, ec_wat = 0, ec_nut = 0;
    k++;
  }
}

void MyLib_::card() {
  data_DHT = SD.open("Data_DHT.csv", FILE_WRITE);
  DateTime now = rtc.now();
  data_DHT.print(k);
  data_DHT.print(",");
  data_DHT.print(now.year(), DEC);
  data_DHT.print('/');
  data_DHT.print(now.month(), DEC);
  data_DHT.print('/');
  data_DHT.print(now.day(), DEC);
  data_DHT.print(", ");
  data_DHT.print(now.hour(), DEC);
  data_DHT.print(':');
  data_DHT.print(now.minute(), DEC);
  data_DHT.print(':');
  data_DHT.print(now.second(), DEC);
  data_DHT.print(",");
  data_DHT.print(h);
  data_DHT.print(",");
  data_DHT.print(t);
  data_DHT.print(",");
  data_DHT.print(temperature);
  data_DHT.print(",");
  data_DHT.print(phValue);
  data_DHT.print(",");
  data_DHT.println(ecValue);
  data_DHT.print(",");
  data_DHT.print(ec_wat);
  data_DHT.print(",");
  data_DHT.print(ec_nut);
  data_DHT.print(",");
  data_DHT.print(ph_acid);
  data_DHT.print(",");
  data_DHT.println(ph_base);
  data_DHT.close();
}

void MyLib_::touch() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.fillRect(0, 0, 320, 40, RED);
  tft.setTextSize(3);
  tft.setCursor(35, 35);
  tft.println("Hydroponic");
  tft.setTextSize(2);
  tft.setCursor(0, 70);
  tft.println("Air Temp ");
  tft.setCursor(260, 70);
  tft.println(" °C ");
  tft.setCursor(0, 110 );
  tft.println("Air Hum ");
  tft.setCursor(260, 110);
  tft.println(" % ");
  tft.setCursor(0, 150);
  tft.println("W Temp ");
  tft.setCursor(260, 150);
  tft.println(" °C ");
  tft.setCursor(0, 190);
  tft.println("pH ");
  tft.setCursor(0, 230);
  tft.println("EC ");
  tft.setCursor(240, 230);
  tft.setCursor(180, 70);
  tft.println(t);
  tft.setCursor(180, 110 );
  tft.println(h);
  tft.setCursor(180, 150);
  tft.println(temperature);
  tft.setCursor(180, 190);
  tft.println(phValue);
  tft.setCursor(180, 230);
  tft.println(ecValue);
}

void MyLib_::serial() {
  Serial.print("Humidite: ");
  Serial.print(h);
  Serial.print(" \t Temperature: ");
  Serial.println(t);
  Serial.print("Water temp: ");
  Serial.print(w_temp);
  Serial.print(" \t pH: ");
  Serial.print(phValue);
  Serial.print(" \t EC: ");
  Serial.println(ecValue);
  Serial.print("ec_watTime: ");
  Serial.print(ec_wat);
  Serial.print(" \t ec_nutTime: ");
  Serial.print(ec_nut);
  Serial.print(" \t ph_acidTime: ");
  Serial.print(ph_acid);
  Serial.print(" \t ph_baseTime: ");
  Serial.println(ph_base);
}

void MyLib_::online() {  
  if (gprsSerial.available())
    Serial.write(gprsSerial.read());

  gprsSerial.println("AT");
  delay(1000);
  gprsSerial.println("AT+CPIN?");
  delay(1000);
  gprsSerial.println("AT+CREG?");
  delay(1000);
  gprsSerial.println("AT+CGATT?");
  delay(1000);
  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
  ShowSerialData();
  gprsSerial.println("AT+CSTT=\"internet\"");//start task and setting the APN,
  delay(1000);
  ShowSerialData();
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
  ShowSerialData();
  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
  ShowSerialData();
  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);
  ShowSerialData();
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
  ShowSerialData();
  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();

  String str = "GET https://api.thingspeak.com/update?api_key=K5JCYN4OO7KGGD1C&field1=" + String(t) + "&field2=" + String(h) +
               "&field3=" + String(w_temp) + "&field4=" + String(phValue) + "&field5=" + String(ecValue) + "&field6=" + String(ph_acid) +
               "&field7=" + String(ph_base) + "&field8=" + String(ec_wat)  + "&field9=" + String(ec_nut);
  Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server

  delay(4000);
  ShowSerialData();

  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet
  gprsSerial.println();
  ShowSerialData();
  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
}
void MyLib_::Gas_Sensor() {
   int analogSensor = analogRead(smokeA0);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
   danger;
   danger_1;
   
  
  }
  else
  {
    normal;
    normal_1;
   
  }
  delay(100);
  
}

void MyLib_::ShowSerialData() {
  while (gprsSerial.available() != 0)
    Serial.write(gprsSerial.read());
}

MyLib_ &MyLib_::getInstance() {
  static MyLib_ instance;
  return instance;
}

MyLib_ &MyLib = MyLib.getInstance();
