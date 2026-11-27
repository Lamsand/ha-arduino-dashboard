#include "energy.h"
#include "Arduino.h"
#include "Colors.h"
#include "Dashboard.h"
#include "config.h"

#ifdef ENABLE_ENERGY
void Dashboard::bliksem(int xInput, int yInput, int state) {
  switch (state) {
  case 0:
    if (publicBackg == BLACK) {
      file = sd.open("Energy/blxmGD.bin");
    } else {
      file = sd.open("Energy/blxmGL.bin");
    }
    break;
  case 1:
    if (publicBackg == BLACK) {
      file = sd.open("Energy/blxmOD.bin");
    } else {
      file = sd.open("Energy/blxmOL.bin");
    }
    break;
  case 2:
    if (publicBackg == BLACK) {
      file = sd.open("Energy/blxmRD.bin");
    } else {
      file = sd.open("Energy/blxmRL.bin");
    }
    break;
  }
  if (!file)
    return;
  file.seek(4); // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 132;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q); // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q); // Draw one row at a time
  file.close();
}

void Dashboard::homeEnergy() {
  display.fillRoundRect(energy.x + 1, energy.y + 1, energy.w - 1, energy.h - 1,
                        10, publicBackg);
  // bliksem(energy.x + energy.w / 4, y + energy.h / 2, energy.state);
  if (energy.currentImport <= -1000) {
    display.setCursor(energy.x + energy.w / 2 - 36,
                      energy.y + energy.h / 2 - 24);
  } else {
    display.setCursor(energy.x + energy.w / 2, energy.y + energy.h / 2 - 24);
  }
  display.setTextColor(publicTEXT);
  display.setTextSize(6);
  if (energy.currentImport != 99999) {
    display.print(energy.currentImport);
  } else {
    display.print("N/A");
  }
  display.print("W");
}
void Dashboard::homeEnergyV() {
  display.fillRect(energy.x + energy.w / 2, energy.y + energy.h / 4,
                   energy.w / 2, energy.h / 2, publicBackg);
  display.fillRect(energy.x + energy.w, energy.y + energy.h / 4, 1,
                   energy.h / 2, publicTEXT);
  display.fillRect(energy.x + energy.w + 2, energy.y + energy.h / 4, 30,
                   energy.h / 2, publicBackg);
  display.fillRect(energy.x + energy.w / 2 - 36, energy.y + energy.h / 4, 36,
                   energy.h / 2, publicBackg);
  if (energy.currentImport <= -1000) {
    display.setCursor(energy.x + energy.w / 2 - 36,
                      energy.y + energy.h / 2 - 24);
  } else {
    display.setCursor(energy.x + energy.w / 2, energy.y + energy.h / 2 - 24);
  }
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(6);
  if (energy.currentImport != 99999) {
    display.print(energy.currentImport);
  } else {
    display.print("N/A");
  }
  display.print("W");
}
void Dashboard::homeEnergyImg() {
  bliksem(energy.x + energy.w / 4, energy.y + energy.h / 2, energy.state);
}

int median(const int *arr, int i, int range) {
  int buf[range * 2 + 1];
  int count = 0;
  for (int j = i - range; j <= i + range; j++) {
    if (j >= 0 && j < 360 && arr[j] != 99999)
      buf[count++] = arr[j];
  }
  if (count == 0)
    return 99999;
  // Simpele bubblesort
  for (int a = 0; a < count - 1; a++)
    for (int b = 0; b < count - a - 1; b++)
      if (buf[b] > buf[b + 1]) {
        int t = buf[b];
        buf[b] = buf[b + 1];
        buf[b + 1] = t;
      }
  return buf[count / 2];
}

