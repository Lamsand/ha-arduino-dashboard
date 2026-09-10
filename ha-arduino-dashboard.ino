/*
  ha-arduino-dashboard.ino - A dashboard for Home Assistant on the Arduino Giga
  Display

  Created by Sander Lambrechts, June 2026. This code is open source and
  may be used, modified, and distributed freely. This project is designed to run
  on the Arduino Giga Display, utilizing its touchscreen capabilities and SD
  card storage for a dynamic and interactive Home Assistant dashboard. The
  dashboard displays various home automation data such as energy consumption,
  music status, lighting, and more, allowing users to interact with their smart
  home devices directly from the display.

  https://github.com/Lamsand/ha-arduino-dashboard
*/

#include <ArduinoMqttClient.h>
#include <WiFi.h>

#include "Arduino_GigaDisplayTouch.h"
#include "Colors.h"
#include "Dashboard.h"
#include "arduino_secrets.h"
#include "config.h"

Arduino_GigaDisplayTouch touchDetector;
// Declarations
int lastTouch;
const int threshold = 250;  // time in milliseconds

int touch_x;
int touch_y;

#define SCREEN_W 800
#define SCREEN_H 480

Dashboard dashboard;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

/*
          ________________________________________
         | TIME                               set.|
         |__________________  ____________________|
         |                   |                    |
         | 0,  44, 400, 218  | 400,  44, 399, 218 |
         |___________________|____________________|
         |                   |                    |
         | 0, 262, 400, 217  | 400, 262, 399, 217 |
         |___________________|____________________|

  co[4]: 0 = every homepage, 1 = only first homepage, 2 = only second homepage,
  3 = hide
*/
#ifdef ENABLE_LIGHTS
int lightCo[5] = {0, 44, 400, 218, 1};
#endif
#ifdef ENABLE_CAR
int carCo[5] = {400, 44, 399, 218, 1};
#endif
#ifdef ENABLE_ENERGY
int energyCo[5] = {0, 262, 400, 217, 1};
#endif
#ifdef ENABLE_MUSIC
int musicCo[5] = {400, 262, 399, 217, 1};
#endif
#ifdef ENABLE_WASTE
int wasteCo[5] = {600, 262, 199, 217, 3};
#endif
#ifdef ENABLE_VENTILATION
int ventiCo[5] = {400, 262, 399, 217, 2};
#endif
#ifdef ENABLE_HEAT_PUMP
int heatPumpCo[5] = {400, 44, 399, 218, 2};
#endif
#ifdef ENABLE_MOWER
int mowerCo[5] = {0, 44, 400, 218, 2};
#endif
#ifdef ENABLE_PRICES
int priceCo[5] = {0, 262, 400, 217, 2};
#endif

#ifdef ENABLE_WASTE
int wasteType = 4;
#endif
#ifdef ENABLE_LIGHTS
bool lamp1state = LOW;
int lamp1brightness = 0;
bool lamp2state = LOW;
int lamp2brightness = 0;
bool lamp3state = LOW;
int lamp3brightness = 0;
bool lamp4state = LOW;
int lamp4brightness = 0;
bool lamp5state = LOW;
int lamp5brightness = 0;
bool lamp6state = LOW;
int lamp6brightness = 0;
#endif
#ifdef ENABLE_CAR
int laadpaal_battery = 0;
float laadpaal_chargingPower = 0;
int laadpaal_chargingSpeed = 0;
int laadpaal_cruisingRange = 0;
int laadpaal_targetCharge = 0;
#endif
#ifdef ENABLE_HEAT_PUMP
bool compressor = LOW;
int hotWaterTemp = 0;
int XDHW = 0;
String heatPumpTime = "00:00";
int heatPumpHour = 0;
int heatPumpMinute = 0;
#endif
#ifdef ENABLE_MOWER
int mowerBattery = 0;
bool mowerState = LOW;
#endif
#ifdef ENABLE_VENTILATION
int keuken_CO2 = 0;
int keuken_humidity = 0;
int keuken_quality = 0;
int keuken_boostRemaining = 8;
bool keuken_boost = HIGH;
int kelder_VOC = 0;
int kelder_humidity = 0;
int kelder_quality = 0;
int kelder_boostRemaining = 210;
bool kelder_boost = HIGH;
#endif
#ifdef ENABLE_ENERGY
int importArr[360];
int currentImport = 0;
int consumptionArr[360];
int currentConsumption = 0;
int productionArr[360];
int currentProduction = 0;
int timeArr[360];
bool newMinute = LOW;
int longImportArr[360];
int longConsumptionArr[360];
int longProductionArr[360];
int longTimeArr[360];
bool longNewMinute = LOW;
int energy_state = 0;
bool energy_roundState = HIGH;
bool energy_longGraph = LOW;
bool drawImport = HIGH;
bool drawProduction = HIGH;
bool drawConsumption = HIGH;
#endif
#ifdef ENABLE_MUSIC
String keuken_title = "";
String keuken_channel = "Spotify";
int keuken_volume = 0;
bool keuken_state = LOW;
String speelkamer_title = "";
String speelkamer_channel = "Spotify";
int speelkamer_volume = 0;
bool speelkamer_state = LOW;
#endif
#ifdef ENABLE_PRICES
float todayEpex[96];
float todayEcopower[96];
float tomorrowEpex[96];
float tomorrowEcopower[96];
bool tomorrowGraph = LOW;
bool drawEpex = HIGH;
bool drawEcopower = HIGH;
float currentEpex = 0;
float currentEcopower = 0;
bool newPricesGraph = LOW;
bool newPricesVal = LOW;
int clickedX = 0;
#endif

String realTime = "Default";
int realTimeInt = 0;
int nightStart = 2000;
int nightEnd = 0700;
bool night = LOW;

int outDoorTemp = 0;
String lastReset = "Default";
int lastEnergyNewValue = 40000;
String page = "setup";
int lastRefresh = 0;
int BACKG = BLACK;
int TEXT = WHITE;
int AllBackg = WHITE;
bool darkMode = HIGH;

////////////// new info /////////////
// ** home **
bool newHomeInfo = LOW;
#ifdef ENABLE_ENERGY
bool newEnergyGraph = LOW;
bool newEnergyVal = LOW;
bool newBlxmInfo = LOW;
#endif
#ifdef ENABLE_CAR
bool newCarInfo = LOW;
#endif
#ifdef ENABLE_MUSIC
bool newKeukenChan = LOW;
bool newSpeelkamerChan = LOW;
bool newKeuken = LOW;
bool newSpeelkamer = LOW;
bool newMusic = LOW;
#endif
#ifdef ENABLE_LIGHTS
bool lamp1change = LOW;
bool lamp2change = LOW;
bool lamp3change = LOW;
bool lamp4change = LOW;
bool lamp5change = LOW;
bool lamp6change = LOW;
#endif
#ifdef ENABLE_VENTILATION
bool newVentiV = LOW;
bool newVentiImg = LOW;
#endif

int loopCount = 0;

void setup() {
  Serial.begin(9600);
  delay(500);  // give SD card time to power up properly

  // Retry SD init up to 5 times
  bool sdOk = false;
  for (int i = 0; i < 5; i++) {
    if (dashboard.begin()) {
      Serial.println("SD success!");
      sdOk = true;
      break;
    }
    Serial.println("SD init failed, retrying...");
    delay(500);
  }
  if (!sdOk) {
    Serial.println("SD failed after retries!");
  }
  if (touchDetector.begin()) {
    Serial.println("Touch controller init - OK");
    Serial.println();
  } else {
    Serial.print("Touch controller init - FAILED");
    while (1);
  }
  // set colors in dashboard.h
  dashboard.publicTEXT = TEXT;
  dashboard.publicBackg = BACKG;
  dashboard.publicAllBackg = AllBackg;
  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(SECRET_SSID);
  dashboard.setupWiFi();
  while (WiFi.begin(SECRET_SSID, SECRET_PASS) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  dashboard.WiFiSetUp();
  Serial.print("Connected to the network, your IP: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  Serial.print("Your MAC: ");
  // Print MAC Address (Crucial for Deco identification)
  byte mac[6];
  WiFi.macAddress(mac);
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i > 0) Serial.print(":");
  }
  delay(2000);

  mqttClient.setId("arduinoInterface");

  mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);

  dashboard.setupHA();
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(SECRET_MQTT_HOST);

  while (!mqttClient.connect(SECRET_MQTT_HOST, SECRET_MQTT_PORT)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    delay(100);
  }
  delay(1000);
  dashboard.HASetUp();
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);
  // subscribe to a topic
  mqttClient.subscribe("home/#");
#ifdef ENABLE_LIGHTS
  mqttClient.subscribe("lights/#");
#endif
#ifdef ENABLE_CAR
  mqttClient.subscribe("laadpaal/#");
#endif
#ifdef ENABLE_ENERGY
  mqttClient.subscribe("energy/#");
#endif
#ifdef ENABLE_PRICES
  mqttClient.subscribe("prices/#");
#endif
#ifdef ENABLE_MUSIC
  mqttClient.subscribe("music/#");
#endif
#ifdef ENABLE_WASTE
  mqttClient.subscribe("afval/#");
#endif
#ifdef ENABLE_VENTILATION
  mqttClient.subscribe("ventilation/#");
