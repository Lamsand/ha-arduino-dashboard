#include "Dashboard.h"
#include "Arduino.h"
#include "Colors.h"

#ifdef ENABLE_LIGHTS
void Dashboard::moonlampImg(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("Lights/Moonlamp/moonlampHD.bin");
    } else {
      file = sd.open("Lights/Moonlamp/moonlampHL.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("Lights/Moonlamp/moonlampLD.bin");
    } else {
      file = sd.open("Lights/Moonlamp/moonlampLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 162;
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
void Dashboard::bolImg(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("Lights/Globe lamp/globeHD.bin");
    } else {
      file = sd.open("Lights/Globe lamp/globeHL.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("Lights/Globe lamp/globeLD.bin");
    } else {
      file = sd.open("Lights/Globe lamp/globeLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 100;
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
void Dashboard::bergingImg(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("Lights/Philips Hue/PhilipsHueHD.bin");
    } else {
      file = sd.open("Lights/Philips Hue/PhilipsHueHL.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("Lights/Philips Hue/PhilipsHueLD.bin");
    } else {
      file = sd.open("Lights/Philips Hue/PhilipsHueLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 156;
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
void Dashboard::sfeerlichtjesImg(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("Lights/Christmas lights/christmasLightsHD.bin");
    } else {
      file = sd.open("Lights/Christmas lights/christmasLightsHL.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("Lights/Christmas lights/christmasLightsLD.bin");
    } else {
      file = sd.open("Lights/Christmas lights/christmasLightsLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 90;
  const int w = 170;
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

void Dashboard::homeLightsImg(int x, int y, int w, int h) {
  bolImg(x + w / 2, y + h / 2, HIGH);
}

void Dashboard::detailLights(bool sfeerlichtjes, bool groteBol, bool glazenBol,
                             bool berging, bool maanlamp, bool raamversiering) {
  // Sfeerlichtjes
  display.fillRoundRect(1, 1, SCREEN_W / 3 - 1, SCREEN_H / 2 - 1, 10,
                        publicBackg);
  // bulb(SCREEN_W / 6, SCREEN_H / 4 + 16, 170, sfeerlichtjes);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(20, 20);
  display.print("Sfeerlichtjes");
  // sfeerlichtjesImg(SCREEN_W / 6, SCREEN_H / 4 + 16, sfeerlichtjes);
  // Grote bol
  display.fillRoundRect(SCREEN_W / 3 + 1, 1, SCREEN_W / 3 - 1, SCREEN_H / 2 - 1,
                        10, publicBackg);
  // bulb(3 * SCREEN_W / 6, SCREEN_H / 4 + 16, 170, groteBol);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(21 + SCREEN_W / 3, 20);
  display.print("Grote bol");
  // bolImg(3 * SCREEN_W / 6, SCREEN_H / 4 + 16, groteBol);
  // Glazen bol
  display.fillRoundRect(2 * SCREEN_W / 3, 1, SCREEN_W / 3, SCREEN_H / 2 - 1, 10,
                        publicBackg);
  // bulb(5 * SCREEN_W / 6, SCREEN_H / 4 + 16, 170, glazenBol);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(20 + 2 * SCREEN_W / 3, 20);
  display.print("Glazen bol");
  // bolImg(5 * SCREEN_W / 6, SCREEN_H / 4 + 16, glazenBol);
  // Berging
  display.fillRoundRect(1, SCREEN_H / 2 + 1, SCREEN_W / 2 - 1, SCREEN_H / 2 - 2,
                        10, publicBackg);
  // bulb(SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, 170, berging);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(20, 260);
  display.print("Maanlamp");
  // moonlampImg(SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, maanlamp);
  // Maanlamp
  display.fillRoundRect(SCREEN_W / 2 + 1, SCREEN_H / 2 + 1, SCREEN_W / 2 - 2,
                        SCREEN_H / 2 - 2, 10, publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(21 + SCREEN_W / 2, 260);
  display.print("Raamversiering");
  // moonlampImg(3 * SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, raamversiering);
}
void Dashboard::lightBr(int br) {
  display.drawRoundRect(299, 39, 202, 282, 51, publicTEXT);
  display.fillRoundRect(300, 40, 200, 280, 50, publicBackg);
  if (br > 33) {
    display.fillRoundRect(305, 40 + 275 - 270 * br / 100, 190, 270 * br / 100,
                          45, YELLOW);
  } else if (br != 0) {
    display.fillRoundRect(305, 40 + 275 - 270 * 33 / 100, 190, 270 * 33 / 100,
                          45, YELLOW);
    display.fillRoundRect(305, 45, 190, 270 * (100 - br) / 100, 45,
                          publicBackg);
  }
  display.setTextSize(3);
  if (br < 30) {
    display.setTextColor(publicTEXT);
    display.setCursor(375, 40 + 270 * (100 - br) / 200 - 11);
    if (br < 10) {
      display.setCursor(384, 40 + 270 * (100 - br) / 200 - 11);
    }
  } else {
    display.setTextColor(darkYELLOW);
    display.setCursor(375, 45 + 270 - 270 * br / 200 - 11);
    if (br == 100) {
      display.setCursor(366, 45 + 270 - 270 * br / 200 - 11);
    }
  }
  display.print(br);
  display.print("%");
}
void Dashboard::lightSw() {
  display.drawRoundRect(299, 339, 202, 102, 51, publicTEXT);
  display.fillRoundRect(300, 340, 200, 100, 50, publicBackg);
  // fillArc(400, 390 , 45, 90, 30, 5, publicTEXT);
  display.fillCircle(400, 390, 30, publicTEXT);
  display.fillCircle(400, 390, 25, publicBackg);
  fillArc(400, 390, -45, 30, 40, 20, publicBackg);
  display.fillRoundRect(397, 355, 6, 30, 2, publicTEXT);
}
#endif