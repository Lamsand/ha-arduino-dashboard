#line 1 "C:\\Users\\Sander\\OneDrive\\Documenten\\.Code\\Arduino\\Giga R1 wifi\\Home assistant\\GitHub\\ha-arduino-dashboard\\energy.h"
#ifndef energy_h
#define energy_h

#include "Arduino.h"

class Energy {
private:
public:
  int currentImport = 99999;
  int currentConsumption = 99999;
  int currentProduction = 99999;

  int import[360];
  int consumption[360];
  int production[360];
  int timeArr[360];

  bool longGraph = LOW;
  int longImport[360];
  int longConsumption[360];
  int longProduction[360];
  int longTimeArr[360];

  bool drawImport = HIGH;
  bool drawProduction = HIGH;
  bool drawConsumption = HIGH;

  bool newMinute = LOW;
  bool longNewMinute = LOW;

  bool roundState = HIGH;
  int state = 0;

  Energy();
};

#endif