#include "MyLib.h"

void setup() {
  MyLib.begin();
  //Mylib.Gas_Sensor();
}

void loop() {
 MyLib.water_quality();
// Mylib.Gas_Sensor();
}
