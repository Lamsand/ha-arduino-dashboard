#ifndef Dashboard_h
#define Dashboard_h

#include "Arduino.h"
// #include "SD.h"

class Dashboard {
public:
  int publicTEXT;
  int publicBackg;
  int publicAllBackg;
  ////// BASICS /////////////
  bool begin();
  int fillArc(int x, int y, int start_angle, int seg_count, int r, int w, unsigned int colour);
  void home();
  void printTime(const String& time);
  void printHeatPumpTime(const String& time);
  void printWaterTemp(int temp);
  void printTemp(int temp);
  void setupWiFi();
  void WiFiSetUp();
  void setupHA();
  void HASetUp();
  void fillScreen(int color);
  ////// LOGOS //////////////
  void settingsLogo(int x, int y, int size, int color);
  void bulb(int x, int y, int size, bool state);
  void car(int x, int y, int size, int color);
  void warmtePomp(int x, int y, int size, int color);
  void HALogo(int x, int y, int size, int color, int backg);
  void noConn(int x, int y, int size);
  void WiFiIcon(int x, int y, int size, int color);
  ////// IMAGES /////////////
  // void readLarge(File& f, uint8_t* buf, uint32_t len);
  // ** lights **
  void moonlampImg(int xInput, int yInput, bool state);
  void bolImg(int xInput, int yInput, bool state);
  void bergingImg(int xInput, int yInput, bool state);
  void sfeerlichtjesImg(int xInput, int yInput, bool state);
  // ** car **
  void carImageMini(int xInput, int yInput);
  void carImageLarge(int xInput, int yInput);
  // ** music **
  void radio1(int xInput, int yInput);
  void radio2(int xInput, int yInput);
  void vuurland(int xInput, int yInput);
  void vrtnws(int xInput, int yInput);
  void spotify(int xInput, int yInput);
  void musicIcon(int xInput, int yInput);
  void radio1S(int xInput, int yInput);
  void radio2S(int xInput, int yInput);
  void vuurlandS(int xInput, int yInput);
  void vrtnwsS(int xInput, int yInput);
  void PommS(int xInput, int yInput);
  void play(int xInput, int yInput);
  void pause(int xInput, int yInput);
  // ** energy **
  void bliksem(int xInput, int yInput, int state);
  // ** waste **
  void wasteIcon(int xInput, int yInput, int wasteType);
  // ** ventilation **
  void ventiIconLarge(int xInput, int yInput, bool state);
  void ventiIcon(int xInput, int yInput, bool state);
  void clockIcon(int xInput, int yInput, int hours);
  void ventiStopIcon(int xInput, int yInput);
  // ** heat pump **
  void heatPumpIcon(int xInput, int yInput, bool state);
  void heatPumpIconSmall(int xInput, int yInput);
  void heatPumpIconLarge(int xInput, int yInput, bool state);
  void WaterTempIcon(int xInput, int yInput);
  void WaterTempIconSmall(int xInput, int yInput);
  // ** mower **
  void mowerIconLarge(int xInput, int yInput);
  void mowerIcon(int xInput, int yInput);
  void mdi_play(int xInput, int yInput, bool inverted);
  void mdi_pause(int xInput, int yInput, bool inverted);
  void mdi_returnHome(int xInput, int yInput, bool inverted);
  ////// HOME ///////////////
  void homeEnergy(int x, int y, int w, int h, int state, int value);
  void homeCar(int x, int y, int w, int h, int battery, float chargingCapacity, int target);
  void homeEmpty(int x, int y, int w, int h);
  void homeHeatPump(int x, int y, int w, int h, int DHWtemp);
  // ** value refresh **
  void homeEnergyV(int x, int y, int w, int h, int state, int value);
  void homeCarV(int x, int y, int w, int h, int battery, float chargingCapacity, int target);
  void homeHeatPumpV(int x, int y, int w, int h, int DHWtemp);
  // ** image draw **
  void homeEnergyImg(int x, int y, int w, int h, int state);
  void homeLightsImg(int x, int y, int w, int h);
  void homeCarImg(int x, int y, int w, int h);
  void homeMusicImg(int x, int y, int w, int h);
  void homeWasteImg(int x, int y, int w, int h, int wasteType);
  void homeVentiImg(int x, int y, int w, int h);
  void homeHeatPumpImg(int x, int y, int w, int h, bool state);
  void homeMowerImg(int x, int y, int w, int h);
  ////// DETAILS /////////////
  // ** energy **
  void energy(int* import, int* prod, int* cons, int* longImport, int* longProd, int* longCons, int currentImport, int currentProduction, int currentConsumption, int* TIME, int* LONGTIME, int state, bool longGraph, bool rounded, bool drawImport, bool drawProduction, bool drawConsumption);
  void energyGraph(int* import, int* prod, int* cons, int* longImport, int* longProd, int* longCons, int* TIME, int* LONGTIME, bool longGraph, bool rounded, bool drawImport, bool drawProduction, bool drawConsumption);
  void energyV(int currentImport, int currentProduction, int currentConsumption);
  // ** settings **
  void settings(bool theme, const String& time);
  // ** car **
  void detailCar(int battery, float chargingCapacity, int chargingSpeed, int target, int range);
  void evcc(int PV, int grid, int toGrid, int toHome, float toCar);
  // ** lights **
  void detailLights(bool sfeerlichtjes, bool groteBol, bool glazenBol, bool berging, bool maanlamp, bool raamversiering);
  void lightBr(int br);
  void lightSw();
  // ** music **
  void music(const String& k_kanaal, const String& k_title, bool k_state, const String& s_kanaal, const String& s_title, bool s_state);
  void musicV(const String& k_kanaal, const String& k_title, const String& s_kanaal, const String& s_title);
  void k_kanaal(const String& k_kanaal, bool k_state);
  void s_kanaal(const String& s_kanaal, bool s_state);
  void k_state(bool k_state);
  void s_state(bool s_state);
  String trimTitle(const String& title, int maxChars);
  void chooseChannel();
  // ** ventilation **
  void ventilationV(bool k_state, bool b_state, int k_CO2, int b_VOC, int k_hum, int b_hum, int k_qua, int b_qua, int k_time, int b_time);
  void ventilationImg(bool k_state, bool b_state);
  // ** heat pump **
  void heatPump(bool compr, int DHWtemp, int XDHW);
  // ** mower **
  void mower(int battery, bool mowing);
};

#endif