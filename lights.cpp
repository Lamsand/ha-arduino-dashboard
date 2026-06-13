#include "Arduino.h"
#include "Colors.h"
#include "Dashboard.h"
#include "config.h"

#ifdef ENABLE_LIGHTS

const char* lamp1 = "Sfeerlichtjes";
const char* lamp1type = "Christmas lights";

const char* lamp2 = "Grote bol";
const char* lamp2type = "Globe lamp";

const char* lamp3 = "Glazen bol";
const char* lamp3type = "Globe lamp";

const char* lamp4 = "Maanlamp";
const char* lamp4type = "Moonlamp";

const char* lamp5 = "Raamversiering";
const char* lamp5type = "ChristmasLights";

const char* lamp6 = "Sfeerlichtjes";
const char* lamp6type = "ChristmasLights";

#ifdef LAMP6_ENABLED
const int lamp1Co[4] = {1, 1, SCREEN_W / 3 - 1, SCREEN_H / 2 - 1};
const int lamp2Co[4] = {SCREEN_W / 3 + 1, 1, SCREEN_W / 3 - 1,
                        SCREEN_H / 2 - 1};
const int lamp3Co[4] = {2 * SCREEN_W / 3, 1, SCREEN_W / 3, SCREEN_H / 2 - 1};
const int lamp4Co[4] = {1, SCREEN_H / 2 + 1, SCREEN_W / 3 - 1,
                        SCREEN_H / 2 - 2};
const int lamp5Co[4] = {SCREEN_W / 3 + 1, SCREEN_H / 2 + 1, SCREEN_W / 3 - 1,
                        SCREEN_H / 2 - 2};
const int lamp6Co[4] = {2 * SCREEN_W / 3, SCREEN_H / 2 + 1, SCREEN_W / 3,
                        SCREEN_H / 2 - 2};
#else
#ifdef LAMP5_ENABLED
const int lamp1Co[4] = {1, 1, SCREEN_W / 3 - 1, SCREEN_H / 2 - 1};
const int lamp2Co[4] = {SCREEN_W / 3 + 1, 1, SCREEN_W / 3 - 1,
                        SCREEN_H / 2 - 1};
const int lamp3Co[4] = {2 * SCREEN_W / 3, 1, SCREEN_W / 3, SCREEN_H / 2 - 1};
const int lamp4Co[4] = {1, SCREEN_H / 2 + 1, SCREEN_W / 2 - 1,
                        SCREEN_H / 2 - 2};
const int lamp5Co[4] = {SCREEN_W / 2 + 1, SCREEN_H / 2 + 1, SCREEN_W / 2,
                        SCREEN_H / 2 - 2};
#else
#ifdef LAMP4_ENABLED
const int lamp1Co[4] = {1, 1, SCREEN_W / 2 - 1, SCREEN_H / 2 - 1};
const int lamp2Co[4] = {SCREEN_W / 2 + 1, 1, SCREEN_W / 2, SCREEN_H / 2 - 1};
const int lamp3Co[4] = {1, SCREEN_H / 2 + 1, SCREEN_W / 2 - 1,
                        SCREEN_H / 2 - 2};
const int lamp4Co[4] = {SCREEN_W / 2 + 1, SCREEN_H / 2 + 1, SCREEN_W / 2,
                        SCREEN_H / 2 - 2};
