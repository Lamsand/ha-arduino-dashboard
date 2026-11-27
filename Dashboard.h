#ifndef Dashboard_h
#define Dashboard_h

#include "Arduino.h"
#include "Arduino_GigaDisplay_GFX.h"
#include "SdFat.h"
#include "config.h"

// --- Global Constants ---
#define SCREEN_W 800
#define SCREEN_H 480
#define chipSelect 5
#define buffSize 150000

#ifdef ENABLE_LIGHTS
class Lights;
#endif

#ifdef ENABLE_ENERGY
#include "energy.h"
#endif
#ifdef ENABLE_LIGHTS
#include "lights.h"
#endif

class Dashboard {
private:
  GigaDisplay_GFX display;
  SdFat sd;
  FsFile file;
  uint16_t rowBuffer[buffSize];

public:
  int publicTEXT;
  int publicBackg;
  int publicAllBackg;
  ////// BASICS /////////////
  bool begin();
  void reInit();
  int fillArc(int x, int y, int start_angle, int seg_count, int r, int w,
              unsigned int colour);
  void home();
  void printTime(const String &time);
  void printHeatPumpTime(const String &time);
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
  void readLarge(FsFile &f, uint8_t *buf, uint32_t len);
  void imageSimulator(int xInput, int yInput, int w, int h, int color);
  // ** lights **
  void moonlampImg(int xInput, int yInput, bool state);
  void globeLightImg(int xInput, int yInput, bool state);
  void PhilipsHueImg(int xInput, int yInput, bool state);
  void christmasLightsImg(int xInput, int yInput, bool state);
  void lightRefresh(const Lights &lights, int lampnumber, bool state);
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
  void evccIcon(int xInput, int yInput, int state);
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
  // ** relays **
  void automationImg(int xInput, int yInput, bool state);
  void switchImg(int xInput, int yInput, bool state);
  void relayRefresh(int relaynumber, bool state);
  ////// HOME ///////////////
  void homeEnergy();
  void homeCar(int x, int y, int w, int h, int battery, float chargingCapacity,
               int target);
  void homeEmpty(int x, int y, int w, int h);
  void homeHeatPump(int x, int y, int w, int h, int DHWtemp);
  void homePrices(int x, int y, int w, int h, float currentPrice);
  // ** value refresh **
  void homeEnergyV();
  void homeCarV(int x, int y, int w, int h, int battery, float chargingCapacity,
                int target);
  void homeHeatPumpV(int x, int y, int w, int h, int DHWtemp);
  void homePricesV(int x, int y, int w, int h, float currentPrice);
  // ** image draw **
  void homeEnergyImg();
  void homeLightsImg(int x, int y, int w, int h);
  void homeCarImg(int x, int y, int w, int h);
  void homeMusicImg(int x, int y, int w, int h);
  void homeWasteImg(int x, int y, int w, int h, int wasteType);
  void homeVentiImg(int x, int y, int w, int h);
  void homeHeatPumpImg(int x, int y, int w, int h, bool state);
  void homeMowerImg(int x, int y, int w, int h);
  void homeRelaysImg(int x, int y, int w, int h);
  ////// DETAILS /////////////
  // ** energy **
#ifdef ENABLE_ENERGY
  Energy energy;
  void detailEnergy();
  void energyGraph();
  void energyV();
#endif
  // ** prices **
#ifdef ENABLE_PRICES
  void prices(float *epex, float *ecopower, float *tomorrowEpex,
              float *tomorrowEcopower, float currentEpex, float currentEcopower,
              bool tomorrowGraph, bool drawEpex, bool drawEcopower,
              int clickedX);
  void pricesGraph(float *epex, float *ecopower, float *tomorrowEpex,
                   float *tomorrowEcopower, float currentEpex,
                   float currentEcopower, bool tomorrowGraph, bool drawEpex,
                   bool drawEcopower, int clickedX);
  void pricesV(float currentEpex, float currentEcopower, int clickedX);
  void detailPrices(int clickedHour, int clickedMinute, float epex,
                    float ecopower);
#endif
  // ** settings **
  void settings(bool theme, const String &time);
  // ** car **
#ifdef ENABLE_CAR
  void detailCar(int battery, float chargingCapacity, int chargingSpeed,
                 int target, int range);
  void evcc(int PV, int grid, int consumption, float toCar, float currentPrice);
#endif
  // ** lights **
#ifdef ENABLE_LIGHTS
  Lights lights;
  void detailLights(const Lights &lights);
  void lightBr(int br);
  void lightSw();
#endif
  // ** music **
#ifdef ENABLE_MUSIC
  void music(const String &k_kanaal, const String &k_title, bool k_state,
             const String &s_kanaal, const String &s_title, bool s_state);
  void musicV(const String &k_kanaal, const String &k_title,
              const String &s_kanaal, const String &s_title);
  void k_kanaal(const String &k_kanaal, bool k_state);
  void s_kanaal(const String &s_kanaal, bool s_state);
  void k_state(bool k_state);
  void s_state(bool s_state);
  String trimTitle(const String &title, int maxChars);
  void chooseChannel();
#endif
  // ** ventilation **
#ifdef ENABLE_VENTILATION
  void ventilationV(bool k_state, bool b_state, int k_CO2, int b_VOC, int k_hum,
                    int b_hum, int k_qua, int b_qua, int k_time, int b_time);
  void ventilationImg(bool k_state, bool b_state);
#endif
  // ** heat pump **
#ifdef ENABLE_HEAT_PUMP
  void heatPump(bool compr, int DHWtemp, int XDHW);
  void heatPumpChooseHour(int setH, int setM, bool clicked, bool PM);
  void heatPumpChooseMinute(int setH, int setM, bool clicked);
  void heatPumpTimeAlreadyPassed();
  void heatPumpAlreadyHot();
#endif
  // ** mower **
#ifdef ENABLE_MOWER
  void mower(int battery, bool mowing);
#endif
  // ** relays **
#ifdef ENABLE_RELAYS
  void detailRelays();
#endif
};

#endif