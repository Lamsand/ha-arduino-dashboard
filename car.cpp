#include "Arduino.h"
#include "Colors.h"
#include "Dashboard.h"

#ifdef ENABLE_CAR
void Dashboard::carImageMini(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("Car/cDC.bin");
  } else {
    file = sd.open("Car/cLC.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 146;
  const int w = 300;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::carImageLarge(int xInput, int yInput) {
  int h = 267;
  int w = 550;
  if (publicBackg == BLACK) {
    file = sd.open("Car/cDL.bin");
    int h = 267;
    int w = 550;
  } else {
    file = sd.open("Car/cLL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}

void Dashboard::homeCar(int x, int y, int w, int h, int battery,
                        float chargingCapacity, int target) {
  display.fillRoundRect(x + 1, y + 1, w - 1, h - 1, 10, publicBackg);
  // int carsize = w - w / 8 - w / 24 - w / 12;
  // if ((h - h / 12 - 3 * w / 24) * 2 < carsize) {
  //   carsize = (h - h / 12 - 3 * w / 24) * 2;
  // }
  // car(x + (w - w / 8 - w / 24) / 2, y + w / 24 + (h - h / 12 - 4 * w / 24) /
  // 2, carsize, publicTEXT); battery percentage
  display.drawRoundRect(x + w / 24 - 1, y + h - h / 12 - w / 24 - 1,
                        3 * w / 4 + 2, h / 12 + 2, h / 24 + 1, publicBackg);
  display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24, 3 * w / 4, h / 12,
                        h / 24, BLACK);
  if (chargingCapacity > 0.0) {
    for (int i = 0; i < target * 3 * w / 400 - h / 12; i = i + 10) {
      display.drawRoundRect(x + w / 24, y + h - h / 12 - w / 24,
                            target * 3 * w / 400 - i, h / 12, h / 24, HABlue);
    }
    if (battery > 20) {
      display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24,
                            battery * 3 * w / 400, h / 12, h / 24, evccGREEN);
    } else {
      display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24,
                            battery * 3 * w / 400, h / 12, h / 24, RED);
    }
  } else {
    display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24,
                          battery * 3 * w / 400, h / 12, h / 24, grey);
  }
  display.drawRoundRect(x + w / 24, y + h - h / 12 - w / 24, 3 * w / 4, h / 12,
                        h / 24, publicTEXT);
  // charging power
  display.drawRoundRect(x + w - w / 8 - w / 24 - 1, y + w / 24 - 1, w / 8 + 2,
                        h - 2 * w / 24 + 2, h / 24 + 1, publicBackg);
  display.fillRoundRect(x + w - w / 8 - w / 24, y + w / 24, w / 8,
                        h - 2 * w / 24, h / 24, BLACK);
  display.fillRoundRect(
      x + w - w / 8 - w / 24,
      y + w / 24 + (11 - chargingCapacity) * (h - 2 * w / 24) / 11 + 1, w / 8,
      chargingCapacity * (h - 2 * w / 24) / 11, h / 24, HABlue);
  display.drawRoundRect(x + w - w / 8 - w / 24, y + w / 24, w / 8,
                        h - 2 * w / 24, h / 24, publicTEXT);
  // carImageMini(x + (w - w / 8 - w / 24) / 2, y + w / 24 + (h - h / 12 - 4 * w
  // / 24) / 2);
}
void Dashboard::homeCarV(int x, int y, int w, int h, int battery,
                         float chargingCapacity, int target) {
  display.drawRoundRect(x + w / 24 - 1, y + h - h / 12 - w / 24 - 1,
                        3 * w / 4 + 2, h / 12 + 2, h / 24 + 1, publicBackg);
  display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24, 3 * w / 4, h / 12,
                        h / 24, BLACK);
  if (chargingCapacity > 0.0) {
    for (int i = 0; i < target * 3 * w / 400 - h / 12; i = i + 10) {
      display.drawRoundRect(x + w / 24, y + h - h / 12 - w / 24,
                            target * 3 * w / 400 - i, h / 12, h / 24, HABlue);
    }
    if (battery > 20) {
      display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24,
                            battery * 3 * w / 400, h / 12, h / 24, evccGREEN);
    } else {
      display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24,
                            battery * 3 * w / 400, h / 12, h / 24, RED);
    }
  } else {
    display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24,
                          battery * 3 * w / 400, h / 12, h / 24, grey);
  }
  display.drawRoundRect(x + w / 24, y + h - h / 12 - w / 24, 3 * w / 4, h / 12,
                        h / 24, publicTEXT);
  // charging power
  display.drawRoundRect(x + w - w / 8 - w / 24 - 1, y + w / 24 - 1, w / 8 + 2,
                        h - 2 * w / 24 + 2, h / 24 + 1, publicBackg);
  display.fillRoundRect(x + w - w / 8 - w / 24, y + w / 24, w / 8,
                        h - 2 * w / 24, h / 24, BLACK);
  if (chargingCapacity > 0.0) {
    display.fillRoundRect(
        x + w - w / 8 - w / 24,
        y + w / 24 + (11 - chargingCapacity) * (h - 2 * w / 24) / 11 + 1, w / 8,
        chargingCapacity * (h - 2 * w / 24) / 11, h / 24, HABlue);
  }
  display.drawRoundRect(x + w - w / 8 - w / 24, y + w / 24, w / 8,
                        h - 2 * w / 24, h / 24, publicTEXT);
}
void Dashboard::homeCarImg(int x, int y, int w, int h) {
  carImageMini(x + (w - w / 8 - w / 24) / 2, y + (h - h / 12 - w / 24) / 2);
}