#else
#ifdef LAMP3_ENABLED
const int lamp1Co[4] = {1, 1, SCREEN_W / 2 - 1, SCREEN_H / 2 - 1};
const int lamp2Co[4] = {SCREEN_W / 2 + 1, 1, SCREEN_W / 2, SCREEN_H / 2 - 1};
const int lamp3Co[4] = {1, SCREEN_H / 2 + 1, SCREEN_W, SCREEN_H / 2 - 2};
#else
#ifdef LAMP2_ENABLED
const int lamp1Co[4] = {1, 1, SCREEN_W SCREEN_H / 2 - 1};
const int lamp2Co[4] = {1, 1, SCREEN_W, SCREEN_H / 2 - 1};
#else
#ifdef LAMP1_ENABLED
const int lamp1Co[4] = {1, 1, SCREEN_W, SCREEN_H};
#endif
#endif
#endif
#endif
#endif
#endif

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
void Dashboard::globeLightImg(int xInput, int yInput, bool state) {
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
void Dashboard::PhilipsHueImg(int xInput, int yInput, bool state) {
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
void Dashboard::christmasLightsImg(int xInput, int yInput, bool state) {
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
  globeLightImg(x + w / 2, y + h / 2, HIGH);
}

void Dashboard::detailLights(bool sfeerlichtjes, bool groteBol, bool glazenBol,
                             bool berging, bool maanlamp, bool raamversiering) {
#ifdef LAMP1_ENABLED
  // Lamp 1
  display.fillRoundRect(lamp1Co[0], lamp1Co[1], lamp1Co[2], lamp1Co[3], 10,
                        publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(19 + lamp1Co[0], 19 + lamp1Co[1]);
  display.print(lamp1);
#endif
#ifdef LAMP2_ENABLED
  // Lamp 2
  display.fillRoundRect(lamp2Co[0], lamp2Co[1], lamp2Co[2], lamp2Co[3], 10,
                        publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(19 + lamp2Co[0], 19 + lamp2Co[1]);
  display.print(lamp2);
#endif
#ifdef LAMP3_ENABLED
  // Lamp 3
  display.fillRoundRect(lamp3Co[0], lamp3Co[1], lamp3Co[2], lamp3Co[3], 10,
                        publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(19 + lamp3Co[0], 19 + lamp3Co[1]);
  display.print(lamp3);
#endif
#ifdef LAMP4_ENABLED
  // Lamp 4
  display.fillRoundRect(lamp4Co[0], lamp4Co[1], lamp4Co[2], lamp4Co[3], 10,
                        publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(19 + lamp4Co[0], 19 + lamp4Co[1]);
  display.print(lamp4);
#endif
#ifdef LAMP5_ENABLED
  // Lamp 5
  display.fillRoundRect(lamp5Co[0], lamp5Co[1], lamp5Co[2], lamp5Co[3], 10,
                        publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(19 + lamp5Co[0], 19 + lamp5Co[1]);
  display.print(lamp5);
#endif
#ifdef LAMP6_ENABLED
  // Lamp 6
  display.fillRoundRect(lamp6Co[0], lamp6Co[1], lamp6Co[2], lamp6Co[3], 10,
                        publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(19 + lamp6Co[0], 19 + lamp6Co[1]);
  display.print(lamp6);
#endif
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

void Dashboard::lightRefresh(int lampnumber, bool state) {
  switch (lampnumber) {
#ifdef LAMP1_ENABLED
    case 1:
      if (lamp1type == "Christmas lights") {
        christmasLightsImg((lamp1Co[0] + lamp1Co[2] / 2),
                           (lamp1Co[1] + lamp1Co[3] / 2), state);
      } else if (lamp1type == "Globe lamp") {
        globeLightImg((lamp1Co[0] + lamp1Co[2] / 2),
                      (lamp1Co[1] + lamp1Co[3] / 2), state);
      } else if (lamp1type == "Moonlamp") {
        moonlampImg((lamp1Co[0] + lamp1Co[2] / 2),
                    (lamp1Co[1] + lamp1Co[3] / 2), state);
      } else if (lamp1type == "Philips Hue") {
        PhilipsHueImg((lamp1Co[0] + lamp1Co[2] / 2),
                      (lamp1Co[1] + lamp1Co[3] / 2), state);
      }
      break;
#endif
#ifdef LAMP2_ENABLED
    case 2:
      if (lamp2type == "Christmas lights") {
        christmasLightsImg((lamp2Co[0] + lamp2Co[2] / 2),
                           (lamp2Co[1] + lamp2Co[3] / 2), state);
      } else if (lamp2type == "Globe lamp") {
        globeLightImg((lamp2Co[0] + lamp2Co[2] / 2),
                      (lamp2Co[1] + lamp2Co[3] / 2), state);
      } else if (lamp2type == "Moonlamp") {
        moonlampImg((lamp2Co[0] + lamp2Co[2] / 2),
                    (lamp2Co[1] + lamp2Co[3] / 2), state);
      } else if (lamp2type == "Philips Hue") {
        PhilipsHueImg((lamp2Co[0] + lamp2Co[2] / 2),
                      (lamp2Co[1] + lamp2Co[3] / 2), state);
      }
      break;
#endif
#ifdef LAMP3_ENABLED
    case 3:
      if (lamp3type == "Christmas lights") {
        christmasLightsImg((lamp3Co[0] + lamp3Co[2] / 2),
                           (lamp3Co[1] + lamp3Co[3] / 2), state);
      } else if (lamp3type == "Globe lamp") {
        globeLightImg((lamp3Co[0] + lamp3Co[2] / 2),
                      (lamp3Co[1] + lamp3Co[3] / 2), state);
      } else if (lamp3type == "Moonlamp") {
        moonlampImg((lamp3Co[0] + lamp3Co[2] / 2),
                    (lamp3Co[1] + lamp3Co[3] / 2), state);
      } else if (lamp3type == "Philips Hue") {
        PhilipsHueImg((lamp3Co[0] + lamp3Co[2] / 2),
                      (lamp3Co[1] + lamp3Co[3] / 2), state);
      }
      break;
#endif
#ifdef LAMP4_ENABLED
    case 4:
      if (lamp4type == "Christmas lights") {
        christmasLightsImg((lamp4Co[0] + lamp4Co[2] / 2),
                           (lamp4Co[1] + lamp4Co[3] / 2), state);
      } else if (lamp4type == "Globe lamp") {
        globeLightImg((lamp4Co[0] + lamp4Co[2] / 2),
                      (lamp4Co[1] + lamp4Co[3] / 2), state);
      } else if (lamp4type == "Moonlamp") {
        moonlampImg((lamp4Co[0] + lamp4Co[2] / 2),
                    (lamp4Co[1] + lamp4Co[3] / 2), state);
      } else if (lamp4type == "Philips Hue") {
        PhilipsHueImg((lamp4Co[0] + lamp4Co[2] / 2),
                      (lamp4Co[1] + lamp4Co[3] / 2), state);
      }
      break;
#endif
#ifdef LAMP5_ENABLED
    case 5:
      if (lamp5type == "Christmas lights") {
        christmasLightsImg((lamp5Co[0] + lamp5Co[2] / 2),
                           (lamp5Co[1] + lamp5Co[3] / 2), state);
      } else if (lamp5type == "Globe lamp") {
        globeLightImg((lamp5Co[0] + lamp5Co[2] / 2),
                      (lamp5Co[1] + lamp5Co[3] / 2), state);
      } else if (lamp5type == "Moonlamp") {
        moonlampImg((lamp5Co[0] + lamp5Co[2] / 2),
                    (lamp5Co[1] + lamp5Co[3] / 2), state);
      } else if (lamp5type == "Philips Hue") {
        PhilipsHueImg((lamp5Co[0] + lamp5Co[2] / 2),
                      (lamp5Co[1] + lamp5Co[3] / 2), state);
      }
      break;
#endif
#ifdef LAMP6_ENABLED
    case 6:
      if (lamp6type == "Christmas lights") {
        christmasLightsImg((lamp6Co[0] + lamp6Co[2] / 2),
                           (lamp6Co[1] + lamp6Co[3] / 2), state);
      } else if (lamp6type == "Globe lamp") {
        globeLightImg((lamp6Co[0] + lamp6Co[2] / 2),
                      (lamp6Co[1] + lamp6Co[3] / 2), state);
      } else if (lamp6type == "Moonlamp") {
        moonlampImg((lamp6Co[0] + lamp6Co[2] / 2),
                    (lamp6Co[1] + lamp6Co[3] / 2), state);
      } else if (lamp6type == "Philips Hue") {
        PhilipsHueImg((lamp6Co[0] + lamp6Co[2] / 2),
                      (lamp6Co[1] + lamp6Co[3] / 2), state);
      }
      break;
#endif
  }
}
#endif