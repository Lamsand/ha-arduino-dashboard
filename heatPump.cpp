#include "Dashboard.h"
#include "Arduino.h"
#include "Colors.h"
#include "config.h"

#ifdef ENABLE_HEAT_PUMP
void Dashboard::printHeatPumpTime(const String& time) {
  display.setCursor(624, 12);
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(3);
  display.print(":");
  display.print(time);
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
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 120;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::heatPumpIconSmall(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("Heat pump/Small/heatPumpDLS.bin");
  } else {
    file = sd.open("Heat pump/Small/heatPumpLLS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 24;
  const int w = 15;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
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
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 350;
  const int w = 250;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::WaterTempIcon(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("Heat pump/Watertemp/WaterTempD.bin");
  } else {
    file = sd.open("Heat pump/Watertemp/WaterTempL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 80;
  const int w = 80;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::WaterTempIconSmall(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("Heat pump/Watertemp/WaterTempDS.bin");
  } else {
    file = sd.open("Heat pump/Watertemp/WaterTempLS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 24;
  const int w = 29;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
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
#endif