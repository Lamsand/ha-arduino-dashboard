#include <WiFi.h>
#include <ArduinoMqttClient.h>
#include "arduino_secrets.h"
#include "Dashboard.h"
#include "Colors.h"
#include "Arduino_GigaDisplayTouch.h"

Arduino_GigaDisplayTouch touchDetector;
// Declarations
int lastTouch;
const int threshold = 250;  //time in milliseconds

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
    */

int lightCo[5] = { 0, 44, 400, 218, 1 };
int carCo[5] = { 400, 44, 399, 218, 1 };
int energyCo[5] = { 0, 262, 400, 217, 0 };
int musicCo[5] = { 400, 262, 399, 217, 1 };
int wasteCo[5] = { 900, 262, 199, 217, 1 };
int ventiCo[5] = { 400, 262, 399, 217, 2 };
int heatPumpCo[5] = { 400, 44, 399, 218, 2 };
int mowerCo[5] = { 0, 44, 400, 218, 2 };

// waste
int wasteType = 4;

// lights
bool lights_sfeerlichtjes = LOW;
bool lights_glazenBol = LOW;
int lights_glazenBol_br = 0;
bool lights_groteBol = LOW;
int lights_groteBol_br = 0;
bool lights_berging = LOW;
int lights_berging_br = 0;
bool lights_maanlamp = LOW;
int lights_maanlamp_br = 0;
bool lights_raamversiering = LOW;

// laadpaal
int laadpaal_battery = 0;
float laadpaal_chargingPower = 0;
int laadpaal_chargingSpeed = 0;
int laadpaal_cruisingRange = 0;
int laadpaal_targetCharge = 0;

// heat pump
bool compressor = LOW;
int hotWaterTemp = 0;
int XDHW = 0;
String heatPumpTime = "00:00";

// mower
int mowerBattery = 0;
bool mowerState = LOW;

//venti
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

// energy
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

// music
String keuken_title = "";
String keuken_channel = "Spotify";
int keuken_volume = 0;
bool keuken_state = LOW;
String speelkamer_title = "";
String speelkamer_channel = "Spotify";
int speelkamer_volume = 0;
bool speelkamer_state = LOW;

