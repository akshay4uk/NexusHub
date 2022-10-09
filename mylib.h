#include <Arduino.h>
#pragma once

class MyLib_ {
  private:
    MyLib_() = default; // Make constructor private

  public:
    static MyLib_ &getInstance(); // Accessor for singleton instance

    MyLib_(const MyLib_ &) = delete; // no copying
    MyLib_ &operator=(const MyLib_ &) = delete;

  public:

    void begin();
    void datalog();
    void water_quality();
    void ShowSerialData();
    void card();
    void serial();
    void touch();
    void online();
    float wt_sensor();
    float ec_sensor();
    float ph_sensor();

};

extern MyLib_ &MyLib;
