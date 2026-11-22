#include "lights.h"
#include "Arduino.h"
#include "Colors.h"
#include "Dashboard.h"
#include "config.h"

#ifdef ENABLE_LIGHTS
/*  lampXtype = type of lamp X
    -  "Christmas lights"
    -  "Globe lamp"
    -  "Moonlamp"
    -  "Philips Hue"
*/
const char *lamp1type = "Christmas lights";
const char *lamp2type = "Globe lamp";
const char *lamp3type = "Globe lamp";
const char *lamp4type = "Moonlamp";
const char *lamp5type = "Christmas lights";
const char *lamp6type = "Philips Hue";

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
  if (!file)
    return;
  file.seek(4); // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 162;
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
  if (!file)
    return;
  file.seek(4); // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 100;
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
  if (!file)
    return;
  file.seek(4); // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 156;
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
  if (!file)
    return;
  file.seek(4); // Skip the 4-byte header of the file
  int y = 0;
  const int h = 90;
  const int w = 170;
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

void Dashboard::homeLightsImg(int x, int y, int w, int h) {
  globeLightImg(x + w / 2, y + h / 2, HIGH);
}

void Dashboard::detailLights(const Lights &lights) {
  if (lights.lamp1enabled) {
    // Lamp 1
    display.fillRoundRect(lights.lamp1co[0], lights.lamp1co[1],
                          lights.lamp1co[2], lights.lamp1co[3], 10,
                          publicBackg);
    display.setTextColor(publicTEXT);
    display.setTextSize(2);
    display.setCursor(19 + lights.lamp1co[0], 19 + lights.lamp1co[1]);
    display.print(lights.lamp1name);
  }
  if (lights.lamp2enabled) {
    // Lamp 2
    display.fillRoundRect(lights.lamp2co[0], lights.lamp2co[1],
                          lights.lamp2co[2], lights.lamp2co[3], 10,
                          publicBackg);
    display.setTextColor(publicTEXT);
    display.setTextSize(2);
    display.setCursor(19 + lights.lamp2co[0], 19 + lights.lamp2co[1]);
    display.print(lights.lamp2name);
  }
  if (lights.lamp3enabled) {
    // Lamp 3
    display.fillRoundRect(lights.lamp3co[0], lights.lamp3co[1],
                          lights.lamp3co[2], lights.lamp3co[3], 10,
                          publicBackg);
    display.setTextColor(publicTEXT);
    display.setTextSize(2);
    display.setCursor(19 + lights.lamp3co[0], 19 + lights.lamp3co[1]);
    display.print(lights.lamp3name);
  }
  if (lights.lamp4enabled) {
    // Lamp 4
    display.fillRoundRect(lights.lamp4co[0], lights.lamp4co[1],
                          lights.lamp4co[2], lights.lamp4co[3], 10,
                          publicBackg);
    display.setTextColor(publicTEXT);
    display.setTextSize(2);
    display.setCursor(19 + lights.lamp4co[0], 19 + lights.lamp4co[1]);
    display.print(lights.lamp4name);
  }
  if (lights.lamp5enabled) {
    // Lamp 5
    display.fillRoundRect(lights.lamp5co[0], lights.lamp5co[1],
                          lights.lamp5co[2], lights.lamp5co[3], 10,
                          publicBackg);
    display.setTextColor(publicTEXT);
    display.setTextSize(2);
    display.setCursor(19 + lights.lamp5co[0], 19 + lights.lamp5co[1]);
    display.print(lights.lamp5name);
  }
  if (lights.lamp6enabled) {
    // Lamp 6
    display.fillRoundRect(lights.lamp6co[0], lights.lamp6co[1],
                          lights.lamp6co[2], lights.lamp6co[3], 10,
                          publicBackg);
    display.setTextColor(publicTEXT);
    display.setTextSize(2);
    display.setCursor(19 + lights.lamp6co[0], 19 + lights.lamp6co[1]);
    display.print(lights.lamp6name);
  }
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

void Dashboard::lightRefresh(const Lights &lights, int lampnumber, bool state) {
  switch (lampnumber) {
  case 1:
    if (lights.lamp1enabled) {
      if (lamp1type == "Christmas lights") {
        christmasLightsImg((lights.lamp1co[0] + lights.lamp1co[2] / 2),
                           (lights.lamp1co[1] + lights.lamp1co[3] / 2 + 14),
                           state);
      } else if (lamp1type == "Globe lamp") {
        globeLightImg((lights.lamp1co[0] + lights.lamp1co[2] / 2),
                      (lights.lamp1co[1] + lights.lamp1co[3] / 2 + 14), state);
      } else if (lamp1type == "Moonlamp") {
        moonlampImg((lights.lamp1co[0] + lights.lamp1co[2] / 2),
                    (lights.lamp1co[1] + lights.lamp1co[3] / 2 + 14), state);
      } else if (lamp1type == "Philips Hue") {
        PhilipsHueImg((lights.lamp1co[0] + lights.lamp1co[2] / 2),
                      (lights.lamp1co[1] + lights.lamp1co[3] / 2 + 14), state);
      }
    }
    break;
  case 2:
    if (lights.lamp2enabled) {
      if (lamp2type == "Christmas lights") {
        christmasLightsImg((lights.lamp2co[0] + lights.lamp2co[2] / 2),
                           (lights.lamp2co[1] + lights.lamp2co[3] / 2 + 14),
                           state);
      } else if (lamp2type == "Globe lamp") {
        globeLightImg((lights.lamp2co[0] + lights.lamp2co[2] / 2),
                      (lights.lamp2co[1] + lights.lamp2co[3] / 2 + 14), state);
      } else if (lamp2type == "Moonlamp") {
        moonlampImg((lights.lamp2co[0] + lights.lamp2co[2] / 2),
                    (lights.lamp2co[1] + lights.lamp2co[3] / 2 + 14), state);
      } else if (lamp2type == "Philips Hue") {
        PhilipsHueImg((lights.lamp2co[0] + lights.lamp2co[2] / 2),
                      (lights.lamp2co[1] + lights.lamp2co[3] / 2 + 14), state);
      }
    }
    break;
  case 3:
    if (lights.lamp3enabled) {
      if (lamp3type == "Christmas lights") {
        christmasLightsImg((lights.lamp3co[0] + lights.lamp3co[2] / 2),
                           (lights.lamp3co[1] + lights.lamp3co[3] / 2 + 14),
                           state);
      } else if (lamp3type == "Globe lamp") {
        globeLightImg((lights.lamp3co[0] + lights.lamp3co[2] / 2),
                      (lights.lamp3co[1] + lights.lamp3co[3] / 2 + 14), state);
      } else if (lamp3type == "Moonlamp") {
        moonlampImg((lights.lamp3co[0] + lights.lamp3co[2] / 2),
                    (lights.lamp3co[1] + lights.lamp3co[3] / 2 + 14), state);
      } else if (lamp3type == "Philips Hue") {
        PhilipsHueImg((lights.lamp3co[0] + lights.lamp3co[2] / 2),
                      (lights.lamp3co[1] + lights.lamp3co[3] / 2 + 14), state);
      }
    }
    break;
  case 4:
    if (lights.lamp4enabled) {
      if (lamp4type == "Christmas lights") {
        christmasLightsImg((lights.lamp4co[0] + lights.lamp4co[2] / 2),
                           (lights.lamp4co[1] + lights.lamp4co[3] / 2 + 14),
                           state);
      } else if (lamp4type == "Globe lamp") {
        globeLightImg((lights.lamp4co[0] + lights.lamp4co[2] / 2),
                      (lights.lamp4co[1] + lights.lamp4co[3] / 2 + 14), state);
      } else if (lamp4type == "Moonlamp") {
        moonlampImg((lights.lamp4co[0] + lights.lamp4co[2] / 2),
                    (lights.lamp4co[1] + lights.lamp4co[3] / 2 + 14), state);
      } else if (lamp4type == "Philips Hue") {
        PhilipsHueImg((lights.lamp4co[0] + lights.lamp4co[2] / 2),
                      (lights.lamp4co[1] + lights.lamp4co[3] / 2 + 14), state);
      }
    }
    break;
  case 5:
    if (lights.lamp5enabled) {
      if (lamp5type == "Christmas lights") {
        christmasLightsImg((lights.lamp5co[0] + lights.lamp5co[2] / 2),
                           (lights.lamp5co[1] + lights.lamp5co[3] / 2 + 14),
                           state);
      } else if (lamp5type == "Globe lamp") {
        globeLightImg((lights.lamp5co[0] + lights.lamp5co[2] / 2),
                      (lights.lamp5co[1] + lights.lamp5co[3] / 2 + 14), state);
      } else if (lamp5type == "Moonlamp") {
        moonlampImg((lights.lamp5co[0] + lights.lamp5co[2] / 2),
                    (lights.lamp5co[1] + lights.lamp5co[3] / 2 + 14), state);
      } else if (lamp5type == "Philips Hue") {
        PhilipsHueImg((lights.lamp5co[0] + lights.lamp5co[2] / 2),
                      (lights.lamp5co[1] + lights.lamp5co[3] / 2 + 14), state);
      }
    }
    break;
  case 6:
    if (lights.lamp6enabled) {
      if (lamp6type == "Christmas lights") {
        christmasLightsImg((lights.lamp6co[0] + lights.lamp6co[2] / 2),
                           (lights.lamp6co[1] + lights.lamp6co[3] / 2 + 14),
                           state);
      } else if (lamp6type == "Globe lamp") {
        globeLightImg((lights.lamp6co[0] + lights.lamp6co[2] / 2),
                      (lights.lamp6co[1] + lights.lamp6co[3] / 2 + 14), state);
      } else if (lamp6type == "Moonlamp") {
        moonlampImg((lights.lamp6co[0] + lights.lamp6co[2] / 2),
                    (lights.lamp6co[1] + lights.lamp6co[3] / 2 + 14), state);
      } else if (lamp6type == "Philips Hue") {
        PhilipsHueImg((lights.lamp6co[0] + lights.lamp6co[2] / 2),
                      (lights.lamp6co[1] + lights.lamp6co[3] / 2 + 14), state);
      }
    }
    break;
  }
}

void Lights::reconfigureCoordinates() {
  if (lamp6enabled) {
    lamp1co[0] = 1;
    lamp1co[1] = 1;
    lamp1co[2] = SCREEN_W / 3 - 1;
    lamp1co[3] = SCREEN_H / 2 - 1;
    lamp2co[0] = SCREEN_W / 3 + 1;
    lamp2co[1] = 1;
    lamp2co[2] = SCREEN_W / 3 - 1;
    lamp2co[3] = SCREEN_H / 2 - 1;
    lamp3co[0] = 2 * SCREEN_W / 3;
    lamp3co[1] = 1;
    lamp3co[2] = SCREEN_W / 3;
    lamp3co[3] = SCREEN_H / 2 - 1;
    lamp4co[0] = 1;
    lamp4co[1] = SCREEN_H / 2 + 1;
    lamp4co[2] = SCREEN_W / 3 - 1;
    lamp4co[3] = SCREEN_H / 2 - 2;
    lamp5co[0] = SCREEN_W / 3 + 1;
    lamp5co[1] = SCREEN_H / 2 + 1;
    lamp5co[2] = SCREEN_W / 3 - 1;
    lamp5co[3] = SCREEN_H / 2 - 2;
    lamp6co[0] = 2 * SCREEN_W / 3;
    lamp6co[1] = SCREEN_H / 2 + 1;
    lamp6co[2] = SCREEN_W / 3;
    lamp6co[3] = SCREEN_H / 2 - 2;
  } else if (lamp5enabled) {
    lamp1co[0] = 1;
    lamp1co[1] = 1;
    lamp1co[2] = SCREEN_W / 3 - 1;
    lamp1co[3] = SCREEN_H / 2 - 1;
    lamp2co[0] = SCREEN_W / 3 + 1;
    lamp2co[1] = 1;
    lamp2co[2] = SCREEN_W / 3 - 1;
    lamp2co[3] = SCREEN_H / 2 - 1;
    lamp3co[0] = 2 * SCREEN_W / 3;
    lamp3co[1] = 1;
    lamp3co[2] = SCREEN_W / 3;
    lamp3co[3] = SCREEN_H / 2 - 1;
    lamp4co[0] = 1;
    lamp4co[1] = SCREEN_H / 2 + 1;
    lamp4co[2] = SCREEN_W / 2 - 1;
    lamp4co[3] = SCREEN_H / 2 - 2;
    lamp5co[0] = SCREEN_W / 2 + 1;
    lamp5co[1] = SCREEN_H / 2 + 1;
    lamp5co[2] = SCREEN_W / 2 - 2;
    lamp5co[3] = SCREEN_H / 2 - 2;
  } else if (lamp4enabled) {
    lamp1co[0] = 1;
    lamp1co[1] = 1;
    lamp1co[2] = SCREEN_W / 2 - 1;
    lamp1co[3] = SCREEN_H / 2 - 1;
    lamp2co[0] = SCREEN_W / 2 + 1;
    lamp2co[1] = 1;
    lamp2co[2] = SCREEN_W / 2 - 2;
    lamp2co[3] = SCREEN_H / 2 - 1;
    lamp3co[0] = 1;
    lamp3co[1] = SCREEN_H / 2 + 1;
    lamp3co[2] = SCREEN_W / 2 - 1;
    lamp3co[3] = SCREEN_H / 2 - 2;
    lamp4co[0] = SCREEN_W / 2 + 1;
    lamp4co[1] = SCREEN_H / 2 + 1;
    lamp4co[2] = SCREEN_W / 2 - 2;
    lamp4co[3] = SCREEN_H / 2 - 2;
  } else if (lamp3enabled) {
    lamp1co[0] = 1;
    lamp1co[1] = 1;
    lamp1co[2] = SCREEN_W / 2 - 1;
    lamp1co[3] = SCREEN_H / 2 - 1;
    lamp2co[0] = SCREEN_W / 2 + 1;
    lamp2co[1] = 1;
    lamp2co[2] = SCREEN_W / 2 - 2;
    lamp2co[3] = SCREEN_H / 2 - 1;
    lamp3co[0] = 1;
    lamp3co[1] = SCREEN_H / 2 + 1;
    lamp3co[2] = SCREEN_W - 2;
    lamp3co[3] = SCREEN_H / 2 - 2;
  } else if (lamp2enabled) {
    lamp1co[0] = 1;
    lamp1co[1] = 1;
    lamp1co[2] = SCREEN_W - 2;
    lamp1co[3] = SCREEN_H / 2 - 1;
    lamp2co[0] = 1;
    lamp2co[1] = SCREEN_H / 2 + 1;
    lamp2co[2] = SCREEN_W - 2;
    lamp2co[3] = SCREEN_H / 2 - 2;
  } else if (lamp1enabled) {
    lamp1co[0] = 1;
    lamp1co[1] = 1;
    lamp1co[2] = SCREEN_W - 2;
    lamp1co[3] = SCREEN_H - 2;
  }
}
#endif