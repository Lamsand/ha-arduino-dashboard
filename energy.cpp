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

void Dashboard::homeEnergy(int x, int y, int w, int h, int state, int value) {
  display.fillRoundRect(x + 1, y + 1, w - 1, h - 1, 10, publicBackg);
  // bliksem(x + w / 4, y + h / 2, state);
  if (value <= -1000) {
    display.setCursor(x + w / 2 - 36, y + h / 2 - 24);
  } else {
    display.setCursor(x + w / 2, y + h / 2 - 24);
  }
  display.setTextColor(publicTEXT);
  display.setTextSize(6);
  if (value != 99999) {
    display.print(value);
  } else {
    display.print("N/A");
  }
  display.print("W");
}
void Dashboard::homeEnergyV(int x, int y, int w, int h, int state, int value) {
  display.fillRect(x + w / 2, y + h / 4, w / 2, h / 2, publicBackg);
  display.fillRect(x + w, y + h / 4, 1, h / 2, publicTEXT);
  display.fillRect(x + w + 2, y + h / 4, 30, h / 2, publicBackg);
  display.fillRect(x + w / 2 - 36, y + h / 4, 36, h / 2, publicBackg);
  if (value <= -1000) {
    display.setCursor(x + w / 2 - 36, y + h / 2 - 24);
  } else {
    display.setCursor(x + w / 2, y + h / 2 - 24);
  }
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(6);
  if (value != 99999) {
    display.print(value);
  } else {
    display.print("N/A");
  }
  display.print("W");
}
void Dashboard::homeEnergyImg(int x, int y, int w, int h, int state) {
  bliksem(x + w / 4, y + h / 2, state);
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
  energyV(energy.currentImport, energy.currentProduction,
          energy.currentConsumption);
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
void Dashboard::energyV(int currentImport, int currentProduction,
                        int currentConsumption) {
  display.setTextSize(3);
  display.setTextColor(publicTEXT, publicBackg);
  display.setCursor(480, 320);
  display.print("Verbruik: ");
  if (currentConsumption != 99999) {
    display.print(currentConsumption);
  } else {
    display.print("N/A");
  }
  display.print("W");
  display.setCursor(480, 365);
  display.print("Import: ");
  if (currentImport != 99999) {
    display.print(currentImport);
  } else {
    display.print("N/A");
  }
  display.print("W");
  display.setCursor(480, 410);
  display.print("Opbrengst: ");
  if (currentProduction != 99999) {
    display.print(currentProduction);
  } else {
    display.print("N/A");
  }
  display.print("W");

  // Overzicht
  if (currentImport <= -1000) {
    display.setCursor(450 + 340 / 2 - 36, 124 + 156 / 2 - 24);
  } else {
    display.setCursor(450 + 340 / 2, 124 + 156 / 2 - 24);
  }
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(6);
  if (currentImport != 99999) {
    display.print(currentImport);
  } else {
    display.print("N/A");
  }
  display.print("W");
  display.fillRect(799, 0, 2, 480, WHITE);
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
#endif