#ifndef energy_h
#define energy_h

#include "Arduino.h"

class Dashboard;

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

  bool newEnergyGraph = LOW;
  bool newEnergyVal = LOW;
  bool newBlxmInfo = LOW;

  Energy();
  void mqtt(const String &topic, const String &message, bool &newHomeInfo,
            const String &realTime);
  void newInfo(Dashboard &dashboard, const String &page, const int *energyCo,
               const int *primes, int homePage, float car_chargingPower,
               float currentEpex);
};

#endif