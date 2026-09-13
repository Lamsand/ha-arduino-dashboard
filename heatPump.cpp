#include "Arduino.h"
#include "Colors.h"
#include "Dashboard.h"
#include "config.h"

#ifdef ENABLE_HEAT_PUMP
void Dashboard::printHeatPumpTime(const String &time) {
  display.setCursor(624, 12);
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(3);
  display.print(":");
  if (time.indexOf("warm") != -1) {
    display.print("warm");
  } else {
    display.print(time);
  }
}
void Dashboard::printWaterTemp(int temp) {
  display.setCursor(490, 12);
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(3);
  display.print(":");
  display.print(temp);
  display.write(0xF8);
  display.print("C ");
}

void Dashboard::heatPumpIcon(int xInput, int yInput, bool state) {
  if (publicBackg == BLACK) {
    if (state) {
      file = sd.open("Heat pump/heatPumpDH.bin");
    } else {
      file = sd.open("Heat pump/heatPumpDL.bin");
    }
  } else {
    if (state) {
      file = sd.open("Heat pump/heatPumpLH.bin");
    } else {
      file = sd.open("Heat pump/heatPumpLL.bin");
    }
  }
  if (!file)
    return;
  file.seek(4); // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 120;
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
void Dashboard::heatPumpIconSmall(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("Heat pump/Small/heatPumpDLS.bin");
  } else {
    file = sd.open("Heat pump/Small/heatPumpLLS.bin");
  }
  if (!file)
    return;
  file.seek(4); // Skip the 4-byte header of the file
  int y = 0;
  const int h = 24;
  const int w = 15;
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
void Dashboard::heatPumpIconLarge(int xInput, int yInput, bool state) {
  if (publicBackg == BLACK) {
    if (state) {
      file = sd.open("Heat pump/Large/heatPumpDHL.bin");
    } else {
      file = sd.open("Heat pump/Large/heatPumpDLL.bin");
    }
  } else {
    if (state) {
      file = sd.open("Heat pump/Large/heatPumpLHL.bin");
    } else {
      file = sd.open("Heat pump/Large/heatPumpLLL.bin");
    }
  }
  if (!file)
    return;
  file.seek(4); // Skip the 4-byte header of the file
  int y = 0;
  const int h = 350;
  const int w = 250;
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
void Dashboard::WaterTempIcon(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("Heat pump/Watertemp/WaterTempD.bin");
  } else {
    file = sd.open("Heat pump/Watertemp/WaterTempL.bin");
  }
  if (!file)
    return;
  file.seek(4); // Skip the 4-byte header of the file
  int y = 0;
  const int h = 80;
  const int w = 80;
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
void Dashboard::WaterTempIconSmall(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("Heat pump/Watertemp/WaterTempDS.bin");
  } else {
    file = sd.open("Heat pump/Watertemp/WaterTempLS.bin");
  }
  if (!file)
    return;
  file.seek(4); // Skip the 4-byte header of the file
  int y = 0;
  const int h = 24;
  const int w = 29;
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

void Dashboard::homeHeatPump(int x, int y, int w, int h, int DHWtemp) {
  display.fillRoundRect(x + 1, y + 1, w - 1, h - 1, 10, publicBackg);
  display.setCursor(x + w / 2, y + h / 2 - 24);
  display.setTextColor(publicTEXT);
  display.setTextSize(6);
  display.print(DHWtemp);
  display.write(0xF8);
  display.print("C ");
}
void Dashboard::homeHeatPumpV(int x, int y, int w, int h, int DHWtemp) {
  display.setCursor(x + w / 2, y + h / 2 - 24);
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(6);
  display.print(DHWtemp);
  display.write(0xF8);
  display.print("C ");
}
void Dashboard::homeHeatPumpImg(int x, int y, int w, int h, bool state) {
  heatPumpIcon(x + w / 4, y + h / 2, state);
}

void Dashboard::heatPump(bool compr, int DHWtemp, int XDHW) {
  display.fillScreen(publicAllBackg);
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);

  // display.setTextColor(publicTEXT, publicBackg);
  // display.setTextSize(3);
  // display.setCursor(100, 100);
  // display.print("Compressor: ");
  // if (compr) {
  //   display.print("actief");
  // } else {
  //   display.print("inactief");
  // }

  // display.setTextSize(3);
  // display.setCursor(100, 130);
  // display.print("Warmwatertemperatuur: ");
  // display.print(DHWtemp);
  // display.write(0xF8);
  // display.print("C");

  display.setTextSize(10);
  display.setCursor(550, 130);
  display.print(DHWtemp);
  display.write(0xF8);
  display.print("C");

  display.setTextSize(3);
  display.setCursor(428, 260);
  display.print("Extra warmwater: ");
  display.print(XDHW);
  display.print("u");

  heatPumpIconLarge(175, 219, compr);
  WaterTempIcon(420, 170);

  if (compr) {
    display.setTextColor(publicTEXT, publicBackg);
    display.setTextSize(3);
    display.setCursor(85, 400);
    display.print("Warm water");
    display.setCursor(103, 430);
    display.print("maken...");
  }
}

void Dashboard::heatPumpChooseHour(int setH, int setM, bool clicked, bool PM) {
  // display.fillScreen(publicAllBackg);
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  display.setTextColor(publicBackg, publicTEXT);
  display.setTextSize(6);
  display.setCursor(310, 384);
  if (setH < 10) {
    display.print("0");
  }
  display.print(setH);
  display.setTextColor(publicTEXT, publicBackg);

  display.print(":");

  if (setM < 10) {
    display.print("0");
  }
  display.print(setM);

  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(3);
  display.setCursor(526, 408);
  if (PM) {
    display.print("Namiddag  ");
  } else {
    display.print("Voormiddag");
  }

  display.fillRoundRect(570, 42, 212, 52, 10, HABlue);
  display.setTextSize(4);
  display.setCursor(580, 52);
  display.setTextColor(BLACK, HABlue);
  display.print("Bevestig");

  display.setTextColor(publicTEXT, publicBackg);

  display.drawCircle(400, 192, 150, publicTEXT);
  display.drawCircle(400, 192, 140, publicTEXT);

  for (int i = 0; i < 12; i++) {
    float angle = (i * 30 - 90) * DEG_TO_RAD;
    int x1 = 400 + cos(angle) * 140;
    int y1 = 192 + sin(angle) * 140;
    int x2 = 400 + cos(angle) * 150;
    int y2 = 192 + sin(angle) * 150;
    display.drawLine(x1, y1, x2, y2, publicTEXT);

    display.setTextSize(3);
    int xText = 400 + cos(angle) * 110;
    int yText = 192 + sin(angle) * 110;
    if (i + 12 * PM < 10) {
      display.setCursor(xText - 9, yText - 12);
    } else {
      display.setCursor(xText - 18, yText - 12);
    }
    display.print(i + 12 * PM);
  }
  if (clicked) {
    int angle = setH % 12 * 30 - 30;
    int x = 400 + cos((angle - 60) * DEG_TO_RAD) * 125;
    int y = 192 + sin((angle - 60) * DEG_TO_RAD) * 125;
    display.drawLine(400, 192, x, y, RED);

    if (angle >= 330 || angle <= 30 || (angle >= 150 && angle <= 210)) {
      display.drawLine(399, 192, x - 1, y, RED);
      display.drawLine(401, 192, x + 1, y, RED);
    } else {
      display.drawLine(400, 191, x, y - 1, RED);
      display.drawLine(400, 193, x, y + 1, RED);
    }
  }
}

void Dashboard::heatPumpChooseMinute(int setH, int setM, bool clicked) {
  // display.fillScreen(publicAllBackg);
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(6);
  display.setCursor(310, 384);
  if (setH < 10) {
    display.print("0");
  }
  display.print(setH);
  display.print(":");
  display.setTextColor(publicBackg, publicTEXT);
  if (setM < 10) {
    display.print("0");
  }
  display.print(setM);

  display.fillRoundRect(570, 42, 212, 52, 10, HABlue);
  display.setTextSize(4);
  display.setCursor(580, 52);
  display.setTextColor(BLACK, HABlue);
  display.print("Bevestig");

  display.setTextColor(publicTEXT, publicBackg);

  display.drawCircle(400, 192, 150, publicTEXT);
  display.drawCircle(400, 192, 140, publicTEXT);

  display.setTextColor(publicTEXT, publicBackg);

  for (int i = 0; i < 12; i++) {
    float angle = (i * 30 - 90) * DEG_TO_RAD;
    int x1 = 400 + cos(angle) * 140;
    int y1 = 192 + sin(angle) * 140;
    int x2 = 400 + cos(angle) * 150;
    int y2 = 192 + sin(angle) * 150;
    display.drawLine(x1, y1, x2, y2, publicTEXT);

    display.setTextSize(3);
    int xText = 400 + cos(angle) * 110;
    int yText = 192 + sin(angle) * 110;
    if (i * 5 < 10) {
      display.setCursor(xText - 9, yText - 12);
    } else {
      display.setCursor(xText - 18, yText - 12);
    }
    display.print(i * 5);
  }
  if (clicked) {
    int angle = setM / 5 * 30 - 30;
    int x = 400 + cos((angle - 60) * DEG_TO_RAD) * 125;
    int y = 192 + sin((angle - 60) * DEG_TO_RAD) * 125;
    display.drawLine(400, 192, x, y, RED);

    if (angle >= 330 || angle <= 30 || (angle >= 150 && angle <= 210)) {
      display.drawLine(399, 192, x - 1, y, RED);
      display.drawLine(401, 192, x + 1, y, RED);
    } else {
      display.drawLine(400, 191, x, y - 1, RED);
      display.drawLine(400, 193, x, y + 1, RED);
    }
  }
}

void Dashboard::heatPumpTimeAlreadyPassed() {
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(2);
  display.setCursor(390, 410);
  display.print("De gekozen tijd is al voorbij!");
  display.setCursor(390, 438);
  display.print("De warmtepomp zal niet starten.");
  display.drawRoundRect(380, 400, 395, 55, 10, publicTEXT);
}
void Dashboard::heatPumpAlreadyHot() {
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(2);
  display.setCursor(390, 410);
  display.print("De warmtepomp heeft al gedraaid!");
  display.setCursor(390, 438);
  display.print("De tijd kan niet meer aangepast worden.");
  display.drawRoundRect(380, 400, 395, 55, 10, publicTEXT);
}
#endif