void Dashboard::detailEnergy() {
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  energyV();
  energyGraph();
  bliksem(450 + 340 / 4, 124 + 156 / 2, energy.state);
}
void Dashboard::energyGraph() {
  if (energy.drawConsumption) {
    display.fillCircle(460, 330, 10, HABlue);
  } else {
    display.fillCircle(460, 330, 10, publicBackg);
    display.drawCircle(460, 330, 10, HABlue);
  }
  if (energy.drawImport) {
    display.fillCircle(460, 375, 10, ORANGE);
  } else {
    display.fillCircle(460, 375, 10, publicBackg);
    display.drawCircle(460, 375, 10, ORANGE);
  }
  if (energy.drawProduction) {
    display.fillCircle(460, 420, 10, evccGREEN);
  } else {
    display.fillCircle(460, 420, 10, publicBackg);
    display.drawCircle(460, 420, 10, evccGREEN);
  }
  display.fillRoundRect(61, 41, 358, 398, 9, publicBackg);
  display.fillRoundRect(450, 40, 340, 44, 10, publicTEXT);
  if (energy.longGraph) {
    display.fillRoundRect(625, 46, 160, 32, 5, publicBackg);
    display.setTextSize(3);
    display.setTextColor(publicBackg);
    display.setCursor(487, 50);
    display.print("30 min");
    display.setTextColor(publicTEXT);
    display.setCursor(687, 50);
    display.print("3u");
    for (int i = -1; i < 11; i++) {
      if (i != 10 && i != -1) {
        display.drawFastHLine(60, 400 - i * 36, 360, GREY);
        if (i == 0) {
          display.drawFastHLine(60, 400 - i * 36, 360, publicTEXT);
        }
      }
      display.setTextColor(GREY);
      display.setTextSize(1);
      display.setCursor(15, 396 - i * 36);
      display.print(i);
      display.print(" kW");
    }
    for (int i = 0; i < 359; i++) {
      if (energy.drawConsumption && (energy.longConsumption[i] != 99999 &&
                                     energy.longConsumption[i + 1] != 99999 &&
                                     energy.longConsumption[i - 1] != 99999)) {
        display.drawLine(
            60 + i,
            400 - map(median(energy.longConsumption, i, 3), 0, 10000, 0, 360),
            61 + i,
            400 -
                map(median(energy.longConsumption, i + 1, 3), 0, 10000, 0, 360),
            HABlue);
      }
      if (energy.drawImport &&
          (energy.longImport[i] != 99999 && energy.longImport[i + 1] != 99999 &&
           energy.longImport[i - 1] != 99999)) {
        display.drawLine(
            60 + i,
            400 - map(median(energy.longImport, i, 3), 0, 10000, 0, 360),
            61 + i,
            400 - map(median(energy.longImport, i + 1, 3), 0, 10000, 0, 360),
            ORANGE);
      }
      if (energy.drawProduction && (energy.longProduction[i] != 99999 &&
                                    energy.longProduction[i + 1] != 99999 &&
                                    energy.longProduction[i - 1] != 99999)) {
        display.drawLine(
            60 + i,
            400 - map(median(energy.longProduction, i, 3), 0, 10000, 0, 360),
            61 + i,
            400 -
                map(median(energy.longProduction, i + 1, 3), 0, 10000, 0, 360),
            evccGREEN);
      }
    }
    display.fillRect(50, 444, 390, 20, publicBackg);
    for (int i = 0; i < 359; i++) {
      if (energy.longTimeArr[i] > 0) {
        for (int x = 0; x < 20; x++) {
          display.drawFastVLine(60 + i, 50 + x * 20, 5, GREY);
        }
        display.setTextColor(GREY);
        display.setTextSize(1);
        if (energy.longTimeArr[i] < 10) {
          display.setCursor(54 + i, 450);
        } else {
          display.setCursor(51 + i, 450);
        }
        display.print(energy.longTimeArr[i]);
        display.print("u");
      }
    }
  } else {
    display.fillRoundRect(455, 46, 160, 32, 5, publicBackg);
    display.setTextSize(3);
    display.setTextColor(publicTEXT);
    display.setCursor(487, 50);
    display.print("30 min");
    display.setTextColor(publicBackg);
    display.setCursor(687, 50);
    display.print("3u");
    for (int i = -1; i < 11; i++) {
      if (i != 10 && i != -1) {
        display.drawFastHLine(60, 400 - i * 36, 360, GREY);
        if (i == 0) {
          display.drawFastHLine(60, 400 - i * 36, 360, publicTEXT);
        }
      }
      display.setTextColor(GREY);
      display.setTextSize(1);
      display.setCursor(15, 396 - i * 36);
      display.print(i);
      display.print(" kW");
    }
    if (energy.roundState) {
      for (int i = 0; i < 359; i++) {
        if (energy.drawConsumption && (energy.consumption[i] != 99999 &&
                                       energy.consumption[i + 1] != 99999 &&
                                       energy.consumption[i - 1] != 99999)) {
          display.drawLine(
              60 + i,
              400 - map(median(energy.consumption, i, 6), 0, 10000, 0, 360),
              61 + i,
              400 - map(median(energy.consumption, i + 1, 6), 0, 10000, 0, 360),
              HABlue);
        }
        if (energy.drawImport &&
            (energy.import[i] != 99999 && energy.import[i + 1] != 99999 &&
             energy.import[i - 1] != 99999)) {
          display.drawLine(
              60 + i, 400 - map(median(energy.import, i, 6), 0, 10000, 0, 360),
              61 + i,
              400 - map(median(energy.import, i + 1, 6), 0, 10000, 0, 360),
              ORANGE);
        }
        if (energy.drawProduction && (energy.production[i] != 99999 &&
                                      energy.production[i + 1] != 99999 &&
                                      energy.production[i - 1] != 99999)) {
          display.drawLine(
              60 + i,
              400 - map(median(energy.production, i, 6), 0, 10000, 0, 360),
              61 + i,
              400 - map(median(energy.production, i + 1, 6), 0, 10000, 0, 360),
              evccGREEN);
        }
      }
    } else {
      for (int i = 0; i < 359; i++) {
        if (energy.drawConsumption && (energy.consumption[i] != 99999 &&
                                       energy.consumption[i + 1] != 99999 &&
                                       energy.consumption[i - 1] != 99999)) {
          display.drawLine(
              60 + i, 400 - map(energy.consumption[i], 0, 10000, 0, 360),
              61 + i, 400 - map(energy.consumption[i + 1], 0, 10000, 0, 360),
              HABlue);
        }
        if (energy.drawImport &&
            (energy.import[i] != 99999 && energy.import[i + 1] != 99999 &&
             energy.import[i - 1] != 99999)) {
          display.drawLine(
              60 + i, 400 - map(energy.import[i], 0, 10000, 0, 360), 61 + i,
              400 - map(energy.import[i + 1], 0, 10000, 0, 360), ORANGE);
        }
        if (energy.drawProduction && (energy.production[i] != 99999 &&
                                      energy.production[i + 1] != 99999 &&
                                      energy.production[i - 1] != 99999)) {
          display.drawLine(
              60 + i, 400 - map(energy.production[i], 0, 10000, 0, 360), 61 + i,
              400 - map(energy.production[i + 1], 0, 10000, 0, 360), evccGREEN);
        }
      }
    }
    display.fillRect(50, 444, 390, 20, publicBackg);
    for (int i = 0; i < 359; i++) {
      if (energy.timeArr[i] > 0) {
        for (int x = 0; x < 20; x++) {
          display.drawFastVLine(60 + i, 50 + x * 20, 5, GREY);
        }
        display.setTextColor(GREY);
        display.setTextSize(1);
        if (energy.timeArr[i] < 10) {
          display.setCursor(54 + i, 450);
        } else {
          display.setCursor(51 + i, 450);
        }
        display.print(energy.timeArr[i]);
        display.print("u");
      }
    }
  }
  display.drawRoundRect(60, 40, 360, 400, 10, publicTEXT);
}
void Dashboard::energyV() {
  display.setTextSize(3);
  display.setTextColor(publicTEXT, publicBackg);
  display.setCursor(480, 320);
  display.print("Verbruik: ");
  if (energy.currentConsumption != 99999) {
    display.print(energy.currentConsumption);
  } else {
    display.print("N/A");
  }
  display.print("W");
  display.setCursor(480, 365);
  display.print("Import: ");
  if (energy.currentImport != 99999) {
    display.print(energy.currentImport);
  } else {
    display.print("N/A");
  }
  display.print("W");
  display.setCursor(480, 410);
  display.print("Opbrengst: ");
  if (energy.currentProduction != 99999) {
    display.print(energy.currentProduction);
  } else {
    display.print("N/A");
  }
  display.print("W");

  // Overzicht
  if (energy.currentImport <= -1000) {
    display.setCursor(450 + 340 / 2 - 36, 124 + 156 / 2 - 24);
  } else {
    display.setCursor(450 + 340 / 2, 124 + 156 / 2 - 24);
  }
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(6);
  if (energy.currentImport != 99999) {
    display.print(energy.currentImport);
  } else {
    display.print("N/A");
  }
  display.print("W");
  display.fillRect(799, 0, 2, 480, publicAllBackg);
}