void Dashboard::detailCar(int battery, float chargingCapacity,
                          int chargingSpeed, int target, int range) {
  const int spacing = 33;
  const int batteryH = 60;
  const int powerW = 150;
  // const int carsize = SCREEN_W - powerW - 3 * spacing;
  display.fillScreen(publicAllBackg);
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  // car((SCREEN_W - powerW - spacing) / 2, (SCREEN_H - batteryH - 3 * spacing)
  // / 2, carsize); battery percentage
  display.drawRoundRect(spacing - 1, SCREEN_H - batteryH - spacing - 1,
                        SCREEN_W - 3 * spacing - powerW + 2, batteryH + 2,
                        batteryH / 2 + 1, publicBackg);
  display.fillRoundRect(spacing, SCREEN_H - batteryH - spacing,
                        SCREEN_W - 3 * spacing - powerW, batteryH, batteryH / 2,
                        BLACK);
  if (chargingCapacity > 0.0) {
    for (int i = 0;
         i < target * (SCREEN_W - 3 * spacing - powerW) / 100 - batteryH;
         i = i + 20) {
      display.drawRoundRect(
          spacing, SCREEN_H - batteryH - spacing,
          target * (SCREEN_W - 3 * spacing - powerW) / 100 - i, batteryH,
          batteryH / 2, HABlue);
    }
    if (battery > 20) {
      display.fillRoundRect(spacing, SCREEN_H - batteryH - spacing,
                            battery * (SCREEN_W - 3 * spacing - powerW) / 100,
                            batteryH, batteryH / 2, evccGREEN);
    } else {
      display.fillRoundRect(spacing, SCREEN_H - batteryH - spacing,
                            battery * (SCREEN_W - 3 * spacing - powerW) / 100,
                            batteryH, batteryH / 2, RED);
    }
  } else {
    display.fillRoundRect(spacing, SCREEN_H - batteryH - spacing,
                          battery * (SCREEN_W - 3 * spacing - powerW) / 100,
                          batteryH, batteryH / 2, grey);
  }
  display.drawRoundRect(spacing, SCREEN_H - batteryH - spacing,
                        SCREEN_W - 3 * spacing - powerW, batteryH, batteryH / 2,
                        publicTEXT);
  // charging power
  display.drawRoundRect(SCREEN_W - powerW - spacing - 1, spacing - 1,
                        powerW + 2, SCREEN_H - 2 * spacing + 2,
                        batteryH / 2 + 1, publicBackg);
  display.fillRoundRect(SCREEN_W - powerW - spacing, spacing, powerW,
                        SCREEN_H - 2 * spacing, batteryH / 2, BLACK);
  display.fillRoundRect(
      SCREEN_W - powerW - spacing,
      spacing + (11 - chargingCapacity) * (SCREEN_H - 2 * spacing) / 11 + 1,
      powerW, chargingCapacity * (SCREEN_H - 2 * spacing) / 11, batteryH / 2,
      HABlue);
  display.drawRoundRect(SCREEN_W - powerW - spacing, spacing, powerW,
                        SCREEN_H - 2 * spacing, batteryH / 2, publicTEXT);

  // Values
  const String batteryText =
      String(String(battery) + "% " + String(range) + "km");
  const int batteryTextW = batteryText.length() * 18 - 3;
  display.setTextSize(3);
  display.setTextColor(BLACK);
  if (battery > 30) {
    display.setCursor(
        spacing + (battery * (SCREEN_W - 3 * spacing - powerW) / 100) / 2 -
            batteryTextW / 2,
        SCREEN_H - batteryH / 2 - spacing - 13);
    display.print(batteryText);
  } else {
    display.setTextColor(WHITE);
    display.setCursor(
        SCREEN_W - 2 * spacing - powerW -
            ((100 - battery) * (SCREEN_W - 3 * spacing - powerW) / 100) / 2 -
            batteryTextW / 2,
        SCREEN_H - batteryH / 2 - spacing - 13);
    display.print(batteryText);
  }

  const String powerText = String(String(chargingCapacity) + "kW");
  const int powerTextW = powerText.length() * 18 - 3;
  display.setTextSize(3);
  if (chargingCapacity > 1.5) {
    display.setTextColor(BLACK);
    display.setCursor(
        SCREEN_W - spacing - powerW / 2 - powerTextW / 2,
        SCREEN_H - spacing -
            (chargingCapacity * (SCREEN_H - 2 * spacing) / 11) / 2 - 12);
    display.print(powerText);
  } else {
    display.setTextColor(WHITE);
    display.setCursor(
        SCREEN_W - spacing - powerW / 2 - powerTextW / 2,
        spacing +
            ((11 - chargingCapacity) * (SCREEN_H - 2 * spacing) / 11) / 2 - 12);
    display.print(powerText);
  }
  carImageLarge(spacing + (SCREEN_W - powerW - 2 * spacing) / 2,
                spacing + (SCREEN_H - batteryH - 3 * spacing) / 2);
}

#endif