// general
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
// ** energy **
bool newEnergyGraph = LOW;
bool newEnergyVal = LOW;
bool newBlxmInfo = LOW;
// ** car **
bool newCarInfo = LOW;
// ** music **
bool newKeukenChan = LOW;
bool newSpeelkamerChan = LOW;
bool newKeuken = LOW;
bool newSpeelkamer = LOW;
bool newMusic = LOW;
// ** lights **
bool maanlampChange = LOW;
bool bergingChange = LOW;
bool sfeerlichtjesChange = LOW;
bool raamversieringChange = LOW;
bool groteBolChange = LOW;
bool glazenBolChange = LOW;
// ** venti **
bool newVentiV = LOW;
bool newVentiImg = LOW;

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
    while (1)
      ;
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
  mqttClient.subscribe("lights/#");
  mqttClient.subscribe("laadpaal/#");
  mqttClient.subscribe("energy/#");
  mqttClient.subscribe("music/#");
  mqttClient.subscribe("afval/#");
  mqttClient.subscribe("ventilation/#");
  mqttClient.subscribe("heatPump/#");
  mqttClient.subscribe("mower/#");
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
    lastTouch = millis();                                    // register last touch
    Serial.print("Contacts:");
    Serial.println(contacts);
    //record the x,y coordinates
    for (uint8_t i = 0; i < contacts; i++) {
      touch_x = 800 - points[i].y;
      touch_y = points[i].x;
    }
    if ((touch_x < 100 && page != "home" && page.indexOf("/") == -1) || page == "blackScreen") {  // Return
      page = "home";
      print();
      // printImg();
    } else if (page == "home") {
      if (touch_x > 700 && touch_y > 40) {
        page = "home2";
        print2();
      } else if (touch_y < carCo[1] + carCo[3] && touch_y > carCo[1] && touch_x < carCo[0] + carCo[2] && touch_x > carCo[0] && (carCo[4] == 0 || carCo[4] == 1)) {  // Car clicked
        page = "detailCar";
        dashboard.detailCar(laadpaal_battery, laadpaal_chargingPower, laadpaal_chargingSpeed, laadpaal_targetCharge, laadpaal_cruisingRange);
      } else if (touch_y < lightCo[1] + lightCo[3] && touch_y > lightCo[1] && touch_x < lightCo[0] + lightCo[2] && touch_x > lightCo[0] && (lightCo[4] == 0 || lightCo[4] == 1)) {  // Lights clicked
        page = "detailLights";
        dashboard.fillScreen(AllBackg);
        dashboard.detailLights(lights_sfeerlichtjes, lights_groteBol, lights_glazenBol, lights_berging, lights_maanlamp, lights_raamversiering);
        dashboard.sfeerlichtjesImg(SCREEN_W / 6, SCREEN_H / 4 + 16, lights_sfeerlichtjes);
        dashboard.bolImg(3 * SCREEN_W / 6, SCREEN_H / 4 + 16, lights_groteBol);
        dashboard.bolImg(5 * SCREEN_W / 6, SCREEN_H / 4 + 16, lights_glazenBol);
        dashboard.moonlampImg(SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, lights_maanlamp);
        dashboard.sfeerlichtjesImg(3 * SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, lights_raamversiering);
      } else if (touch_y < energyCo[1] + energyCo[3] && touch_y > energyCo[1] && touch_x < energyCo[0] + energyCo[2] && touch_x > energyCo[0] && (energyCo[4] == 0 || energyCo[4] == 1)) {  // Energy clicked
        page = "energy";
        dashboard.fillScreen(AllBackg);
        dashboard.energy(importArr, productionArr, consumptionArr, longImportArr, longProductionArr, longConsumptionArr, currentImport, currentProduction, currentConsumption, timeArr, longTimeArr, energy_state, energy_longGraph, energy_roundState, drawImport, drawProduction, drawConsumption);
      } else if (touch_y < musicCo[1] + musicCo[3] && touch_y > musicCo[1] && touch_x < musicCo[0] + musicCo[2] && touch_x > musicCo[0] && (musicCo[4] == 0 || musicCo[4] == 1)) {  // Music clicked
        page = "music";
        dashboard.fillScreen(AllBackg);
        dashboard.music(keuken_channel, keuken_title, keuken_state, speelkamer_channel, speelkamer_title, speelkamer_state);
      } else if (touch_y < heatPumpCo[1] + heatPumpCo[3] && touch_y > heatPumpCo[1] && touch_x < heatPumpCo[0] + heatPumpCo[2] && touch_x > heatPumpCo[0] && (heatPumpCo[4] == 0 || heatPumpCo[4] == 1)) {  // Music clicked
        page = "heatPump";
        dashboard.fillScreen(AllBackg);
        dashboard.heatPump(compressor, hotWaterTemp, XDHW);
      } else if (touch_y < mowerCo[1] + mowerCo[3] && touch_y > mowerCo[1] && touch_x < mowerCo[0] + mowerCo[2] && touch_x > mowerCo[0] && (mowerCo[4] == 0 || mowerCo[4] == 1)) {  // Music clicked
        page = "mower";
        dashboard.fillScreen(AllBackg);
        dashboard.mower(mowerBattery, mowerState);
      } else if (touch_y < ventiCo[1] + ventiCo[3] && touch_y > ventiCo[1] && touch_x < ventiCo[0] + ventiCo[2] && touch_x > ventiCo[0] && (ventiCo[4] == 0 || ventiCo[4] == 1)) {  // Ventilation clicked
        page = "venti";
        dashboard.fillScreen(AllBackg);
        dashboard.homeEmpty(0, 0, 798, 239);
        dashboard.homeEmpty(0, 239, 798, 239);
        dashboard.ventilationV(keuken_boost, kelder_boost, keuken_CO2, kelder_VOC, keuken_humidity, kelder_humidity, keuken_quality, kelder_quality, keuken_boostRemaining, kelder_boostRemaining);
        dashboard.ventilationImg(keuken_boost, kelder_boost);
      } else if (touch_x > 746 && touch_x < 800 && touch_y > 0 && touch_y < 44) {  // Settings clicked
        page = "settings";
        dashboard.settings(darkMode, lastReset);
      }
    } else if (page == "home2") {
      if (touch_y < carCo[1] + carCo[3] && touch_y > carCo[1] && touch_x < carCo[0] + carCo[2] && touch_x > carCo[0] && (carCo[4] == 0 || carCo[4] == 2)) {  // Car clicked
        page = "detailCar";
        dashboard.detailCar(laadpaal_battery, laadpaal_chargingPower, laadpaal_chargingSpeed, laadpaal_targetCharge, laadpaal_cruisingRange);
      } else if (touch_y < lightCo[1] + lightCo[3] && touch_y > lightCo[1] && touch_x < lightCo[0] + lightCo[2] && touch_x > lightCo[0] && (lightCo[4] == 0 || lightCo[4] == 2)) {  // Lights clicked
        page = "detailLights";
        dashboard.fillScreen(AllBackg);
        dashboard.detailLights(lights_sfeerlichtjes, lights_groteBol, lights_glazenBol, lights_berging, lights_maanlamp, lights_raamversiering);
        dashboard.sfeerlichtjesImg(SCREEN_W / 6, SCREEN_H / 4 + 16, lights_sfeerlichtjes);
        dashboard.bolImg(3 * SCREEN_W / 6, SCREEN_H / 4 + 16, lights_groteBol);
        dashboard.bolImg(5 * SCREEN_W / 6, SCREEN_H / 4 + 16, lights_glazenBol);
        dashboard.moonlampImg(SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, lights_maanlamp);
        dashboard.sfeerlichtjesImg(3 * SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, lights_raamversiering);
      } else if (touch_y < energyCo[1] + energyCo[3] && touch_y > energyCo[1] && touch_x < energyCo[0] + energyCo[2] && touch_x > energyCo[0] && (energyCo[4] == 0 || energyCo[4] == 2)) {  // Energy clicked
        page = "energy";
        dashboard.fillScreen(AllBackg);
        dashboard.energy(importArr, productionArr, consumptionArr, longImportArr, longProductionArr, longConsumptionArr, currentImport, currentProduction, currentConsumption, timeArr, longTimeArr, energy_state, energy_longGraph, energy_roundState, drawImport, drawProduction, drawConsumption);
      } else if (touch_y < heatPumpCo[1] + heatPumpCo[3] && touch_y > heatPumpCo[1] && touch_x < heatPumpCo[0] + heatPumpCo[2] && touch_x > heatPumpCo[0] && (heatPumpCo[4] == 0 || heatPumpCo[4] == 2)) {  // Music clicked
        page = "heatPump";
        dashboard.fillScreen(AllBackg);
        dashboard.heatPump(compressor, hotWaterTemp, XDHW);
      } else if (touch_y < mowerCo[1] + mowerCo[3] && touch_y > mowerCo[1] && touch_x < mowerCo[0] + mowerCo[2] && touch_x > mowerCo[0] && (mowerCo[4] == 0 || mowerCo[4] == 2)) {  // Music clicked
        page = "mower";
        dashboard.fillScreen(AllBackg);
        dashboard.mower(mowerBattery, mowerState);
      } else if (touch_y < musicCo[1] + musicCo[3] && touch_y > musicCo[1] && touch_x < musicCo[0] + musicCo[2] && touch_x > musicCo[0] && (musicCo[4] == 0 || musicCo[4] == 2)) {  // Music clicked
        page = "music";
        dashboard.fillScreen(AllBackg);
        dashboard.music(keuken_channel, keuken_title, keuken_state, speelkamer_channel, speelkamer_title, speelkamer_state);
      } else if (touch_y < ventiCo[1] + ventiCo[3] && touch_y > ventiCo[1] && touch_x < ventiCo[0] + ventiCo[2] && touch_x > ventiCo[0] && (ventiCo[4] == 0 || ventiCo[4] == 2)) {  // Ventilation clicked
        page = "venti";
        dashboard.fillScreen(AllBackg);
        dashboard.homeEmpty(0, 0, 799, 239);
        dashboard.homeEmpty(0, 239, 799, 239);
        dashboard.ventilationV(keuken_boost, kelder_boost, keuken_CO2, kelder_VOC, keuken_humidity, kelder_humidity, keuken_quality, kelder_quality, keuken_boostRemaining, kelder_boostRemaining);
        dashboard.ventilationImg(keuken_boost, kelder_boost);
      } else if (touch_x > 746 && touch_x < 800 && touch_y > 0 && touch_y < 44) {  // Settings clicked
        page = "settings";
        dashboard.settings(darkMode, lastReset);
      }
    } else if (page == "detailLights") {
      if (touch_x < 267 && touch_x > 100 && touch_y < 262 && touch_y > 44) {  // Sfeerlichtjes
        mqttClient.beginMessage("lights/sfeerlichtjes");
        mqttClient.print("Switch");
        mqttClient.endMessage();
      }
      if (touch_x > 267 && touch_x < 533 && touch_y < 262 && touch_y > 44) {  // Grote bol
        contacts = touchDetector.getTouchPoints(points);
        while (contacts == 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        while (contacts > 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
          // if (lastTouch + 600 < millis()) {
          //   break;
          // }
        }
        if (lastTouch + 600 < millis()) {
          page = "detailLights/groteBol";
          dashboard.lightSw();
          dashboard.lightBr(lights_groteBol_br);
        } else {
          Serial.println("Tap");
          mqttClient.beginMessage("lights/groteBol");
          mqttClient.print("Switch");
          mqttClient.endMessage();
        }
      }
      if (touch_x > 533 && touch_x < 800 && touch_y < 262 && touch_y > 44) {  // Glazen bol
        contacts = touchDetector.getTouchPoints(points);
        while (contacts == 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        while (contacts > 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
          // if (lastTouch + 600 < millis()) {
          //   break;
          // }
        }
        if (lastTouch + 600 < millis()) {
          page = "detailLights/glazenBol";
          dashboard.lightSw();
          dashboard.lightBr(lights_glazenBol_br);
        } else {
          Serial.println("Tap");
          mqttClient.beginMessage("lights/glazenBol");
          mqttClient.print("Switch");
          mqttClient.endMessage();
        }
      }
      if (touch_x < 400 && touch_x > 100 && touch_y < 480 && touch_y > 262) {  // Maanlamp
        contacts = touchDetector.getTouchPoints(points);
        while (contacts == 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
        }
        while (contacts > 0) {
          contacts = touchDetector.getTouchPoints(points);
          delay(20);
          // if (lastTouch + 600 < millis()) {
          //   break;
          // }
        }
        if (lastTouch + 600 < millis()) {
          page = "detailLights/maanlamp";
          dashboard.lightSw();
          dashboard.lightBr(lights_maanlamp_br);
        } else {
          Serial.println("Tap");
          mqttClient.beginMessage("lights/maanlamp");
          mqttClient.print("Switch");
          mqttClient.endMessage();
        }
      }
      // if (touch_x > 400 && touch_x < 800 && touch_y < 480 && touch_y > 262) {  // Berging
      //   contacts = touchDetector.getTouchPoints(points);
      //   while (contacts == 0) {
      //     contacts = touchDetector.getTouchPoints(points);
      //     delay(20);
      //   }
      //   while (contacts > 0) {
      //     contacts = touchDetector.getTouchPoints(points);
      //     delay(20);
      //     if (lastTouch + 600 < millis()) {
      //       break;
      //     }
      //   }
      //   if (lastTouch + 600 < millis()) {
      //     page = "detailLights/berging";
      //     dashboard.lightSw();
      //     dashboard.lightBr(lights_berging_br);
      //   } else {
      //     Serial.println("Tap");
      //     mqttClient.beginMessage("lights/berging");
      //     mqttClient.print("Switch");
      //     mqttClient.endMessage();
      //   }
      // }
      if (touch_x > 400 && touch_x < 800 && touch_y < 480 && touch_y > 262) {  // Raamversiering
        mqttClient.beginMessage("lights/raamversiering");
        mqttClient.print("Switch");
        mqttClient.endMessage();
      }

    } else if (page == "settings") {
      if (touch_x < 360 && touch_x > 285 && touch_y < 140 && touch_y > 109) {  // darkMode clicked
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
      } else if (touch_x < 170 && touch_x > 130 && touch_y < 462 && touch_y > 422) {
        Serial.println("RESET!!!!!");
        NVIC_SystemReset();
      }
    } else if (page.startsWith("detailLights/") == HIGH) {
      String k = page;
      k.replace("detailLights/", "");
      Serial.println(k);
      if ((touch_x < 300 || touch_x > 500) || (touch_y < 40 || touch_y > 440)) {  // Return
        Serial.println("return");
        page = "detailLights";
        dashboard.fillScreen(AllBackg);
        dashboard.detailLights(lights_sfeerlichtjes, lights_groteBol, lights_glazenBol, lights_berging, lights_maanlamp, lights_raamversiering);
        dashboard.detailLights(lights_sfeerlichtjes, lights_groteBol, lights_glazenBol, lights_berging, lights_maanlamp, lights_raamversiering);
        dashboard.sfeerlichtjesImg(SCREEN_W / 6, SCREEN_H / 4 + 16, lights_sfeerlichtjes);
        dashboard.bolImg(3 * SCREEN_W / 6, SCREEN_H / 4 + 16, lights_groteBol);
        dashboard.bolImg(5 * SCREEN_W / 6, SCREEN_H / 4 + 16, lights_glazenBol);
        dashboard.moonlampImg(SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, lights_maanlamp);
        dashboard.sfeerlichtjesImg(3 * SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, lights_raamversiering);
      }
      if (touch_x > 300 && touch_x < 500 && touch_y > 340 && touch_y < 440) {  // O/I button clicked
        mqttClient.beginMessage("lights/" + k);
        mqttClient.print("Switch");
        mqttClient.endMessage();
      }
    } else if (page.startsWith("music")) {
      if (page == "music") {
        if (touch_x < 350 && touch_x > 50 && touch_y < 350 && touch_y > 50 && (touch_x < 250 || touch_x > 350 || touch_y < 300 || touch_y > 400)) {  // Keuken icon clicked
          page = "music/keuken";
          dashboard.chooseChannel();
          newMusic = LOW;
          newKeukenChan = LOW;
          newSpeelkamerChan = LOW;
          newSpeelkamer = LOW;
          newKeuken = LOW;
        }
        if (touch_x > 250 && touch_x < 350 && touch_y > 300 && touch_y < 400) {  // Keuken button clicked
          mqttClient.beginMessage("music/keuken");
          mqttClient.print("Switch");
          mqttClient.endMessage();
        }
        if (touch_x < 750 && touch_x > 450 && touch_y < 350 && touch_y > 50 && (touch_x < 650 || touch_x > 750 || touch_y < 300 || touch_y > 400)) {  // Speelkamer icon clicked
          page = "music/speelkamer";
          dashboard.chooseChannel();
          newMusic = LOW;
        }
        if (touch_x > 650 && touch_x < 750 && touch_y > 300 && touch_y < 400) {  // Speelkamer button clicked
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
        dashboard.music(keuken_channel, keuken_title, keuken_state, speelkamer_channel, speelkamer_title, speelkamer_state);
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
        dashboard.music(keuken_channel, keuken_title, keuken_state, speelkamer_channel, speelkamer_title, speelkamer_state);
      }
    } else if (page == "energy") {
      newEnergyGraph = HIGH;
      if (touch_x > 440 && touch_x < 800 && touch_y > 30 && touch_y < 94) {
        energy_longGraph = !energy_longGraph;
      } else if (touch_x > 440 && touch_x < 480) {
        if (touch_y > 310 && touch_y < 350) {
          drawConsumption = !drawConsumption;
        } else if (touch_y > 355 && touch_y < 395) {
          drawImport = !drawImport;
        } else if (touch_y > 400 && touch_y < 440) {
          drawProduction = !drawProduction;
        }
      } else if (!energy_longGraph) {
        energy_roundState = !energy_roundState;
      } else {
        newEnergyGraph = LOW;
      }
    } else if (page == "venti") {
      if (keuken_boost) {
        if (touch_x > 595 && touch_x < 765 && touch_y > 35 && touch_y < 205) {  // Keuken 1u
          mqttClient.beginMessage("ventilation/keuken/control");
          mqttClient.print("Stop");
          mqttClient.endMessage();
        }
      } else {
        if (touch_x > 390 && touch_x < 560 && touch_y > 35 && touch_y < 205) {  // Keuken 1u
          mqttClient.beginMessage("ventilation/keuken/control");
          mqttClient.print(60);
          mqttClient.endMessage();
        }
        if (touch_x > 595 && touch_x < 765 && touch_y > 35 && touch_y < 205) {  // Keuken 1u
          mqttClient.beginMessage("ventilation/keuken/control");
          mqttClient.print(180);
          mqttClient.endMessage();
        }
      }
      if (kelder_boost) {
        if (touch_x > 595 && touch_x < 765 && touch_y > 275 && touch_y < 445) {  // Keuken 1u
          mqttClient.beginMessage("ventilation/kelder/control");
          mqttClient.print("Stop");
          mqttClient.endMessage();
        }
      } else {
        if (touch_x > 390 && touch_x < 560 && touch_y > 275 && touch_y < 445) {  // Keuken 1u
          mqttClient.beginMessage("ventilation/kelder/control");
          mqttClient.print(60);
          mqttClient.endMessage();
        }
        if (touch_x > 595 && touch_x < 765 && touch_y > 275 && touch_y < 445) {  // Keuken 1u
          mqttClient.beginMessage("ventilation/kelder/control");
          mqttClient.print(180);
          mqttClient.endMessage();
        }
      }
    } else if (page == "mower") {
      if (mowerState) {
        if (touch_x > 264 && touch_x < 464 && touch_y > 360 && touch_y < 440) {  // Return
          mqttClient.beginMessage("mower/set");
          mqttClient.print("Return");
          mqttClient.endMessage();
        }
        if (touch_x > 40 && touch_x < 240 && touch_y > 360 && touch_y < 440) {  // Pause
          mqttClient.beginMessage("mower/set");
          mqttClient.print("Pause");
          mqttClient.endMessage();
        }
      } else {
        if (touch_x > 40 && touch_x < 464 && touch_y > 360 && touch_y < 440) {  // Start
          mqttClient.beginMessage("mower/set");
          mqttClient.print("Start");
          mqttClient.endMessage();
        }
      }
    }
    lastTouch = millis();
  }
  if (sfeerlichtjesChange || groteBolChange || glazenBolChange || maanlampChange || bergingChange || raamversieringChange) {
    if (page.startsWith("detailLights")) {
      // Serial.println("Refresh");
      if (sfeerlichtjesChange) {
        dashboard.sfeerlichtjesImg(SCREEN_W / 6, SCREEN_H / 4 + 16, lights_sfeerlichtjes);
      }
      if (groteBolChange) {
        dashboard.bolImg(3 * SCREEN_W / 6, SCREEN_H / 4 + 16, lights_groteBol);
      }
      if (glazenBolChange) {
        dashboard.bolImg(5 * SCREEN_W / 6, SCREEN_H / 4 + 16, lights_glazenBol);
      }
      if (maanlampChange) {
        dashboard.moonlampImg(SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, lights_maanlamp);
      }
      if (raamversieringChange) {
        dashboard.sfeerlichtjesImg(3 * SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, lights_raamversiering);
      }
      if (page.startsWith("detailLights/") == HIGH) {
        dashboard.lightSw();
        if (page.endsWith("groteBol") == HIGH) {
          dashboard.lightBr(lights_groteBol_br);
        } else if (page.endsWith("glazenBol") == HIGH) {
          dashboard.lightBr(lights_glazenBol_br);
        } else if (page.endsWith("berging") == HIGH) {
          dashboard.lightBr(lights_berging_br);
        } else if (page.endsWith("maanlamp") == HIGH) {
          dashboard.lightBr(lights_maanlamp_br);
        }
      }
    }
    sfeerlichtjesChange = LOW;
    groteBolChange = LOW;
    glazenBolChange = LOW;
    maanlampChange = LOW;
    bergingChange = LOW;
    raamversieringChange = LOW;
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
  if (newMusic || newKeukenChan || newSpeelkamerChan || newSpeelkamer || newKeuken) {
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
        dashboard.musicV(keuken_channel, keuken_title, speelkamer_channel, speelkamer_title);
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
  if (newEnergyVal || newEnergyGraph || newBlxmInfo) {
    if (page == "energy") {
      if (newEnergyVal) {
        dashboard.energyV(currentImport, currentProduction, currentConsumption);
      }
      if (newBlxmInfo) {
        dashboard.bliksem(450 + 340 / 4, 124 + 156 / 2, energy_state);
      }
      if (newEnergyGraph) {
        dashboard.energyGraph(importArr, productionArr, consumptionArr, longImportArr, longProductionArr, longConsumptionArr, timeArr, longTimeArr, energy_longGraph, energy_roundState, drawImport, drawProduction, drawConsumption);
      }
      // Serial.println("Refresh");
    }
    if (page == "home" && newBlxmInfo) {
      if (energyCo[4] == 0 || energyCo[4] == 1) { dashboard.bliksem(0 + 399 / 4, 262 + 217 / 2, energy_state); }
    } else if (page == "home2" && newBlxmInfo) {
      if (energyCo[4] == 0 || energyCo[4] == 2) { dashboard.bliksem(0 + 399 / 4, 262 + 217 / 2, energy_state); }
    }
    newBlxmInfo = LOW;
    newEnergyVal = LOW;
    newEnergyGraph = LOW;
  }
  if (newVentiV || newVentiImg) {
    if (page == "venti") {
      if (newVentiV) {
        dashboard.ventilationV(keuken_boost, kelder_boost, keuken_CO2, kelder_VOC, keuken_humidity, kelder_humidity, keuken_quality, kelder_quality, keuken_boostRemaining, kelder_boostRemaining);
      }
      if (newVentiImg) {
        dashboard.ventilationImg(keuken_boost, kelder_boost);
      }
    }
    newVentiV = LOW;
    newVentiImg = LOW;
  }
  if (WiFi.status() != WL_CONNECTED || lastEnergyNewValue + 15000 < millis()) {
    NVIC_SystemReset();
  }
  if (night && lastTouch + 60000 < millis()) {  // Shut down at night
    dashboard.fillScreen(BLACK);
    page = "blackScreen";
  } else if (lastTouch + 60000 < millis() && page != "home" && page.indexOf("/") == -1) {  // Return to home after a while
    page = "home";
    print();
    // printImg();
  }
  delay(10);
}
void onMqttMessage(int messageSize) {
  String topic = mqttClient.messageTopic();
  String message = mqttClient.readString();
  if (topic.startsWith("mower")) {  // Print topic & message
    Serial.print("'");
    Serial.print(topic);
    Serial.print("'   '");
    Serial.print(message);
    Serial.println("'");
    Serial.println();
  }
  ///////////////////////////////////////////////////////// home/#
  if (topic == "home/time") {
    message += "L";
    message.replace(":00L", "");
    realTime = message;
    message.replace(":", "");
    realTimeInt = message.toInt();
    if (page == "home") {
      dashboard.printTime(realTime);
    }
    if (realTimeInt > nightStart || realTimeInt < nightEnd) {
      night = HIGH;
    } else {
      night = LOW;
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
  /////////////////////////////////////////////////////// lights/#
  if (topic.startsWith("lights/") == HIGH) {
    if (message != "Switch") {
      if (topic == "lights/sfeerlichtjes") {
        if (message == "on") {
          lights_sfeerlichtjes = 1;
        } else if (message == "off") {
          lights_sfeerlichtjes = 0;
        }
        sfeerlichtjesChange = HIGH;
      }
      if (topic == "lights/groteBol") {
        if (message == "") {
          lights_groteBol_br = 0;
          lights_groteBol = 0;
        } else {
          lights_groteBol_br = message.toInt() * 100 / 255;
          lights_groteBol = 1;
        }
        groteBolChange = HIGH;
      }
      if (topic == "lights/glazenBol") {
        if (message == "") {
          lights_glazenBol_br = 0;
          lights_glazenBol = 0;
        } else {
          lights_glazenBol_br = message.toInt() * 100 / 255;
          lights_glazenBol = 1;
        }
        glazenBolChange = HIGH;
      }
      if (topic == "lights/berging") {
        if (message == "") {
          lights_berging_br = 0;
          lights_berging = 0;
        } else {
          lights_berging_br = message.toInt() * 100 / 255;
          lights_berging = 1;
        }
        bergingChange = HIGH;
      }
      if (topic == "lights/maanlamp") {
        if (message == "") {
          lights_maanlamp_br = 0;
          lights_maanlamp = 0;
        } else {
          lights_maanlamp_br = message.toInt() * 100 / 255;
          lights_maanlamp = 1;
        }
        maanlampChange = HIGH;
      }
      if (topic == "lights/raamversiering") {
        if (message == "on") {
          lights_raamversiering = 1;
        } else if (message == "off") {
          lights_raamversiering = 0;
        }
        raamversieringChange = HIGH;
      }
    }
  }
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
      } else if (currentImport < 7000) {
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
    }
    lastEnergyNewValue = millis();
  }
  if (topic.startsWith("music/") && message != "Switch" && message != "Up" && message != "Down" && message != "Next" && message != "Last") {
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
        } else if (message == "VRT Studio Brussel Vuurland" || message == "Studio Brussel Vuurland" || message == "Vuurland") {
          message = "Vuurland";
        } else if (message == "VRT NWS") {
        } else if (message == "Radio 2 vlaams-brabant" || message == "Radio 2") {
          message = "Radio 2";
        } else if (message == "Spotify Connect" || message == "" || message == "Spotify") {
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
        } else if (message == "VRT Studio Brussel Vuurland" || message == "Studio Brussel Vuurland" || message == "Vuurland") {
          message = "Vuurland";
        } else if (message == "VRT NWS") {
        } else if (message == "Radio 2 vlaams-brabant" || message == "Radio 2") {
          message = "Radio 2";
        } else if (message == "Spotify Connect" || message == "" || message == "Spotify") {
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
  if (topic.startsWith("afval")) {
    //   Wastetype:  Rest = 0; GFT = 1;  PMD = 2;  Papier = 3;  Rest + GFT = 4; None = 5
    musicCo[2] = 199;  // { 400, 262, 199, 217 };
    wasteCo[0] = 600;  // { 600, 262, 199, 217 };
    if (message.indexOf("GFT") != -1) {
      if (message.indexOf("Restafval") != -1 || message.indexOf("restafval") != -1) {
        wasteType = 4;
      } else {
        wasteType = 1;
      }
    } else if (message.indexOf("Restafval") != -1 || message.indexOf("restafval") != -1) {
      wasteType = 0;
    } else if (message.indexOf("PMD") != -1) {
      wasteType = 2;
    } else if (message.indexOf("Papier") != -1 || message.indexOf("papier") != -1) {
      wasteType = 3;
    } else {
      wasteType = 5;
      musicCo[2] = 399;  // { 400, 262, 399, 217 };
      wasteCo[0] = 900;  // { 900, 262, 199, 217 };
    }
    print();
    printImg();
  }
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
      if (page == "home") {
        dashboard.printHeatPumpTime(heatPumpTime);
      }
    }
  }
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
}
void print() {
  dashboard.home();
  dashboard.printTime(realTime);
  dashboard.printTemp(outDoorTemp);
  dashboard.printHeatPumpTime(heatPumpTime);
  dashboard.printWaterTemp(hotWaterTemp);
  if (lightCo[4] == 0 || lightCo[4] == 1) { dashboard.homeEmpty(lightCo[0], lightCo[1], lightCo[2], lightCo[3]); }
  if (carCo[4] == 0 || carCo[4] == 1) { dashboard.homeCar(carCo[0], carCo[1], carCo[2], carCo[3], laadpaal_battery, laadpaal_chargingPower, laadpaal_targetCharge); }
  if (musicCo[4] == 0 || musicCo[4] == 1) { dashboard.homeEmpty(musicCo[0], musicCo[1], musicCo[2], musicCo[3]); }
  if (ventiCo[4] == 0 || ventiCo[4] == 1) { dashboard.homeEmpty(ventiCo[0], ventiCo[1], ventiCo[2], ventiCo[3]); }
  if (energyCo[4] == 0 || energyCo[4] == 1) { dashboard.homeEnergy(energyCo[0], energyCo[1], energyCo[2], energyCo[3], energy_state, currentImport); }
  if (wasteCo[4] == 0 || wasteCo[4] == 1) { dashboard.homeEmpty(wasteCo[0], wasteCo[1], wasteCo[2], wasteCo[3]); }
  if (heatPumpCo[4] == 0 || heatPumpCo[4] == 1) { dashboard.homeHeatPump(heatPumpCo[0], heatPumpCo[1], heatPumpCo[2], heatPumpCo[3], hotWaterTemp); }
  if (mowerCo[4] == 0 || mowerCo[4] == 1) { dashboard.homeEmpty(mowerCo[0], mowerCo[1], mowerCo[2], mowerCo[3]); }
  printImg();
}
void printV() {
  if (carCo[4] == 0 || carCo[4] == 1) { dashboard.homeCarV(carCo[0], carCo[1], carCo[2], carCo[3], laadpaal_battery, laadpaal_chargingPower, laadpaal_targetCharge); }
  if (energyCo[4] == 0 || energyCo[4] == 1) { dashboard.homeEnergyV(energyCo[0], energyCo[1], energyCo[2], energyCo[3], energy_state, currentImport); }
  if (heatPumpCo[4] == 0 || heatPumpCo[4] == 1) { dashboard.homeHeatPumpV(heatPumpCo[0], heatPumpCo[1], heatPumpCo[2], heatPumpCo[3], hotWaterTemp); }
}
void printImg() {
  if (lightCo[4] == 0 || lightCo[4] == 1) { dashboard.homeLightsImg(lightCo[0], lightCo[1], lightCo[2], lightCo[3]); }
  if (carCo[4] == 0 || carCo[4] == 1) { dashboard.homeCarImg(carCo[0], carCo[1], carCo[2], carCo[3]); }
  if (energyCo[4] == 0 || energyCo[4] == 1) { dashboard.homeEnergyImg(energyCo[0], energyCo[1], energyCo[2], energyCo[3], energy_state); }
  if (musicCo[4] == 0 || musicCo[4] == 1) { dashboard.homeMusicImg(musicCo[0], musicCo[1], musicCo[2], musicCo[3]); }
  if (ventiCo[4] == 0 || ventiCo[4] == 1) { dashboard.homeVentiImg(ventiCo[0], ventiCo[1], ventiCo[2], ventiCo[3]); }
  if (wasteCo[4] == 0 || wasteCo[4] == 1) { dashboard.homeWasteImg(wasteCo[0], wasteCo[1], wasteCo[2], wasteCo[3], wasteType); }
  if (heatPumpCo[4] == 0 || heatPumpCo[4] == 1) { dashboard.homeHeatPumpImg(heatPumpCo[0], heatPumpCo[1], heatPumpCo[2], heatPumpCo[3], compressor); }
  if (mowerCo[4] == 0 || mowerCo[4] == 1) { dashboard.homeMowerImg(mowerCo[0], mowerCo[1], mowerCo[2], mowerCo[3]); }
}
void print2() {
  dashboard.home();
  dashboard.printTime(realTime);
  dashboard.printTemp(outDoorTemp);
  dashboard.printHeatPumpTime(heatPumpTime);
  dashboard.printWaterTemp(hotWaterTemp);
  if (lightCo[4] == 0 || lightCo[4] == 2) { dashboard.homeEmpty(lightCo[0], lightCo[1], lightCo[2], lightCo[3]); }
  if (carCo[4] == 0 || carCo[4] == 2) { dashboard.homeCar(carCo[0], carCo[1], carCo[2], carCo[3], laadpaal_battery, laadpaal_chargingPower, laadpaal_targetCharge); }
  if (musicCo[4] == 0 || musicCo[4] == 2) { dashboard.homeEmpty(musicCo[0], musicCo[1], musicCo[2], musicCo[3]); }
  if (ventiCo[4] == 0 || ventiCo[4] == 2) { dashboard.homeEmpty(ventiCo[0], ventiCo[1], ventiCo[2], ventiCo[3]); }
  if (energyCo[4] == 0 || energyCo[4] == 2) { dashboard.homeEnergy(energyCo[0], energyCo[1], energyCo[2], energyCo[3], energy_state, currentImport); }
  if (wasteCo[4] == 0 || wasteCo[4] == 2) { dashboard.homeEmpty(wasteCo[0], wasteCo[1], wasteCo[2], wasteCo[3]); }
  if (heatPumpCo[4] == 0 || heatPumpCo[4] == 2) { dashboard.homeHeatPump(heatPumpCo[0], heatPumpCo[1], heatPumpCo[2], heatPumpCo[3], hotWaterTemp); }
  if (mowerCo[4] == 0 || mowerCo[4] == 2) { dashboard.homeEmpty(mowerCo[0], mowerCo[1], mowerCo[2], mowerCo[3]); }
  print2Img();
}
void print2V() {
  if (carCo[4] == 0 || carCo[4] == 2) { dashboard.homeCarV(carCo[0], carCo[1], carCo[2], carCo[3], laadpaal_battery, laadpaal_chargingPower, laadpaal_targetCharge); }
  if (energyCo[4] == 0 || energyCo[4] == 2) { dashboard.homeEnergyV(energyCo[0], energyCo[1], energyCo[2], energyCo[3], energy_state, currentImport); }
  if (heatPumpCo[4] == 0 || heatPumpCo[4] == 2) { dashboard.homeHeatPumpV(heatPumpCo[0], heatPumpCo[1], heatPumpCo[2], heatPumpCo[3], hotWaterTemp); }
}
void print2Img() {
  if (lightCo[4] == 0 || lightCo[4] == 2) { dashboard.homeLightsImg(lightCo[0], lightCo[1], lightCo[2], lightCo[3]); }
  if (carCo[4] == 0 || carCo[4] == 2) { dashboard.homeCarImg(carCo[0], carCo[1], carCo[2], carCo[3]); }
  if (energyCo[4] == 0 || energyCo[4] == 2) { dashboard.homeEnergyImg(energyCo[0], energyCo[1], energyCo[2], energyCo[3], energy_state); }
  if (musicCo[4] == 0 || musicCo[4] == 2) { dashboard.homeMusicImg(musicCo[0], musicCo[1], musicCo[2], musicCo[3]); }
  if (ventiCo[4] == 0 || ventiCo[4] == 2) { dashboard.homeVentiImg(ventiCo[0], ventiCo[1], ventiCo[2], ventiCo[3]); }
  if (wasteCo[4] == 0 || wasteCo[4] == 2) { dashboard.homeWasteImg(wasteCo[0], wasteCo[1], wasteCo[2], wasteCo[3], wasteType); }
  if (heatPumpCo[4] == 0 || heatPumpCo[4] == 2) { dashboard.homeHeatPumpImg(heatPumpCo[0], heatPumpCo[1], heatPumpCo[2], heatPumpCo[3], compressor); }
  if (mowerCo[4] == 0 || mowerCo[4] == 2) { dashboard.homeMowerImg(mowerCo[0], mowerCo[1], mowerCo[2], mowerCo[3]); }
}