Energy::Energy() {
  for (int i = 0; i < 360; i++) {
    import[i] = 99999;
    consumption[i] = 99999;
    production[i] = 99999;
    timeArr[i] = 0;

    longImport[i] = 99999;
    longConsumption[i] = 99999;
    longProduction[i] = 99999;
    longTimeArr[i] = 0;
  }
}
void Energy::mqtt(const String &topic, const String &message, bool &newHomeInfo,
                  const String &realTime) {
  String normalizedMessage = message;
  if (topic.startsWith("energy/")) {
    if (topic == "energy/currentImport") {
      normalizedMessage.replace(".0", "");
      newHomeInfo = HIGH;
      newEnergyVal = HIGH;
      currentImport = normalizedMessage.toInt();
      if (currentImport < 0) {
        if (state != 0) {
          state = 0;
          newBlxmInfo = HIGH;
        }
      } else if (currentImport < 4800) {
        if (state != 1) {
          state = 1;
          newBlxmInfo = HIGH;
        }
      } else {
        if (state != 2) {
          state = 2;
          newBlxmInfo = HIGH;
        }
      }
      if (normalizedMessage == "unavailable") {
        currentImport = 99999;
      }
    }
    if (topic == "energy/currentProduction") {
      normalizedMessage.replace(".0", "");
      newEnergyVal = HIGH;
      currentProduction = normalizedMessage.toInt();
      if (normalizedMessage == "unavailable") {
        currentProduction = 99999;
      }
    }
    if (topic == "energy/currentConsumption") {
      normalizedMessage.replace(".0", "");
      newEnergyVal = HIGH;
      currentConsumption = normalizedMessage.toInt();
      if (normalizedMessage == "unavailable") {
        currentConsumption = 99999;
      }
    }
  }
  if (topic == "home/interval") {
    if (message == "5s") {
      String k = realTime;
      k.replace(":", "");
      for (int i = 0; i < 359; i++) {
        import[i] = import[i + 1];
        consumption[i] = consumption[i + 1];
        production[i] = production[i + 1];
        timeArr[i] = timeArr[i + 1];
        // Serial.print(import[i]);
        // Serial.print(" ");
      }
      if (currentImport < -1111) {
        import[359] = -1111;
      } else {
        import[359] = currentImport;
      }
      if (currentConsumption < -1111) {
        consumption[359] = -1111;
      } else {
        consumption[359] = currentConsumption;
      }
      production[359] = currentProduction;
      // Serial.print(import[359]);
      // Serial.println();

      if (k.toInt() % 100 == 0 && newMinute) {
        timeArr[359] = k.toInt() / 100;
        newMinute = LOW;
      } else {
        timeArr[359] = 0;
      }
      newEnergyGraph = HIGH;
    }
    if (message == "30s") {
      String k = realTime;
      k.replace(":", "");
      for (int i = 0; i < 359; i++) {
        longImport[i] = longImport[i + 1];
        longConsumption[i] = longConsumption[i + 1];
        longProduction[i] = longProduction[i + 1];
        longTimeArr[i] = longTimeArr[i + 1];
        // Serial.print(longImport[i]);
        // Serial.print(" ");
      }
      if (currentImport < -1111) {
        longImport[359] = -1111;
      } else {
        longImport[359] = currentImport;
      }
      if (currentConsumption < -1111) {
        longConsumption[359] = -1111;
      } else {
        longConsumption[359] = currentConsumption;
      }
      longProduction[359] = currentProduction;
      // Serial.print(longImport[359]);
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
  if (topic == "home/time") {
    newMinute = HIGH;
    longNewMinute = HIGH;
  }
}
void Energy::newInfo(Dashboard &dashboard, const String &page,
                     const int *primes, int homePage, float car_chargingPower,
                     float currentEpex) {
  if (newEnergyVal || newEnergyGraph || newBlxmInfo) {
    if (page == "energy") {
      if (newEnergyVal) {
        dashboard.energyV();
      }
      if (newBlxmInfo) {
        dashboard.bliksem(450 + 340 / 4, 124 + 156 / 2, state);
      }
      if (newEnergyGraph) {
        dashboard.energyGraph();
      }
    }
    if (page == "home" && newBlxmInfo) {
      if (prime != 0 && prime % primes[homePage - 1] == 0) {
        dashboard.bliksem(x + w / 4, y + h / 2, state);
      }
    }
    if (page == "evcc") {
      dashboard.evcc(currentProduction, currentImport, currentConsumption,
                     car_chargingPower, currentEpex);
    }
    newBlxmInfo = LOW;
    newEnergyVal = LOW;
    newEnergyGraph = LOW;
  }
}
bool Energy::touchHandler(Dashboard &dashboard, int touch_x, int touch_y,
                          String &page, float car_chargingPower,
                          float currentEpex) {
  bool handled = LOW;
  if (page == "energy") {
    newEnergyGraph = HIGH;
    handled = HIGH;
    if (touch_x > 440 && touch_x < 800 && touch_y > 20 &&
        touch_y < 100) { // long graph clicked
      longGraph = !longGraph;
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
             touch_y < 124 + 156 / 2 + 85) { // lightning bolt clicked: to evcc
      page = "evcc";
      dashboard.fillScreen(dashboard.publicAllBackg);
      dashboard.homeEmpty(0, 0, 798, 479);
      Serial.println(currentEpex);
      dashboard.evcc(currentProduction, currentImport, currentConsumption,
                     car_chargingPower, currentEpex);
    }
#endif
    else if (!longGraph) {
      roundState = !roundState;
    } else {
      newEnergyGraph = LOW;
    }
  }
  return handled;
}
void Energy::setCoordinates(int *energyCo) {
  x = energyCo[0];
  y = energyCo[1];
  w = energyCo[2];
  h = energyCo[3];
  prime = energyCo[4];
}
bool Energy::clicked(int touch_x, int touch_y, int primePageNr) {
  return (touch_x > x && touch_x < x + w && touch_y > y && touch_y < y + h &&
          onHomePage(primePageNr));
}
bool Energy::onHomePage(int primePageNr) {
  return (prime != 0 && prime % primePageNr == 0);
}
#endif