#endif
#ifdef ENABLE_HEAT_PUMP
  mqttClient.subscribe("heatPump/#");
#endif
#ifdef ENABLE_MOWER
  mqttClient.subscribe("mower/#");
#endif

  delay(2000);
  // topics can be unsubscribed using:
  // mqttClient.unsubscribe("home/connection");
  page = "home";
  mqttClient.poll();
  print();
  // printImg();
  newHomeInfo = LOW;
}
void loop() {
  mqttClient.poll();
  if (loopCount == 400) {
    mqttClient.beginMessage("home/connection");
    mqttClient.print("Yoohoo!");
    mqttClient.endMessage();
    loopCount = 0;
  }
  loopCount++;
  uint8_t contacts;
  GDTpoint_t points[5];
  contacts = touchDetector.getTouchPoints(points);
  if (contacts > 0 && (millis() - lastTouch > threshold)) {  // touch
    lastTouch = millis();  // register last touch
    Serial.print("Contacts:");
    Serial.println(contacts);
    // record the x,y coordinates
    for (uint8_t i = 0; i < contacts; i++) {
      touch_x = 800 - points[i].y;
      touch_y = points[i].x;
    }
    if ((touch_x < 100 && page != "home" && page.indexOf("/") == -1 &&
         page != "prices") ||
        page == "blackScreen") {  // Return
      if (page == "blackScreen") {
        digitalWrite(74, HIGH);
        dashboard.reInit();
      }
      page = "home";
      print();
    } else if (page == "home") {
      if (touch_x > 700 && touch_y > 40) {  // to home2
        page = "home2";
        print2();
      }

#ifdef ENABLE_CAR
      else if (touch_y < carCo[1] + carCo[3] && touch_y > carCo[1] &&
               touch_x < carCo[0] + carCo[2] && touch_x > carCo[0] &&
               (carCo[4] == 0 || carCo[4] == 1)) {  // Car clicked
        page = "detailCar";
        dashboard.detailCar(laadpaal_battery, laadpaal_chargingPower,
                            laadpaal_chargingSpeed, laadpaal_targetCharge,
                            laadpaal_cruisingRange);
      }
#endif
#ifdef ENABLE_LIGHTS
      else if (touch_y < lightCo[1] + lightCo[3] && touch_y > lightCo[1] &&
               touch_x < lightCo[0] + lightCo[2] && touch_x > lightCo[0] &&
               (lightCo[4] == 0 || lightCo[4] == 1)) {  // Lights clicked
        page = "detailLights";
        dashboard.fillScreen(AllBackg);
        dashboard.detailLights();
        dashboard.lightRefresh(1, lamp1state);
        dashboard.lightRefresh(2, lamp2state);
        dashboard.lightRefresh(3, lamp3state);
        dashboard.lightRefresh(4, lamp4state);
        dashboard.lightRefresh(5, lamp5state);
        dashboard.lightRefresh(6, lamp6state);
      }
#endif
#ifdef ENABLE_ENERGY
      else if (touch_y < energyCo[1] + energyCo[3] && touch_y > energyCo[1] &&
               touch_x < energyCo[0] + energyCo[2] && touch_x > energyCo[0] &&
               (energyCo[4] == 0 || energyCo[4] == 1)) {  // Energy clicked
        page = "energy";
        dashboard.fillScreen(AllBackg);
        dashboard.energy(importArr, productionArr, consumptionArr,
                         longImportArr, longProductionArr, longConsumptionArr,
                         currentImport, currentProduction, currentConsumption,
                         timeArr, longTimeArr, energy_state, energy_longGraph,
                         energy_roundState, drawImport, drawProduction,
                         drawConsumption);
      }
#endif
#ifdef ENABLE_PRICES
      else if (touch_y < priceCo[1] + priceCo[3] && touch_y > priceCo[1] &&
               touch_x < priceCo[0] + priceCo[2] && touch_x > priceCo[0] &&
               (priceCo[4] == 0 || priceCo[4] == 1)) {  // Prices clicked
        page = "prices";
        dashboard.fillScreen(AllBackg);
        dashboard.prices(todayEpex, todayEcopower, tomorrowEpex,
                         tomorrowEcopower, currentEpex, currentEcopower,
                         tomorrowGraph, drawEpex, drawEcopower, clickedX);
      }
#endif
#ifdef ENABLE_MUSIC
      else if (touch_y < musicCo[1] + musicCo[3] && touch_y > musicCo[1] &&
               touch_x < musicCo[0] + musicCo[2] && touch_x > musicCo[0] &&
               (musicCo[4] == 0 || musicCo[4] == 1)) {  // Music clicked
        page = "music";
        dashboard.fillScreen(AllBackg);
        dashboard.music(keuken_channel, keuken_title, keuken_state,
                        speelkamer_channel, speelkamer_title, speelkamer_state);
      }
#endif
#ifdef ENABLE_HEAT_PUMP
      else if (touch_y < heatPumpCo[1] + heatPumpCo[3] &&
               touch_y > heatPumpCo[1] &&
               touch_x < heatPumpCo[0] + heatPumpCo[2] &&
               touch_x > heatPumpCo[0] &&
               (heatPumpCo[4] == 0 ||
                heatPumpCo[4] == 1)) {  // Heat Pump clicked
        page = "heatPump";
        dashboard.fillScreen(AllBackg);
        dashboard.heatPump(compressor, hotWaterTemp, XDHW);
      }
#endif
#ifdef ENABLE_MOWER
      else if (touch_y < mowerCo[1] + mowerCo[3] && touch_y > mowerCo[1] &&
               touch_x < mowerCo[0] + mowerCo[2] && touch_x > mowerCo[0] &&
               (mowerCo[4] == 0 || mowerCo[4] == 1)) {  // Mower clicked
        page = "mower";
        dashboard.fillScreen(AllBackg);
        dashboard.mower(mowerBattery, mowerState);
      }
#endif
#ifdef ENABLE_VENTILATION
      else if (touch_y < ventiCo[1] + ventiCo[3] && touch_y > ventiCo[1] &&
               touch_x < ventiCo[0] + ventiCo[2] && touch_x > ventiCo[0] &&
               (ventiCo[4] == 0 || ventiCo[4] == 1)) {  // Ventilation clicked
        page = "venti";
        dashboard.fillScreen(AllBackg);
        dashboard.homeEmpty(0, 0, 798, 239);
        dashboard.homeEmpty(0, 239, 798, 239);
        dashboard.ventilationV(keuken_boost, kelder_boost, keuken_CO2,
                               kelder_VOC, keuken_humidity, kelder_humidity,
                               keuken_quality, kelder_quality,
                               keuken_boostRemaining, kelder_boostRemaining);
        dashboard.ventilationImg(keuken_boost, kelder_boost);
      }
#endif
      else if (touch_x > 746 && touch_x < 800 && touch_y > 0 &&
               touch_y < 44) {  // Settings clicked
        page = "settings";
        dashboard.settings(darkMode, lastReset);
      }
    } else if (page == "home2") {
      if (0) {
      }
#ifdef ENABLE_CAR
      else if (touch_y < carCo[1] + carCo[3] && touch_y > carCo[1] &&
               touch_x < carCo[0] + carCo[2] && touch_x > carCo[0] &&
               (carCo[4] == 0 || carCo[4] == 2)) {  // Car clicked
        page = "detailCar";
        dashboard.detailCar(laadpaal_battery, laadpaal_chargingPower,
                            laadpaal_chargingSpeed, laadpaal_targetCharge,
                            laadpaal_cruisingRange);
      }
#endif
#ifdef ENABLE_LIGHTS
      else if (touch_y < lightCo[1] + lightCo[3] && touch_y > lightCo[1] &&
               touch_x < lightCo[0] + lightCo[2] && touch_x > lightCo[0] &&
               (lightCo[4] == 0 || lightCo[4] == 2)) {  // Lights clicked
        page = "detailLights";
        dashboard.fillScreen(AllBackg);
        dashboard.detailLights();
        dashboard.lightRefresh(1, lamp1state);
        dashboard.lightRefresh(2, lamp2state);
        dashboard.lightRefresh(3, lamp3state);
        dashboard.lightRefresh(4, lamp4state);
        dashboard.lightRefresh(5, lamp5state);
        dashboard.lightRefresh(6, lamp6state);
      }
#endif
#ifdef ENABLE_ENERGY
      else if (touch_y < energyCo[1] + energyCo[3] && touch_y > energyCo[1] &&
               touch_x < energyCo[0] + energyCo[2] && touch_x > energyCo[0] &&
               (energyCo[4] == 0 || energyCo[4] == 2)) {  // Energy clicked
        page = "energy";
        dashboard.fillScreen(AllBackg);
        dashboard.energy(importArr, productionArr, consumptionArr,
                         longImportArr, longProductionArr, longConsumptionArr,
                         currentImport, currentProduction, currentConsumption,
                         timeArr, longTimeArr, energy_state, energy_longGraph,
                         energy_roundState, drawImport, drawProduction,
                         drawConsumption);
      }
#endif
#ifdef ENABLE_PRICES
      else if (touch_y < priceCo[1] + priceCo[3] && touch_y > priceCo[1] &&
               touch_x < priceCo[0] + priceCo[2] && touch_x > priceCo[0] &&
               (priceCo[4] == 0 || priceCo[4] == 2)) {  // Prices clicked
        page = "prices";
        dashboard.fillScreen(AllBackg);
        dashboard.prices(todayEpex, todayEcopower, tomorrowEpex,
                         tomorrowEcopower, currentEpex, currentEcopower,
                         tomorrowGraph, drawEpex, drawEcopower, clickedX);
      }
#endif
#ifdef ENABLE_HEAT_PUMP
      else if (touch_y < heatPumpCo[1] + heatPumpCo[3] &&
               touch_y > heatPumpCo[1] &&
               touch_x < heatPumpCo[0] + heatPumpCo[2] &&
               touch_x > heatPumpCo[0] &&
               (heatPumpCo[4] == 0 ||
                heatPumpCo[4] == 2)) {  // Heat Pump clicked
        page = "heatPump";
        dashboard.fillScreen(AllBackg);
        dashboard.heatPump(compressor, hotWaterTemp, XDHW);
      }
#endif
#ifdef ENABLE_MOWER
      else if (touch_y < mowerCo[1] + mowerCo[3] && touch_y > mowerCo[1] &&
               touch_x < mowerCo[0] + mowerCo[2] && touch_x > mowerCo[0] &&
               (mowerCo[4] == 0 || mowerCo[4] == 2)) {  // Mower clicked
        page = "mower";
        dashboard.fillScreen(AllBackg);
        dashboard.mower(mowerBattery, mowerState);
      }
#endif
#ifdef ENABLE_MUSIC
      else if (touch_y < musicCo[1] + musicCo[3] && touch_y > musicCo[1] &&
               touch_x < musicCo[0] + musicCo[2] && touch_x > musicCo[0] &&
               (musicCo[4] == 0 || musicCo[4] == 2)) {  // Music clicked
        page = "music";
        dashboard.fillScreen(AllBackg);
        dashboard.music(keuken_channel, keuken_title, keuken_state,
                        speelkamer_channel, speelkamer_title, speelkamer_state);
      }
#endif
#ifdef ENABLE_VENTILATION
      else if (touch_y < ventiCo[1] + ventiCo[3] && touch_y > ventiCo[1] &&
               touch_x < ventiCo[0] + ventiCo[2] && touch_x > ventiCo[0] &&
               (ventiCo[4] == 0 || ventiCo[4] == 2)) {  // Ventilation clicked
        page = "venti";
        dashboard.fillScreen(AllBackg);
        dashboard.homeEmpty(0, 0, 799, 239);
        dashboard.homeEmpty(0, 239, 799, 239);
        dashboard.ventilationV(keuken_boost, kelder_boost, keuken_CO2,
                               kelder_VOC, keuken_humidity, kelder_humidity,
                               keuken_quality, kelder_quality,
                               keuken_boostRemaining, kelder_boostRemaining);
        dashboard.ventilationImg(keuken_boost, kelder_boost);
      }
#endif
      else if (touch_x > 746 && touch_x < 800 && touch_y > 0 &&
               touch_y < 44) {  // Settings clicked
        page = "settings";
        dashboard.settings(darkMode, lastReset);
      }
    } else if (page == "settings") {
      if (touch_x < 360 && touch_x > 285 && touch_y < 140 &&
          touch_y > 109) {  // darkMode clicked
        darkMode = !darkMode;
        if (darkMode) {
          BACKG = BLACK;
          TEXT = WHITE;
          AllBackg = WHITE;
          dashboard.publicTEXT = TEXT;
          dashboard.publicBackg = BACKG;
          dashboard.publicAllBackg = AllBackg;
        } else {
          BACKG = WHITE;
          TEXT = BLACK;
          AllBackg = BLACK;
          dashboard.publicTEXT = TEXT;
          dashboard.publicBackg = BACKG;
          dashboard.publicAllBackg = AllBackg;
        }
        dashboard.settings(darkMode, lastReset);
      } else if (touch_x < 170 && touch_x > 130 && touch_y < 462 &&
                 touch_y > 422) {
        Serial.println("RESET!!!!!");
        NVIC_SystemReset();
      }
    }
#ifdef ENABLE_LIGHTS
    else if (page == "detailLights") {
#ifdef LAMP1_ENABLED
      if (touch_x > lamp1Co[0] && touch_x < lamp1Co[0] + lamp1Co[2] &&
          touch_y > lamp1Co[1] &&
          touch_y < lamp1Co[1] + lamp1Co[3]) {  // Lamp 1
#ifndef LAMP1_ON_OFF
        contacts = touchDetector.getTouchPoints(points);
        while (contacts == 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        while (contacts > 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        if (lastTouch + 600 < millis()) {
          page = "detailLights/sfeerlichtjes";
          dashboard.lightSw();
          dashboard.lightBr(lamp1brightness);
        } else {
#endif
          mqttClient.beginMessage("lights/sfeerlichtjes");
          mqttClient.print("Switch");
          mqttClient.endMessage();
#ifndef LAMP1_ON_OFF
        }
#endif
      }
#endif
#ifdef LAMP2_ENABLED
      if (touch_x > lamp2Co[0] && touch_x < lamp2Co[0] + lamp2Co[2] &&
          touch_y > lamp2Co[1] &&
          touch_y < lamp2Co[1] + lamp2Co[3]) {  // Lamp 2
#ifndef LAMP2_ON_OFF
        contacts = touchDetector.getTouchPoints(points);
        while (contacts == 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        while (contacts > 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        if (lastTouch + 600 < millis()) {
          page = "detailLights/groteBol";
          dashboard.lightSw();
          dashboard.lightBr(lamp2brightness);
        } else {
#endif
          Serial.println("Tap");
          mqttClient.beginMessage("lights/groteBol");
          mqttClient.print("Switch");
          mqttClient.endMessage();
#ifndef LAMP2_ON_OFF
        }
#endif
      }
#endif
#ifdef LAMP3_ENABLED
      if (touch_x > lamp3Co[0] && touch_x < lamp3Co[0] + lamp3Co[2] &&
          touch_y > lamp3Co[1] &&
          touch_y < lamp3Co[1] + lamp3Co[3]) {  // Lamp 3
#ifndef LAMP3_ON_OFF
        contacts = touchDetector.getTouchPoints(points);
        while (contacts == 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        while (contacts > 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        if (lastTouch + 600 < millis()) {
          page = "detailLights/glazenBol";
          dashboard.lightSw();
          dashboard.lightBr(lamp3brightness);
        } else {
#endif
          Serial.println("Tap");
          mqttClient.beginMessage("lights/glazenBol");
          mqttClient.print("Switch");
          mqttClient.endMessage();
#ifndef LAMP3_ON_OFF
        }
#endif
      }
#endif
#ifdef LAMP4_ENABLED
      if (touch_x > lamp4Co[0] && touch_x < lamp4Co[0] + lamp4Co[2] &&
          touch_y > lamp4Co[1] &&
          touch_y < lamp4Co[1] + lamp4Co[3]) {  // Lamp 4
#ifndef LAMP4_ON_OFF
        contacts = touchDetector.getTouchPoints(points);
        while (contacts == 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        while (contacts > 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        if (lastTouch + 600 < millis()) {
          page = "detailLights/maanlamp";
          dashboard.lightSw();
          dashboard.lightBr(lamp4brightness);
        } else {
#endif
          Serial.println("Tap");
          mqttClient.beginMessage("lights/maanlamp");
          mqttClient.print("Switch");
          mqttClient.endMessage();
#ifndef LAMP4_ON_OFF
        }
#endif
      }
#endif
#ifdef LAMP5_ENABLED
      if (touch_x > lamp5Co[0] && touch_x < lamp5Co[0] + lamp5Co[2] &&
          touch_y > lamp5Co[1] &&
          touch_y < lamp5Co[1] + lamp5Co[3]) {  // Lamp 5
#ifndef LAMP5_ON_OFF
        contacts = touchDetector.getTouchPoints(points);
        while (contacts == 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        while (contacts > 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        if (lastTouch + 600 < millis()) {
          page = "detailLights/raamversiering";
          dashboard.lightSw();
          dashboard.lightBr(lamp5brightness);
        } else {
#endif
          mqttClient.beginMessage("lights/raamversiering");
          mqttClient.print("Switch");
          mqttClient.endMessage();
#ifndef LAMP5_ON_OFF
        }
#endif
      }
#endif
#ifdef LAMP6_ENABLED
      if (touch_x > lamp6Co[0] && touch_x < lamp6Co[0] + lamp6Co[2] &&
          touch_y > lamp6Co[1] &&
          touch_y < lamp6Co[1] + lamp6Co[3]) {  // Lamp 6
#ifndef LAMP6_ON_OFF
        contacts = touchDetector.getTouchPoints(points);
        while (contacts == 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        while (contacts > 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
          if (lastTouch + 600 < millis()) {
            break;
          }
        }
        if (lastTouch + 600 < millis()) {
          page = "detailLights/berging";
          dashboard.lightSw();
          dashboard.lightBr(lamp6brightness);
        } else {
#endif
          Serial.println("Tap");
          mqttClient.beginMessage("lights/berging");
          mqttClient.print("Switch");
          mqttClient.endMessage();
#ifndef LAMP6_ON_OFF
        }
#endif
      }
#endif
    } else if (page.startsWith("detailLights/") == HIGH) {
      String k = page;
      k.replace("detailLights/", "");
      Serial.println(k);
      if ((touch_x < 300 || touch_x > 500) ||
          (touch_y < 40 || touch_y > 440)) {  // Return
        Serial.println("return");
        page = "detailLights";
        dashboard.fillScreen(AllBackg);
        dashboard.detailLights();
        dashboard.lightRefresh(1, lamp1state);
        dashboard.lightRefresh(2, lamp2state);
        dashboard.lightRefresh(3, lamp3state);
        dashboard.lightRefresh(4, lamp4state);
        dashboard.lightRefresh(5, lamp5state);
        dashboard.lightRefresh(6, lamp6state);
      }
      if (touch_x > 300 && touch_x < 500 && touch_y > 340 &&
          touch_y < 440) {  // O/I button clicked
        mqttClient.beginMessage("lights/" + k);
        mqttClient.print("Switch");
        mqttClient.endMessage();
      }
    }
#endif
#ifdef ENABLE_MUSIC
    else if (page.startsWith("music")) {
      if (page == "music") {
        if (touch_x < 350 && touch_x > 50 && touch_y < 350 && touch_y > 50 &&
            (touch_x < 250 || touch_x > 350 || touch_y < 300 ||
             touch_y > 400)) {  // Keuken icon clicked
          page = "music/keuken";
          dashboard.chooseChannel();
          newMusic = LOW;
          newKeukenChan = LOW;
          newSpeelkamerChan = LOW;
          newSpeelkamer = LOW;
          newKeuken = LOW;
        }
        if (touch_x > 250 && touch_x < 350 && touch_y > 300 &&
            touch_y < 400) {  // Keuken button clicked
          mqttClient.beginMessage("music/keuken");
          mqttClient.print("Switch");
          mqttClient.endMessage();
        }
        if (touch_x < 750 && touch_x > 450 && touch_y < 350 && touch_y > 50 &&
            (touch_x < 650 || touch_x > 750 || touch_y < 300 ||
             touch_y > 400)) {  // Speelkamer icon clicked
          page = "music/speelkamer";
          dashboard.chooseChannel();
          newMusic = LOW;
        }
        if (touch_x > 650 && touch_x < 750 && touch_y > 300 &&
            touch_y < 400) {  // Speelkamer button clicked
          mqttClient.beginMessage("music/speelkamer");
          mqttClient.print("Switch");
          mqttClient.endMessage();
        }
      } else if (page == "music/keuken") {
        if (touch_x > 100) {
          mqttClient.beginMessage("music/keuken");
          if (touch_y < 240 && touch_y > 0) {
            if (touch_x < 275) {
              mqttClient.print("toRadio1");
            } else if (touch_x < 525) {
              mqttClient.print("toRadio2");
            } else {
              mqttClient.print("toPommelienThijs");
            }
          } else {
            if (touch_x < 400) {
              mqttClient.print("toNWS");
            } else {
              mqttClient.print("toVuurland");
            }
          }
          mqttClient.endMessage();
        }
        page = "music";
        dashboard.music(keuken_channel, keuken_title, keuken_state,
                        speelkamer_channel, speelkamer_title, speelkamer_state);
      } else if (page == "music/speelkamer") {
        if (touch_x > 100) {
          mqttClient.beginMessage("music/speelkamer");
          if (touch_y < 240 && touch_y > 0) {
            if (touch_x < 275) {
              mqttClient.print("toRadio1");
            } else if (touch_x < 525) {
              mqttClient.print("toRadio2");
            } else {
              mqttClient.print("toPommelienThijs");
            }
          } else {
            if (touch_x < 400) {
              mqttClient.print("toNWS");
            } else {
              mqttClient.print("toVuurland");
            }
          }
          mqttClient.endMessage();
        }
        page = "music";
        dashboard.music(keuken_channel, keuken_title, keuken_state,
                        speelkamer_channel, speelkamer_title, speelkamer_state);
      }
    }
#endif
#ifdef ENABLE_ENERGY
    else if (page == "energy") {
      newEnergyGraph = HIGH;
      if (touch_x > 440 && touch_x < 800 && touch_y > 20 &&
          touch_y < 100) {  // long graph clicked
        energy_longGraph = !energy_longGraph;
      } else if (touch_x > 440 && touch_x < 480) {
        if (touch_y > 310 && touch_y < 350) {
          drawConsumption = !drawConsumption;
        } else if (touch_y > 355 && touch_y < 395) {
          drawImport = !drawImport;
        } else if (touch_y > 400 && touch_y < 440) {
          drawProduction = !drawProduction;
        }
      }
#ifdef ENABLE_evcc
      else if (touch_x > 450 + 340 / 4 - 66 && touch_x < 450 + 340 / 4 + 66 &&
               touch_y > 124 + 156 / 2 - 85 &&
               touch_y <
                   124 + 156 / 2 + 85) {  // lightning bolt clicked: to evcc
        page = "evcc";
        dashboard.fillScreen(AllBackg);
        dashboard.homeEmpty(0, 0, 798, 479);
        Serial.println(currentEpex);
        dashboard.evcc(currentProduction, currentImport, currentConsumption,
                       laadpaal_chargingPower, currentEpex);
      }
#endif
      else if (!energy_longGraph) {
        energy_roundState = !energy_roundState;
      } else {
        newEnergyGraph = LOW;
      }
    }
#endif
#ifdef ENABLE_PRICES
    else if (page == "prices") {
      newPricesGraph = HIGH;
      if (touch_x < 60) {
        page = "home";
        print();
      } else if (touch_x > 440 && touch_x < 800 && touch_y > 20 &&
                 touch_y < 100) {  // tomorrow graph clicked
        tomorrowGraph = !tomorrowGraph;
      } else if (touch_x > 440 && touch_x < 480) {
        if (touch_y > 350 && touch_y < 390) {
          drawEpex = !drawEpex;
        } else if (touch_y > 400 && touch_y < 440) {
          drawEcopower = !drawEcopower;
        }
      } else if (touch_x > 61 && touch_x < 420 && touch_y > 40 &&
                 touch_y < 440) {
        clickedX = touch_x;
      }
    }
#endif
#ifdef ENABLE_VENTILATION
    else if (page == "venti") {
      if (keuken_boost) {
        if (touch_x > 595 && touch_x < 765 && touch_y > 35 &&
            touch_y < 205) {  // Keuken 1u
          mqttClient.beginMessage("ventilation/keuken/control");
          mqttClient.print("Stop");
          mqttClient.endMessage();
        }
      } else {
        if (touch_x > 390 && touch_x < 560 && touch_y > 35 &&
            touch_y < 205) {  // Keuken 1u
          mqttClient.beginMessage("ventilation/keuken/control");
          mqttClient.print(60);
          mqttClient.endMessage();
        }
        if (touch_x > 595 && touch_x < 765 && touch_y > 35 &&
            touch_y < 205) {  // Keuken 1u
          mqttClient.beginMessage("ventilation/keuken/control");
          mqttClient.print(180);
          mqttClient.endMessage();
        }
      }
      if (kelder_boost) {
        if (touch_x > 595 && touch_x < 765 && touch_y > 275 &&
            touch_y < 445) {  // Keuken 1u
          mqttClient.beginMessage("ventilation/kelder/control");
          mqttClient.print("Stop");
          mqttClient.endMessage();
        }
      } else {
        if (touch_x > 390 && touch_x < 560 && touch_y > 275 &&
            touch_y < 445) {  // Keuken 1u
          mqttClient.beginMessage("ventilation/kelder/control");
          mqttClient.print(60);
          mqttClient.endMessage();
        }
        if (touch_x > 595 && touch_x < 765 && touch_y > 275 &&
            touch_y < 445) {  // Keuken 1u
          mqttClient.beginMessage("ventilation/kelder/control");
          mqttClient.print(180);
          mqttClient.endMessage();
        }
      }
    }
#endif
#ifdef ENABLE_MOWER
    else if (page == "mower") {
      if (mowerState) {
        if (touch_x > 264 && touch_x < 464 && touch_y > 360 &&
            touch_y < 440) {  // Return
          mqttClient.beginMessage("mower/set");
          mqttClient.print("Return");
          mqttClient.endMessage();
        }
        if (touch_x > 40 && touch_x < 240 && touch_y > 360 &&
            touch_y < 440) {  // Pause
          mqttClient.beginMessage("mower/set");
          mqttClient.print("Pause");
          mqttClient.endMessage();
        }
      } else {
        if (touch_x > 40 && touch_x < 464 && touch_y > 360 &&
            touch_y < 440) {  // Start
          mqttClient.beginMessage("mower/set");
          mqttClient.print("Start");
          mqttClient.endMessage();
        }
      }
    }
#endif
#ifdef ENABLE_CAR
#ifdef ENABLE_evcc
    else if (page == "detailCar") {
      if (touch_x > 600) {
        page = "evcc";
        dashboard.fillScreen(AllBackg);
        dashboard.homeEmpty(0, 0, 798, 479);
        Serial.println(currentEpex);
        dashboard.evcc(currentProduction, currentImport, currentConsumption,
                       laadpaal_chargingPower, currentEpex);
      }
    }
#endif
#endif
#ifdef ENABLE_HEAT_PUMP
    else if (page == "heatPump") {
      if (touch_x < 300) {
        page = "heatPump/ChooseHour";
        dashboard.heatPumpChooseHour(heatPumpHour, heatPumpMinute, -1, -1);
      }
    } else if (page.startsWith("heatPump/")) {
      if (touch_x < 100) {
        page = "heatPump";
        dashboard.heatPump(compressor, hotWaterTemp, XDHW);
      }
      if (touch_x > 418 && touch_x < 454 && touch_y > 374 && touch_y < 442) {
        page = "heatPump/ChooseMinute";
        dashboard.heatPumpChooseMinute(heatPumpHour, heatPumpMinute, -1, -1);
      }
      if (touch_x > 346 && touch_x < 382 && touch_y > 374 && touch_y < 442) {
        page = "heatPump/ChooseHour";
        dashboard.heatPumpChooseHour(heatPumpHour, heatPumpMinute, -1, -1);
      }
    }
#endif

    lastTouch = millis();
  }

  if (newHomeInfo) {
    if (page == "home") {
      printV();
      // Serial.println("Refresh");
    } else if (page == "home2") {
      print2V();
    }
    newHomeInfo = LOW;
  }
#ifdef ENABLE_LIGHTS
  if (lamp1change || lamp2change || lamp3change || lamp4change || lamp6change ||
      lamp5change) {
    if (page.startsWith("detailLights")) {
      // Serial.println("Refresh");
      if (lamp1change) {
        dashboard.lightRefresh(1, lamp1state);
      }
      if (lamp2change) {
        dashboard.lightRefresh(2, lamp2state);
      }
      if (lamp3change) {
        dashboard.lightRefresh(3, lamp3state);
      }
      if (lamp4change) {
        dashboard.lightRefresh(4, lamp4state);
      }
      if (lamp5change) {
        dashboard.lightRefresh(5, lamp5state);
      }
      if (lamp6change) {
        dashboard.lightRefresh(6, lamp6state);
      }
      if (page.startsWith("detailLights/") == HIGH) {
        dashboard.lightSw();
        if (page.endsWith("groteBol") == HIGH) {
          dashboard.lightBr(lamp3brightness);
        } else if (page.endsWith("glazenBol") == HIGH) {
          dashboard.lightBr(lamp2brightness);
        } else if (page.endsWith("berging") == HIGH) {
          dashboard.lightBr(lamp6brightness);
        } else if (page.endsWith("maanlamp") == HIGH) {
          dashboard.lightBr(lamp4brightness);
        }
      }
    }
    lamp1change = LOW;
    lamp2change = LOW;
    lamp3change = LOW;
    lamp4change = LOW;
    lamp6change = LOW;
    lamp5change = LOW;
  }
#endif
#ifdef ENABLE_MUSIC
  if (newMusic || newKeukenChan || newSpeelkamerChan || newSpeelkamer ||
      newKeuken) {
    if (page == "music") {
      if (newKeukenChan) {
        dashboard.k_kanaal(keuken_channel, keuken_state);
      }
      if (newSpeelkamerChan) {
        dashboard.s_kanaal(speelkamer_channel, speelkamer_state);
      }
      if (newSpeelkamer) {
        dashboard.s_state(speelkamer_state);
      }
      if (newKeuken) {
        dashboard.k_state(keuken_state);
      }
      if (newMusic) {
        dashboard.musicV(keuken_channel, keuken_title, speelkamer_channel,
                         speelkamer_title);
        dashboard.k_state(keuken_state);
        dashboard.s_state(speelkamer_state);
      }
    }
    newMusic = LOW;
    newKeukenChan = LOW;
    newSpeelkamerChan = LOW;
    newSpeelkamer = LOW;
    newKeuken = LOW;
  }
#endif
#ifdef ENABLE_ENERGY
  if (newEnergyVal || newEnergyGraph || newBlxmInfo) {
    if (page == "energy") {
      if (newEnergyVal) {
        dashboard.energyV(currentImport, currentProduction, currentConsumption);
      }
      if (newBlxmInfo) {
        dashboard.bliksem(450 + 340 / 4, 124 + 156 / 2, energy_state);
      }
      if (newEnergyGraph) {
        dashboard.energyGraph(importArr, productionArr, consumptionArr,
                              longImportArr, longProductionArr,
                              longConsumptionArr, timeArr, longTimeArr,
                              energy_longGraph, energy_roundState, drawImport,
                              drawProduction, drawConsumption);
      }
      // Serial.println("Refresh");
    }
    if (page == "home" && newBlxmInfo) {
      if (energyCo[4] == 0 || energyCo[4] == 1) {
        dashboard.bliksem(0 + 399 / 4, 262 + 217 / 2, energy_state);
      }
    } else if (page == "home2" && newBlxmInfo) {
      if (energyCo[4] == 0 || energyCo[4] == 2) {
        dashboard.bliksem(0 + 399 / 4, 262 + 217 / 2, energy_state);
      }
    }
    if (page == "evcc") {
      Serial.println(currentEcopower);
      dashboard.evcc(currentProduction, currentImport, currentConsumption,
                     laadpaal_chargingPower, currentEpex);
    }
    newBlxmInfo = LOW;
    newEnergyVal = LOW;
    newEnergyGraph = LOW;
  }
#endif
#ifdef ENABLE_VENTILATION
  if (newVentiV || newVentiImg) {
    if (page == "venti") {
      if (newVentiV) {
        dashboard.ventilationV(keuken_boost, kelder_boost, keuken_CO2,
                               kelder_VOC, keuken_humidity, kelder_humidity,
                               keuken_quality, kelder_quality,
                               keuken_boostRemaining, kelder_boostRemaining);
      }
      if (newVentiImg) {
        dashboard.ventilationImg(keuken_boost, kelder_boost);
      }
    }
    newVentiV = LOW;
    newVentiImg = LOW;
  }
#endif
#ifdef ENABLE_PRICES
  if (newPricesVal || newPricesGraph) {
    if (page == "prices") {
      if (newPricesVal) {
        dashboard.pricesV(currentEpex, currentEcopower, clickedX);
      }
      if (newPricesGraph) {
        dashboard.pricesGraph(todayEpex, todayEcopower, tomorrowEpex,
                              tomorrowEcopower, currentEpex, currentEcopower,
                              tomorrowGraph, drawEpex, drawEcopower, clickedX);
      }
    }
    newPricesVal = LOW;
    newPricesGraph = LOW;
    clickedX = 0;
  }
#endif

  if (WiFi.status() != WL_CONNECTED || lastEnergyNewValue + 15000 < millis()) {
    NVIC_SystemReset();
  }
  if (night && lastTouch + 60000 < millis()) {  // Shut down at night
    dashboard.fillScreen(BLACK);
    digitalWrite(74, LOW);
    page = "blackScreen";
  } else if (lastTouch + 60000 < millis() && page != "home" &&
             page.indexOf("/") == -1) {  // Return to home after a while
    page = "home";
    print();
    // printImg();
  }
  delay(10);
}
void onMqttMessage(int messageSize) {
  String topic = mqttClient.messageTopic();
  String message = mqttClient.readString();
  if (topic.startsWith("prices")) {  // Print topic & message
    Serial.print("'");
    Serial.print(topic);
    Serial.print("'   '");
    Serial.print(message);
    Serial.println("'");
    Serial.println();
  }
  ///////////////////////////////////////////////////////// home/#
  if (topic == "home/time") {
    // message += "L";
    // message.replace(":00L", "");
    realTime = message;
    message.replace(":", "");
    realTimeInt = message.toInt();
    if (page == "home") {
      dashboard.printTime(realTime);
    }
    if (realTimeInt > nightStart || realTimeInt < nightEnd) {
      night = HIGH;
    } else {
      if (night) {
        night = LOW;
        digitalWrite(74, HIGH);
        dashboard.reInit();
        print();
        page = "home";
      }
    }
    newMinute = HIGH;
    longNewMinute = HIGH;
    if (lastReset == "Default") {
      lastReset = realTime;
    }
  }
  if (topic == "home/temp") {
    outDoorTemp = message.toInt();
    if (page == "home") {
      dashboard.printTemp(outDoorTemp);
    }
  }
#ifdef ENABLE_CAR
  //////////////////////////////////////////////////////// laadpaal/#
  if (topic.startsWith("laadpaal/") == HIGH) {
    if (topic == "laadpaal/battery") {
      laadpaal_battery = message.toInt();
      newHomeInfo = HIGH;
    }
    if (topic == "laadpaal/chargingPower") {
      laadpaal_chargingPower = message.toFloat();
      newHomeInfo = HIGH;
    }
    /*if (topic == "laadpaal/chargingSpeed") {
      laadpaal_chargingSpeed = message.toInt();
    }*/
    if (topic == "laadpaal/cruisingRange") {
      laadpaal_cruisingRange = message.toInt();
    }
    if (topic == "laadpaal/targetCharge") {
      laadpaal_targetCharge = message.toInt();
      newHomeInfo = HIGH;
    }
  }
#endif  // ENABLE_CAR

#ifdef ENABLE_LIGHTS
  /////////////////////////////////////////////////////// lights/#
  if (topic.startsWith("lights/") == HIGH) {
    if (message != "Switch") {
      if (topic == "lights/sfeerlichtjes") {
        if (message == "on") {
          lamp1state = 1;
        } else if (message == "off") {
          lamp1state = 0;
        }
        lamp1change = HIGH;
      }
      if (topic == "lights/groteBol") {
        if (message == "") {
          lamp2brightness = 0;
          lamp2state = 0;
        } else {
          lamp2brightness = message.toInt() * 100 / 255;
          lamp2state = 1;
        }
        lamp2change = HIGH;
      }
      if (topic == "lights/glazenBol") {
        if (message == "") {
          lamp3brightness = 0;
          lamp3state = 0;
        } else {
          lamp3brightness = message.toInt() * 100 / 255;
          lamp3state = 1;
        }
        lamp3change = HIGH;
      }
      if (topic == "lights/maanlamp") {
        if (message == "") {
          lamp4brightness = 0;
          lamp4state = 0;
        } else {
          lamp4brightness = message.toInt() * 100 / 255;
          lamp4state = 1;
        }
        lamp4change = HIGH;
      }
      if (topic == "lights/raamversiering") {
        if (message == "on") {
          lamp5state = 1;
        } else if (message == "off") {
          lamp5state = 0;
        }
        lamp5change = HIGH;
      }
      if (topic == "lights/berging") {
        if (message == "") {
          lamp6brightness = 0;
          lamp6state = 0;
        } else {
          lamp6brightness = message.toInt() * 100 / 255;
          lamp6state = 1;
        }
        lamp6change = HIGH;
      }
    }
  }
#endif  // ENABLE_LIGHTS

#ifdef ENABLE_ENERGY
  /////////////////////////////////////////////////////// energy/#
  if (topic.startsWith("energy/") == HIGH) {
    if (topic == "energy/currentImport") {
      message.replace(".0", "");
      newHomeInfo = HIGH;
      newEnergyVal = HIGH;
      currentImport = message.toInt();
      if (currentImport < 0) {
        if (energy_state != 0) {
          energy_state = 0;
          newBlxmInfo = HIGH;
        }
      } else if (currentImport < 4800) {
        if (energy_state != 1) {
          energy_state = 1;
          newBlxmInfo = HIGH;
        }
      } else {
        if (energy_state != 2) {
          energy_state = 2;
          newBlxmInfo = HIGH;
        }
      }
    }
    if (topic == "energy/currentProduction") {
      message.replace(".0", "");
      currentProduction = message.toInt();
    }
    if (topic == "energy/currentConsumption") {
      message.replace(".0", "");
      currentConsumption = message.toInt();
    }
    if (topic == "energy/newValue") {
      if (message == "Short") {
        String k = realTime;
        k.replace(":", "");
        for (int i = 0; i < 359; i++) {
          importArr[i] = importArr[i + 1];
          consumptionArr[i] = consumptionArr[i + 1];
          productionArr[i] = productionArr[i + 1];
          timeArr[i] = timeArr[i + 1];
          // Serial.print(importArr[i]);
          // Serial.print(" ");
        }
        if (currentImport < -1111) {
          importArr[359] = -1111;
        } else {
          importArr[359] = currentImport;
        }
        if (currentConsumption < -1111) {
          consumptionArr[359] = -1111;
        } else {
          consumptionArr[359] = currentConsumption;
        }
        productionArr[359] = currentProduction;
        // Serial.print(importArr[359]);
        // Serial.println();

        if (k.toInt() % 100 == 0 && newMinute) {
          timeArr[359] = k.toInt() / 100;
          newMinute = LOW;
        } else {
          timeArr[359] = 0;
        }
        newEnergyGraph = HIGH;
      } else {
        String k = realTime;
        k.replace(":", "");
        for (int i = 0; i < 359; i++) {
          longImportArr[i] = longImportArr[i + 1];
          longConsumptionArr[i] = longConsumptionArr[i + 1];
          longProductionArr[i] = longProductionArr[i + 1];
          longTimeArr[i] = longTimeArr[i + 1];
          // Serial.print(longImportArr[i]);
          // Serial.print(" ");
        }
        if (currentImport < -1111) {
          longImportArr[359] = -1111;
        } else {
          longImportArr[359] = currentImport;
        }
        if (currentConsumption < -1111) {
          longConsumptionArr[359] = -1111;
        } else {
          longConsumptionArr[359] = currentConsumption;
        }
        longProductionArr[359] = currentProduction;
        // Serial.print(longImportArr[359]);
        // Serial.println();
        if (k.toInt() % 100 == 0 && longNewMinute) {
          longTimeArr[359] = k.toInt() / 100;
          longNewMinute = LOW;
        } else {
          longTimeArr[359] = 0;
        }
        newEnergyGraph = HIGH;
      }
      lastEnergyNewValue = millis();
    }
  }
#endif  // ENABLE_ENERGY
#ifdef ENABLE_PRICES
  /////////////////////////////////////////////////////// prices/#
  if (topic.startsWith("prices/") == HIGH) {
    if (topic == "prices/currentEpex") {
      // newEnergyVal = HIGH;
      currentEpex = message.toFloat();
    }
    if (topic == "prices/currentEcopower") {
      // newEnergyVal = HIGH;
      currentEcopower = message.toFloat();
    }
    if (topic == "prices/todayEpex") {
      int index = 0;
      const char* ptr = message.c_str();

      // Skip opening bracket
      while (*ptr && *ptr != '[') ptr++;
      if (*ptr == '[') ptr++;

      while (index < 96 && *ptr) {
        char* end;
        float value = strtof(ptr, &end);

        if (end != ptr) {
          todayEpex[index++] = value;
          ptr = end;
        } else {
          ptr++;
        }

        while (*ptr && (*ptr == ',' || *ptr == ' ' || *ptr == '\n' ||
                        *ptr == '\r' || *ptr == '\t')) {
          ptr++;
        }

        if (*ptr == ']') break;
      }

      Serial.println("✓ Loaded todayEpex: ");
      Serial.print("  {");
      for (int i = 0; i < 96; i++) {
        Serial.print(todayEpex[i]);
        if (i < 95) Serial.print("; ");
      }
      Serial.println("}");
    }
    if (topic == "prices/todayEcopower") {
      int index = 0;
      const char* ptr = message.c_str();

      // Skip opening bracket
      while (*ptr && *ptr != '[') ptr++;
      if (*ptr == '[') ptr++;

      while (index < 96 && *ptr) {
        char* end;
        float value = strtof(ptr, &end);

        if (end != ptr) {
          todayEcopower[index++] = value;
          ptr = end;
        } else {
          ptr++;
        }

        while (*ptr && (*ptr == ',' || *ptr == ' ' || *ptr == '\n' ||
                        *ptr == '\r' || *ptr == '\t')) {
          ptr++;
        }

        if (*ptr == ']') break;
      }

      Serial.println("✓ Loaded todayEcopower");
    }
    if (topic == "prices/tomorrowEpex") {
      if (message == "false") {
        for (int i = 0; i < 96; i++) {
          tomorrowEpex[i] = 0;
        }
        tomorrowEpex[0] = -100;
      } else {
        int index = 0;
        const char* ptr = message.c_str();

        // Skip opening bracket
        while (*ptr && *ptr != '[') ptr++;
        if (*ptr == '[') ptr++;

        while (index < 96 && *ptr) {
          char* end;
          float value = strtof(ptr, &end);

          if (end != ptr) {
            tomorrowEpex[index++] = value;
            ptr = end;
          } else {
            ptr++;
          }

          while (*ptr && (*ptr == ',' || *ptr == ' ' || *ptr == '\n' ||
                          *ptr == '\r' || *ptr == '\t')) {
            ptr++;
          }

          if (*ptr == ']') break;
        }

        Serial.println("✓ Loaded tomorrowEpex");
      }
    }
    if (topic == "prices/tomorrowEcopower") {
      if (message == "false") {
        for (int i = 0; i < 96; i++) {
          tomorrowEcopower[i] = 0;
        }
        tomorrowEcopower[0] = -100;
      } else {
        int index = 0;
        const char* ptr = message.c_str();

        // Skip opening bracket
        while (*ptr && *ptr != '[') ptr++;
        if (*ptr == '[') ptr++;

        while (index < 96 && *ptr) {
          char* end;
          float value = strtof(ptr, &end);

          if (end != ptr) {
            tomorrowEcopower[index++] = value;
            ptr = end;
          } else {
            ptr++;
          }

          while (*ptr && (*ptr == ',' || *ptr == ' ' || *ptr == '\n' ||
                          *ptr == '\r' || *ptr == '\t')) {
            ptr++;
          }

          if (*ptr == ']') break;
        }

        Serial.println("✓ Loaded tomorrowEcopower");
      }
    }
  }
#endif  // ENABLE_PRICES

#ifdef ENABLE_MUSIC
  if (topic.startsWith("music/") && message != "Switch" && message != "Up" &&
      message != "Down" && message != "Next" && message != "Last") {
    newMusic = HIGH;
    if (topic.startsWith("music/keuken")) {
      if (topic == "music/keuken/volume") {
        keuken_volume = message.toInt();
      }
      if (topic == "music/keuken/title") {
        keuken_title = message;
      }
      if (topic == "music/keuken/channel") {
        if (message == "VRT Radio 1" || message == "Radio 1") {
          message = "Radio 1";
        } else if (message == "VRT Studio Brussel Vuurland" ||
                   message == "Studio Brussel Vuurland" ||
                   message == "Vuurland") {
          message = "Vuurland";
        } else if (message == "VRT NWS") {
        } else if (message == "Radio 2 vlaams-brabant" ||
                   message == "Radio 2") {
          message = "Radio 2";
        } else if (message == "Spotify Connect" || message == "" ||
                   message == "Spotify") {
          message = "Spotify";
        }
        keuken_channel = message;
        newKeukenChan = HIGH;
      }
      if (topic == "music/keuken/state") {
        if (message == "playing") {
          keuken_state = HIGH;
        } else {
          keuken_state = LOW;
        }
        newKeuken = HIGH;
      }
    }
    if (topic.startsWith("music/speelkamer")) {
      if (topic == "music/speelkamer/volume") {
        speelkamer_volume = message.toInt();
      }
      if (topic == "music/speelkamer/title") {
        speelkamer_title = message;
      }
      if (topic == "music/speelkamer/channel") {
        if (message == "VRT Radio 1" || message == "Radio 1") {
          message = "Radio 1";
        } else if (message == "VRT Studio Brussel Vuurland" ||
                   message == "Studio Brussel Vuurland" ||
                   message == "Vuurland") {
          message = "Vuurland";
        } else if (message == "VRT NWS") {
        } else if (message == "Radio 2 vlaams-brabant" ||
                   message == "Radio 2") {
          message = "Radio 2";
        } else if (message == "Spotify Connect" || message == "" ||
                   message == "Spotify") {
          message = "Spotify";
        }
        speelkamer_channel = message;
        newSpeelkamerChan = HIGH;
      }
      if (topic == "music/speelkamer/state") {
        if (message == "playing") {
          speelkamer_state = HIGH;
        } else {
          speelkamer_state = LOW;
        }
        newSpeelkamer = HIGH;
      }
    }
  }
#endif  // ENABLE_MUSIC

#ifdef ENABLE_WASTE
  if (topic.startsWith("afval")) {
//   Wastetype:  Rest = 0; GFT = 1;  PMD = 2;  Papier = 3;  Rest + GFT = 4; PMD
//   + GFT = 5; Papier + GFT = 6; None = 7
#ifdef ENABLE_MUSIC
    musicCo[2] = 199;
#endif
    wasteCo[4] = 1;
    if (message.indexOf("GFT") != -1) {
      if (message.indexOf("Restafval") != -1 ||
          message.indexOf("restafval") != -1) {
        wasteType = 4;
      } else if (message.indexOf("PMD") != -1 || message.indexOf("pmd") != -1) {
        wasteType = 5;
      } else if (message.indexOf("Papier") != -1 ||
                 message.indexOf("papier") != -1) {
        wasteType = 6;
      } else {
        wasteType = 1;
      }
    } else if (message.indexOf("Restafval") != -1 ||
               message.indexOf("restafval") != -1) {
      wasteType = 0;
    } else if (message.indexOf("PMD") != -1) {
      wasteType = 2;
    } else if (message.indexOf("Papier") != -1 ||
               message.indexOf("papier") != -1) {
      wasteType = 3;
    } else {
      wasteType = 7;
#ifdef ENABLE_MUSIC
      musicCo[2] = 399;  // { 400, 262, 399, 217 };
#endif
      wasteCo[4] = 3;  // { 900, 262, 199, 217 };
    }
    print();
  }
#endif  // ENABLE_WASTE

#ifdef ENABLE_VENTILATION
  if (topic.startsWith("ventilation/")) {
    newVentiV = HIGH;
    if (topic.startsWith("ventilation/keuken")) {
      if (topic.startsWith("ventilation/keuken/CO2")) {
        keuken_CO2 = message.toInt();
      }
      if (topic.startsWith("ventilation/keuken/humidity")) {
        keuken_humidity = message.toInt();
      }
      if (topic.startsWith("ventilation/keuken/state")) {
        if (message == "off") {
          keuken_boost = LOW;
        } else {
          keuken_boost = HIGH;
        }
        newVentiImg = HIGH;
      }
      if (topic.startsWith("ventilation/keuken/remaining")) {
        keuken_boostRemaining = message.toInt();
      }
      if (topic.startsWith("ventilation/keuken/remaining")) {
        keuken_boostRemaining = message.toInt();
      }
      if (topic.startsWith("ventilation/keuken/AQI")) {
        keuken_quality = message.toInt();
      }
    }
    if (topic.startsWith("ventilation/kelder")) {
      newVentiV = HIGH;
      if (topic.startsWith("ventilation/kelder/VOC")) {
        kelder_VOC = message.toInt();
      }
      if (topic.startsWith("ventilation/kelder/humidity")) {
        kelder_humidity = message.toInt();
      }
      if (topic.startsWith("ventilation/kelder/state")) {
        if (message == "off") {
          kelder_boost = LOW;
        } else {
          kelder_boost = HIGH;
        }
        newVentiImg = HIGH;
      }
      if (topic.startsWith("ventilation/kelder/remaining")) {
        kelder_boostRemaining = message.toInt();
      }
      if (topic.startsWith("ventilation/kelder/remaining")) {
        kelder_boostRemaining = message.toInt();
      }
      if (topic.startsWith("ventilation/kelder/AQI")) {
        kelder_quality = message.toInt();
      }
    }
  }
#endif  // ENABLE_VENTILATION

#ifdef ENABLE_HEAT_PUMP
  if (topic.startsWith("heatPump")) {
    if (topic == "heatPump/compressor") {
      if (message == "off") {
        compressor = LOW;
      }
      if (message == "on") {
        compressor = HIGH;
      }
    }
    if (topic == "heatPump/hotWaterTemp") {
      hotWaterTemp = message.toInt();
      newHomeInfo = HIGH;
    }
    if (topic == "heatPump/XDHW") {
      XDHW = message.toInt();
    }
    if (topic == "heatPump/nextTime") {
      heatPumpTime = message;
      heatPumpHour = message.substring(0, 2).toInt();
      heatPumpMinute = message.substring(3, 5).toInt();
      if (page == "home") {
        dashboard.printHeatPumpTime(heatPumpTime);
      }
    }
  }
#endif  // ENABLE_HEAT_PUMP

#ifdef ENABLE_MOWER
  if (topic.startsWith("mower/")) {
    if (topic == "mower/battery") {
      mowerBattery = message.toInt();
    }
    if (topic == "mower/state") {
      if (message == "docked" || message == "paused") {
        mowerState = LOW;
      }
      if (message == "mowing") {
        mowerState = HIGH;
      }
    }
  }
#endif  // ENABLE_MOWER
}
void print() {
  dashboard.home();
  dashboard.printTime(realTime);
  dashboard.printTemp(outDoorTemp);
#ifdef ENABLE_HEAT_PUMP
  dashboard.printHeatPumpTime(heatPumpTime);
  dashboard.printWaterTemp(hotWaterTemp);
#endif
#ifdef ENABLE_LIGHTS
  if (lightCo[4] == 0 || lightCo[4] == 1) {
    dashboard.homeEmpty(lightCo[0], lightCo[1], lightCo[2], lightCo[3]);
  }
#endif
#ifdef ENABLE_CAR
  if (carCo[4] == 0 || carCo[4] == 1) {
    dashboard.homeCar(carCo[0], carCo[1], carCo[2], carCo[3], laadpaal_battery,
                      laadpaal_chargingPower, laadpaal_targetCharge);
  }
#endif
#ifdef ENABLE_MUSIC
  if (musicCo[4] == 0 || musicCo[4] == 1) {
    dashboard.homeEmpty(musicCo[0], musicCo[1], musicCo[2], musicCo[3]);
  }
#endif
#ifdef ENABLE_VENTILATION
  if (ventiCo[4] == 0 || ventiCo[4] == 1) {
    dashboard.homeEmpty(ventiCo[0], ventiCo[1], ventiCo[2], ventiCo[3]);
  }
#endif
#ifdef ENABLE_ENERGY
  if (energyCo[4] == 0 || energyCo[4] == 1) {
    dashboard.homeEnergy(energyCo[0], energyCo[1], energyCo[2], energyCo[3],
                         energy_state, currentImport);
  }
#endif
#ifdef ENABLE_WASTE
  if (wasteCo[4] == 0 || wasteCo[4] == 1) {
    dashboard.homeEmpty(wasteCo[0], wasteCo[1], wasteCo[2], wasteCo[3]);
  }
#endif
#ifdef ENABLE_HEAT_PUMP
  if (heatPumpCo[4] == 0 || heatPumpCo[4] == 1) {
    dashboard.homeHeatPump(heatPumpCo[0], heatPumpCo[1], heatPumpCo[2],
                           heatPumpCo[3], hotWaterTemp);
  }
#endif
#ifdef ENABLE_MOWER
  if (mowerCo[4] == 0 || mowerCo[4] == 1) {
    dashboard.homeEmpty(mowerCo[0], mowerCo[1], mowerCo[2], mowerCo[3]);
  }
#endif
#ifdef ENABLE_PRICES
  if (priceCo[4] == 0 || priceCo[4] == 1) {
    dashboard.homePrices(priceCo[0], priceCo[1], priceCo[2], priceCo[3],
                         currentEpex);
  }
#endif
  printImg();
}
void printV() {
#ifdef ENABLE_CAR
  if (carCo[4] == 0 || carCo[4] == 1) {
    dashboard.homeCarV(carCo[0], carCo[1], carCo[2], carCo[3], laadpaal_battery,
                       laadpaal_chargingPower, laadpaal_targetCharge);
  }
#endif
#ifdef ENABLE_ENERGY
  if (energyCo[4] == 0 || energyCo[4] == 1) {
    dashboard.homeEnergyV(energyCo[0], energyCo[1], energyCo[2], energyCo[3],
                          energy_state, currentImport);
  }
#endif
#ifdef ENABLE_HEAT_PUMP
  if (heatPumpCo[4] == 0 || heatPumpCo[4] == 1) {
    dashboard.homeHeatPumpV(heatPumpCo[0], heatPumpCo[1], heatPumpCo[2],
                            heatPumpCo[3], hotWaterTemp);
  }
#endif
#ifdef ENABLE_PRICES
  if (priceCo[4] == 0 || priceCo[4] == 1) {
    dashboard.homePricesV(priceCo[0], priceCo[1], priceCo[2], priceCo[3],
                          currentEpex);
  }
#endif
}
void printImg() {
#ifdef ENABLE_LIGHTS
  if (lightCo[4] == 0 || lightCo[4] == 1) {
    dashboard.homeLightsImg(lightCo[0], lightCo[1], lightCo[2], lightCo[3]);
  }
#endif
#ifdef ENABLE_CAR
  if (carCo[4] == 0 || carCo[4] == 1) {
    dashboard.homeCarImg(carCo[0], carCo[1], carCo[2], carCo[3]);
  }
#endif
#ifdef ENABLE_ENERGY
  if (energyCo[4] == 0 || energyCo[4] == 1) {
    dashboard.homeEnergyImg(energyCo[0], energyCo[1], energyCo[2], energyCo[3],
                            energy_state);
  }
#endif
#ifdef ENABLE_MUSIC
  if (musicCo[4] == 0 || musicCo[4] == 1) {
    dashboard.homeMusicImg(musicCo[0], musicCo[1], musicCo[2], musicCo[3]);
  }
#endif
#ifdef ENABLE_VENTILATION
  if (ventiCo[4] == 0 || ventiCo[4] == 1) {
    dashboard.homeVentiImg(ventiCo[0], ventiCo[1], ventiCo[2], ventiCo[3]);
  }
#endif
#ifdef ENABLE_WASTE
  if (wasteCo[4] == 0 || wasteCo[4] == 1) {
    dashboard.homeWasteImg(wasteCo[0], wasteCo[1], wasteCo[2], wasteCo[3],
                           wasteType);
  }
#endif
#ifdef ENABLE_HEAT_PUMP
  if (heatPumpCo[4] == 0 || heatPumpCo[4] == 1) {
    dashboard.homeHeatPumpImg(heatPumpCo[0], heatPumpCo[1], heatPumpCo[2],
                              heatPumpCo[3], compressor);
  }
#endif
#ifdef ENABLE_MOWER
  if (mowerCo[4] == 0 || mowerCo[4] == 1) {
    dashboard.homeMowerImg(mowerCo[0], mowerCo[1], mowerCo[2], mowerCo[3]);
  }
#endif
}
void print2() {
  dashboard.home();
  dashboard.printTime(realTime);
  dashboard.printTemp(outDoorTemp);
#ifdef ENABLE_HEAT_PUMP
  dashboard.printHeatPumpTime(heatPumpTime);
  dashboard.printWaterTemp(hotWaterTemp);
#endif
#ifdef ENABLE_LIGHTS
  if (lightCo[4] == 0 || lightCo[4] == 2) {
    dashboard.homeEmpty(lightCo[0], lightCo[1], lightCo[2], lightCo[3]);
  }
#endif
#ifdef ENABLE_CAR
  if (carCo[4] == 0 || carCo[4] == 2) {
    dashboard.homeCar(carCo[0], carCo[1], carCo[2], carCo[3], laadpaal_battery,
                      laadpaal_chargingPower, laadpaal_targetCharge);
  }
#endif
#ifdef ENABLE_MUSIC
  if (musicCo[4] == 0 || musicCo[4] == 2) {
    dashboard.homeEmpty(musicCo[0], musicCo[1], musicCo[2], musicCo[3]);
  }
#endif
#ifdef ENABLE_VENTILATION
  if (ventiCo[4] == 0 || ventiCo[4] == 2) {
    dashboard.homeEmpty(ventiCo[0], ventiCo[1], ventiCo[2], ventiCo[3]);
  }
#endif
#ifdef ENABLE_ENERGY
  if (energyCo[4] == 0 || energyCo[4] == 2) {
    dashboard.homeEnergy(energyCo[0], energyCo[1], energyCo[2], energyCo[3],
                         energy_state, currentImport);
  }
#endif
#ifdef ENABLE_WASTE
  if (wasteCo[4] == 0 || wasteCo[4] == 2) {
    dashboard.homeEmpty(wasteCo[0], wasteCo[1], wasteCo[2], wasteCo[3]);
  }
#endif
#ifdef ENABLE_HEAT_PUMP
  if (heatPumpCo[4] == 0 || heatPumpCo[4] == 2) {
    dashboard.homeHeatPump(heatPumpCo[0], heatPumpCo[1], heatPumpCo[2],
                           heatPumpCo[3], hotWaterTemp);
  }
#endif
#ifdef ENABLE_MOWER
  if (mowerCo[4] == 0 || mowerCo[4] == 2) {
    dashboard.homeEmpty(mowerCo[0], mowerCo[1], mowerCo[2], mowerCo[3]);
  }
#endif
#ifdef ENABLE_PRICES
  if (priceCo[4] == 0 || priceCo[4] == 2) {
    dashboard.homePrices(priceCo[0], priceCo[1], priceCo[2], priceCo[3],
                         currentEpex);
  }
#endif
  print2Img();
}
void print2V() {
#ifdef ENABLE_CAR
  if (carCo[4] == 0 || carCo[4] == 2) {
    dashboard.homeCarV(carCo[0], carCo[1], carCo[2], carCo[3], laadpaal_battery,
                       laadpaal_chargingPower, laadpaal_targetCharge);
  }
#endif
#ifdef ENABLE_ENERGY
  if (energyCo[4] == 0 || energyCo[4] == 2) {
    dashboard.homeEnergyV(energyCo[0], energyCo[1], energyCo[2], energyCo[3],
                          energy_state, currentImport);
  }
#endif
#ifdef ENABLE_HEAT_PUMP
  if (heatPumpCo[4] == 0 || heatPumpCo[4] == 2) {
    dashboard.homeHeatPumpV(heatPumpCo[0], heatPumpCo[1], heatPumpCo[2],
                            heatPumpCo[3], hotWaterTemp);
  }
#endif
#ifdef ENABLE_PRICES
  if (priceCo[4] == 0 || priceCo[4] == 2) {
    dashboard.homePricesV(priceCo[0], priceCo[1], priceCo[2], priceCo[3],
                          currentEpex);
  }
#endif
}
void print2Img() {
#ifdef ENABLE_LIGHTS
  if (lightCo[4] == 0 || lightCo[4] == 2) {
    dashboard.homeLightsImg(lightCo[0], lightCo[1], lightCo[2], lightCo[3]);
  }
#endif
#ifdef ENABLE_CAR
  if (carCo[4] == 0 || carCo[4] == 2) {
    dashboard.homeCarImg(carCo[0], carCo[1], carCo[2], carCo[3]);
  }
#endif
#ifdef ENABLE_ENERGY
  if (energyCo[4] == 0 || energyCo[4] == 2) {
    dashboard.homeEnergyImg(energyCo[0], energyCo[1], energyCo[2], energyCo[3],
                            energy_state);
  }
#endif
#ifdef ENABLE_MUSIC
  if (musicCo[4] == 0 || musicCo[4] == 2) {
    dashboard.homeMusicImg(musicCo[0], musicCo[1], musicCo[2], musicCo[3]);
  }
#endif
#ifdef ENABLE_VENTILATION
  if (ventiCo[4] == 0 || ventiCo[4] == 2) {
    dashboard.homeVentiImg(ventiCo[0], ventiCo[1], ventiCo[2], ventiCo[3]);
  }
#endif
#ifdef ENABLE_WASTE
  if (wasteCo[4] == 0 || wasteCo[4] == 2) {
    dashboard.homeWasteImg(wasteCo[0], wasteCo[1], wasteCo[2], wasteCo[3],
                           wasteType);
  }
#endif
#ifdef ENABLE_HEAT_PUMP
  if (heatPumpCo[4] == 0 || heatPumpCo[4] == 2) {
    dashboard.homeHeatPumpImg(heatPumpCo[0], heatPumpCo[1], heatPumpCo[2],
                              heatPumpCo[3], compressor);
  }
#endif
#ifdef ENABLE_MOWER
  if (mowerCo[4] == 0 || mowerCo[4] == 2) {
    dashboard.homeMowerImg(mowerCo[0], mowerCo[1], mowerCo[2], mowerCo[3]);
  }
#endif
}