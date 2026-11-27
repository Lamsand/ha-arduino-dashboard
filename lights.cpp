#include "lights.h"
#include "Arduino.h"
#include "Arduino_GigaDisplayTouch.h"
#include "Colors.h"
#include "Dashboard.h"
#include "config.h"
#include <ArduinoMqttClient.h>

extern int lastTouch;

#ifdef ENABLE_LIGHTS
/*  Lights::lampXimg = image of lamp X
    -  "Christmas lights"
    -  "Globe lamp"
    -  "Moonlamp"
    -  "Philips Hue"
*/

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
      if (lights.lamp1img == "Christmas lights") {
        christmasLightsImg((lights.lamp1co[0] + lights.lamp1co[2] / 2),
                           (lights.lamp1co[1] + lights.lamp1co[3] / 2 + 14),
                           state);
      } else if (lights.lamp1img == "Globe lamp") {
        globeLightImg((lights.lamp1co[0] + lights.lamp1co[2] / 2),
                      (lights.lamp1co[1] + lights.lamp1co[3] / 2 + 14), state);
      } else if (lights.lamp1img == "Moonlamp") {
        moonlampImg((lights.lamp1co[0] + lights.lamp1co[2] / 2),
                    (lights.lamp1co[1] + lights.lamp1co[3] / 2 + 14), state);
      } else if (lights.lamp1img == "Philips Hue") {
        PhilipsHueImg((lights.lamp1co[0] + lights.lamp1co[2] / 2),
                      (lights.lamp1co[1] + lights.lamp1co[3] / 2 + 14), state);
      }
    }
    break;
  case 2:
    if (lights.lamp2enabled) {
      if (lights.lamp2img == "Christmas lights") {
        christmasLightsImg((lights.lamp2co[0] + lights.lamp2co[2] / 2),
                           (lights.lamp2co[1] + lights.lamp2co[3] / 2 + 14),
                           state);
      } else if (lights.lamp2img == "Globe lamp") {
        globeLightImg((lights.lamp2co[0] + lights.lamp2co[2] / 2),
                      (lights.lamp2co[1] + lights.lamp2co[3] / 2 + 14), state);
      } else if (lights.lamp2img == "Moonlamp") {
        moonlampImg((lights.lamp2co[0] + lights.lamp2co[2] / 2),
                    (lights.lamp2co[1] + lights.lamp2co[3] / 2 + 14), state);
      } else if (lights.lamp2img == "Philips Hue") {
        PhilipsHueImg((lights.lamp2co[0] + lights.lamp2co[2] / 2),
                      (lights.lamp2co[1] + lights.lamp2co[3] / 2 + 14), state);
      }
    }
    break;
  case 3:
    if (lights.lamp3enabled) {
      if (lights.lamp3img == "Christmas lights") {
        christmasLightsImg((lights.lamp3co[0] + lights.lamp3co[2] / 2),
                           (lights.lamp3co[1] + lights.lamp3co[3] / 2 + 14),
                           state);
      } else if (lights.lamp3img == "Globe lamp") {
        globeLightImg((lights.lamp3co[0] + lights.lamp3co[2] / 2),
                      (lights.lamp3co[1] + lights.lamp3co[3] / 2 + 14), state);
      } else if (lights.lamp3img == "Moonlamp") {
        moonlampImg((lights.lamp3co[0] + lights.lamp3co[2] / 2),
                    (lights.lamp3co[1] + lights.lamp3co[3] / 2 + 14), state);
      } else if (lights.lamp3img == "Philips Hue") {
        PhilipsHueImg((lights.lamp3co[0] + lights.lamp3co[2] / 2),
                      (lights.lamp3co[1] + lights.lamp3co[3] / 2 + 14), state);
      }
    }
    break;
  case 4:
    if (lights.lamp4enabled) {
      if (lights.lamp4img == "Christmas lights") {
        christmasLightsImg((lights.lamp4co[0] + lights.lamp4co[2] / 2),
                           (lights.lamp4co[1] + lights.lamp4co[3] / 2 + 14),
                           state);
      } else if (lights.lamp4img == "Globe lamp") {
        globeLightImg((lights.lamp4co[0] + lights.lamp4co[2] / 2),
                      (lights.lamp4co[1] + lights.lamp4co[3] / 2 + 14), state);
      } else if (lights.lamp4img == "Moonlamp") {
        moonlampImg((lights.lamp4co[0] + lights.lamp4co[2] / 2),
                    (lights.lamp4co[1] + lights.lamp4co[3] / 2 + 14), state);
      } else if (lights.lamp4img == "Philips Hue") {
        PhilipsHueImg((lights.lamp4co[0] + lights.lamp4co[2] / 2),
                      (lights.lamp4co[1] + lights.lamp4co[3] / 2 + 14), state);
      }
    }
    break;
  case 5:
    if (lights.lamp5enabled) {
      if (lights.lamp5img == "Christmas lights") {
        christmasLightsImg((lights.lamp5co[0] + lights.lamp5co[2] / 2),
                           (lights.lamp5co[1] + lights.lamp5co[3] / 2 + 14),
                           state);
      } else if (lights.lamp5img == "Globe lamp") {
        globeLightImg((lights.lamp5co[0] + lights.lamp5co[2] / 2),
                      (lights.lamp5co[1] + lights.lamp5co[3] / 2 + 14), state);
      } else if (lights.lamp5img == "Moonlamp") {
        moonlampImg((lights.lamp5co[0] + lights.lamp5co[2] / 2),
                    (lights.lamp5co[1] + lights.lamp5co[3] / 2 + 14), state);
      } else if (lights.lamp5img == "Philips Hue") {
        PhilipsHueImg((lights.lamp5co[0] + lights.lamp5co[2] / 2),
                      (lights.lamp5co[1] + lights.lamp5co[3] / 2 + 14), state);
      }
    }
    break;
  case 6:
    if (lights.lamp6enabled) {
      if (lights.lamp6img == "Christmas lights") {
        christmasLightsImg((lights.lamp6co[0] + lights.lamp6co[2] / 2),
                           (lights.lamp6co[1] + lights.lamp6co[3] / 2 + 14),
                           state);
      } else if (lights.lamp6img == "Globe lamp") {
        globeLightImg((lights.lamp6co[0] + lights.lamp6co[2] / 2),
                      (lights.lamp6co[1] + lights.lamp6co[3] / 2 + 14), state);
      } else if (lights.lamp6img == "Moonlamp") {
        moonlampImg((lights.lamp6co[0] + lights.lamp6co[2] / 2),
                    (lights.lamp6co[1] + lights.lamp6co[3] / 2 + 14), state);
      } else if (lights.lamp6img == "Philips Hue") {
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
void Lights::mqtt(const String &topic, const String &message,
                  bool &newHomeInfo) {
  Lights &lights = *this;
  String normalizedMessage = message;
  if (topic.startsWith("lights/")) {
    if (normalizedMessage != "Switch") {
      if (topic.startsWith("lights/lamp1")) {
        if (topic == "lights/lamp1") {
          if (normalizedMessage == "off") {
            lights.lamp1state = 0;
            lights.lamp1brightness = 0;
          } else {
            lights.lamp1state = 1;
            lights.lamp1brightness = normalizedMessage.toInt() * 100 / 255;
          }
          lights.lamp1change = HIGH;
        }
        if (topic == "lights/lamp1/name") {
          lights.lamp1name = normalizedMessage;
        }
        if (topic == "lights/lamp1/on-off") {
          if (normalizedMessage == "off") {
            lights.lamp1OnOff = 0;
          } else {
            lights.lamp1OnOff = 1;
          }
        }
        if (topic == "lights/lamp1/enabled") {
          if (normalizedMessage == "on") {
            lights.lamp1enabled = 1;
          } else {
            lights.lamp1enabled = 0;
          }
          lights.reconfigureCoordinates();
        }
        if (topic == "lights/lamp1/image") {
          lights.lamp1img = normalizedMessage;
          if (normalizedMessage == "Custom") {
            lights.lamp1cstmImg = HIGH;
          } else {
            lights.lamp1cstmImg = LOW;
          }
        }
        if (topic == "lights/lamp1/image/custom") {
          if (lights.lamp1cstmImg) {
            lights.lamp1img = normalizedMessage;
          }
        }
      }
      if (topic.startsWith("lights/lamp2")) {
        if (topic == "lights/lamp2") {
          if (normalizedMessage == "off") {
            lights.lamp2state = 0;
            lights.lamp2brightness = 0;
          } else {
            lights.lamp2state = 1;
            lights.lamp2brightness = normalizedMessage.toInt() * 100 / 255;
          }
          lights.lamp2change = HIGH;
        }
        if (topic == "lights/lamp2/name") {
          lights.lamp2name = normalizedMessage;
        }
        if (topic == "lights/lamp2/on-off") {
          if (normalizedMessage == "off") {
            lights.lamp2OnOff = 0;
          } else {
            lights.lamp2OnOff = 1;
          }
        }
        if (topic == "lights/lamp2/enabled") {
          if (normalizedMessage == "on") {
            lights.lamp2enabled = 1;
          } else {
            lights.lamp2enabled = 0;
          }
          lights.reconfigureCoordinates();
        }
        if (topic == "lights/lamp2/image") {
          lights.lamp2img = normalizedMessage;
          if (normalizedMessage == "Custom") {
            lights.lamp2cstmImg = HIGH;
          } else {
            lights.lamp2cstmImg = LOW;
          }
        }
        if (topic == "lights/lamp2/image/custom") {
          if (lights.lamp2cstmImg) {
            lights.lamp2img = normalizedMessage;
          }
        }
      }
      if (topic.startsWith("lights/lamp3")) {
        if (topic == "lights/lamp3") {
          if (normalizedMessage == "off") {
            lights.lamp3state = 0;
            lights.lamp3brightness = 0;
          } else {
            lights.lamp3state = 1;
            lights.lamp3brightness = normalizedMessage.toInt() * 100 / 255;
          }
          lights.lamp3change = HIGH;
        }
        if (topic == "lights/lamp3/name") {
          lights.lamp3name = normalizedMessage;
        }
        if (topic == "lights/lamp3/on-off") {
          if (normalizedMessage == "off") {
            lights.lamp3OnOff = 0;
          } else {
            lights.lamp3OnOff = 1;
          }
        }
        if (topic == "lights/lamp3/enabled") {
          if (normalizedMessage == "on") {
            lights.lamp3enabled = 1;
          } else {
            lights.lamp3enabled = 0;
          }
          lights.reconfigureCoordinates();
        }
        if (topic == "lights/lamp3/image") {
          lights.lamp3img = normalizedMessage;
          if (normalizedMessage == "Custom") {
            lights.lamp3cstmImg = HIGH;
          } else {
            lights.lamp3cstmImg = LOW;
          }
        }
        if (topic == "lights/lamp3/image/custom") {
          if (lights.lamp3cstmImg) {
            lights.lamp3img = normalizedMessage;
          }
        }
      }
      if (topic.startsWith("lights/lamp4")) {
        if (topic == "lights/lamp4") {
          if (normalizedMessage == "off") {
            lights.lamp4state = 0;
            lights.lamp4brightness = 0;
          } else {
            lights.lamp4state = 1;
            lights.lamp4brightness = normalizedMessage.toInt() * 100 / 255;
          }
          lights.lamp4change = HIGH;
        }
        if (topic == "lights/lamp4/name") {
          lights.lamp4name = normalizedMessage;
        }
        if (topic == "lights/lamp4/on-off") {
          if (normalizedMessage == "off") {
            lights.lamp4OnOff = 0;
          } else {
            lights.lamp4OnOff = 1;
          }
        }
        if (topic == "lights/lamp4/enabled") {
          if (normalizedMessage == "on") {
            lights.lamp4enabled = 1;
          } else {
            lights.lamp4enabled = 0;
          }
          lights.reconfigureCoordinates();
        }
        if (topic == "lights/lamp4/image") {
          lights.lamp4img = normalizedMessage;
          if (normalizedMessage == "Custom") {
            lights.lamp4cstmImg = HIGH;
          } else {
            lights.lamp4cstmImg = LOW;
          }
        }
        if (topic == "lights/lamp4/image/custom") {
          if (lights.lamp4cstmImg) {
            lights.lamp4img = normalizedMessage;
          }
        }
      }
      if (topic.startsWith("lights/lamp5")) {
        if (topic == "lights/lamp5") {
          if (normalizedMessage == "off") {
            lights.lamp5state = 0;
            lights.lamp5brightness = 0;
          } else {
            lights.lamp5state = 1;
            lights.lamp5brightness = normalizedMessage.toInt() * 100 / 255;
          }
          lights.lamp5change = HIGH;
        }
        if (topic == "lights/lamp5/name") {
          lights.lamp5name = normalizedMessage;
        }
        if (topic == "lights/lamp5/on-off") {
          if (normalizedMessage == "off") {
            lights.lamp5OnOff = 0;
          } else {
            lights.lamp5OnOff = 1;
          }
        }
        if (topic == "lights/lamp5/enabled") {
          if (normalizedMessage == "on") {
            lights.lamp5enabled = 1;
          } else {
            lights.lamp5enabled = 0;
          }
          lights.reconfigureCoordinates();
        }
        if (topic == "lights/lamp5/image") {
          lights.lamp5img = normalizedMessage;
          if (normalizedMessage == "Custom") {
            lights.lamp5cstmImg = HIGH;
          } else {
            lights.lamp5cstmImg = LOW;
          }
        }
        if (topic == "lights/lamp5/image/custom") {
          if (lights.lamp5cstmImg) {
            lights.lamp5img = normalizedMessage;
          }
        }
      }
      if (topic.startsWith("lights/lamp6")) {
        if (topic == "lights/lamp6") {
          if (normalizedMessage == "off") {
            lights.lamp6state = 0;
            lights.lamp6brightness = 0;
          } else {
            lights.lamp6state = 1;
            lights.lamp6brightness = normalizedMessage.toInt() * 100 / 255;
          }
          lights.lamp6change = HIGH;
        }
        if (topic == "lights/lamp6/name") {
          lights.lamp6name = normalizedMessage;
        }
        if (topic == "lights/lamp6/on-off") {
          if (normalizedMessage == "off") {
            lights.lamp6OnOff = 0;
          } else {
            lights.lamp6OnOff = 1;
          }
        }
        if (topic == "lights/lamp6/enabled") {
          if (normalizedMessage == "on") {
            lights.lamp6enabled = 1;
          } else {
            lights.lamp6enabled = 0;
          }
          lights.reconfigureCoordinates();
        }
        if (topic == "lights/lamp6/image") {
          lights.lamp6img = normalizedMessage;
          if (normalizedMessage == "Custom") {
            lights.lamp6cstmImg = HIGH;
          } else {
            lights.lamp6cstmImg = LOW;
          }
        }
        if (topic == "lights/lamp6/image/custom") {
          if (lights.lamp6cstmImg) {
            lights.lamp6img = normalizedMessage;
          }
        }
      }
    }
  }
}
void Lights::newInfo(Dashboard &dashboard, const String &page,
                     const int *primes, int homePage) {
  if (lamp1change || lamp2change || lamp3change || lamp4change || lamp6change ||
      lamp5change) {
    if (page.startsWith("detailLights")) {
      if (lamp1change) {
        dashboard.lightRefresh(*this, 1, lamp1state);
      }
      if (lamp2change) {
        dashboard.lightRefresh(*this, 2, lamp2state);
      }
      if (lamp3change) {
        dashboard.lightRefresh(*this, 3, lamp3state);
      }
      if (lamp4change) {
        dashboard.lightRefresh(*this, 4, lamp4state);
      }
      if (lamp5change) {
        dashboard.lightRefresh(*this, 5, lamp5state);
      }
      if (lamp6change) {
        dashboard.lightRefresh(*this, 6, lamp6state);
      }
      if (page.startsWith("detailLights/") == HIGH) {
        dashboard.lightSw();
        if (page.endsWith("lamp1") == HIGH) {
          dashboard.lightBr(lamp1brightness);
        } else if (page.endsWith("lamp2") == HIGH) {
          dashboard.lightBr(lamp2brightness);
        } else if (page.endsWith("lamp3") == HIGH) {
          dashboard.lightBr(lamp3brightness);
        } else if (page.endsWith("lamp4") == HIGH) {
          dashboard.lightBr(lamp4brightness);
        } else if (page.endsWith("lamp5") == HIGH) {
          dashboard.lightBr(lamp5brightness);
        } else if (page.endsWith("lamp6") == HIGH) {
          dashboard.lightBr(lamp6brightness);
        }
      }
    }
    lamp1change = LOW;
    lamp2change = LOW;
    lamp3change = LOW;
    lamp4change = LOW;
    lamp5change = LOW;
    lamp6change = LOW;
  }
}
bool Lights::touchHandler(Dashboard &dashboard, MqttClient &mqttClient,
                          Arduino_GigaDisplayTouch &touchDetector, int touch_x,
                          int touch_y, String &page) {
  bool handled = LOW;
  uint8_t contacts;
  GDTpoint_t points[5];
  if (page == "detailLights") {
    handled = HIGH;
    if (lamp1enabled) {
      if (touch_x > lamp1co[0] && touch_x < lamp1co[0] + lamp1co[2] &&
          touch_y > lamp1co[1] && touch_y < lamp1co[1] + lamp1co[3]) { // Lamp 1
        if (lamp1OnOff) {
          mqttClient.beginMessage("lights/lamp1");
          mqttClient.print("Switch");
          mqttClient.endMessage();
        } else {
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
            page = "detailLights/lamp1";
            dashboard.lightSw();
            dashboard.lightBr(lamp1brightness);
          } else {
            mqttClient.beginMessage("lights/lamp1");
            mqttClient.print("Switch");
            mqttClient.endMessage();
          }
        }
      }
    }
    if (lamp2enabled) {
      if (touch_x > lamp2co[0] && touch_x < lamp2co[0] + lamp2co[2] &&
          touch_y > lamp2co[1] && touch_y < lamp2co[1] + lamp2co[3]) { // Lamp 2
        if (lamp2OnOff) {
          mqttClient.beginMessage("lights/lamp2");
          mqttClient.print("Switch");
          mqttClient.endMessage();
        } else {
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
            page = "detailLights/lamp2";
            dashboard.lightSw();
            dashboard.lightBr(lamp2brightness);
          } else {
            mqttClient.beginMessage("lights/lamp2");
            mqttClient.print("Switch");
            mqttClient.endMessage();
          }
        }
      }
    }
    if (lamp3enabled) {
      if (touch_x > lamp3co[0] && touch_x < lamp3co[0] + lamp3co[2] &&
          touch_y > lamp3co[1] && touch_y < lamp3co[1] + lamp3co[3]) { // Lamp 3
        if (lamp3OnOff) {
          mqttClient.beginMessage("lights/lamp3");
          mqttClient.print("Switch");
          mqttClient.endMessage();
        } else {
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
            page = "detailLights/lamp3";
            dashboard.lightSw();
            dashboard.lightBr(lamp3brightness);
          } else {
            mqttClient.beginMessage("lights/lamp3");
            mqttClient.print("Switch");
            mqttClient.endMessage();
          }
        }
      }
    }
    if (lamp4enabled) {
      if (touch_x > lamp4co[0] && touch_x < lamp4co[0] + lamp4co[2] &&
          touch_y > lamp4co[1] && touch_y < lamp4co[1] + lamp4co[3]) { // Lamp 4
        if (lamp5OnOff) {
          mqttClient.beginMessage("lights/lamp4");
          mqttClient.print("Switch");
          mqttClient.endMessage();
        } else {
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
            page = "detailLights/lamp4";
            dashboard.lightSw();
            dashboard.lightBr(lamp5brightness);
          } else {
            mqttClient.beginMessage("lights/lamp4");
            mqttClient.print("Switch");
            mqttClient.endMessage();
          }
        }
      }
    }
    if (lamp5enabled) {
      if (touch_x > lamp5co[0] && touch_x < lamp5co[0] + lamp5co[2] &&
          touch_y > lamp5co[1] && touch_y < lamp5co[1] + lamp5co[3]) { // Lamp 5
        if (lamp5OnOff) {
          mqttClient.beginMessage("lights/lamp5");
          mqttClient.print("Switch");
          mqttClient.endMessage();
        } else {
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
            page = "detailLights/lamp5";
            dashboard.lightSw();
            dashboard.lightBr(lamp5brightness);
          } else {
            mqttClient.beginMessage("lights/lamp5");
            mqttClient.print("Switch");
            mqttClient.endMessage();
          }
        }
      }
    }
    if (lamp6enabled) {
      if (touch_x > lamp6co[0] && touch_x < lamp6co[0] + lamp6co[2] &&
          touch_y > lamp6co[1] && touch_y < lamp6co[1] + lamp6co[3]) { // Lamp 6
        if (lamp6OnOff) {
          mqttClient.beginMessage("lights/lamp6");
          mqttClient.print("Switch");
          mqttClient.endMessage();
        } else {
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
            page = "detailLights/lamp6";
            dashboard.lightSw();
            dashboard.lightBr(lamp6brightness);
          } else {
            mqttClient.beginMessage("lights/lamp6");
            mqttClient.print("Switch");
            mqttClient.endMessage();
          }
        }
      }
    }
  } else if (page.startsWith("detailLights/") == HIGH) {
    handled = HIGH;
    String k = page;
    k.replace("detailLights/", "");
    Serial.println(k);
    if ((touch_x < 300 || touch_x > 500) ||
        (touch_y < 40 || touch_y > 440)) { // Return
      Serial.println("return");
      page = "detailLights";
      dashboard.fillScreen(dashboard.publicAllBackg);
      dashboard.detailLights(*this);
      dashboard.lightRefresh(*this, 1, lamp1state);
      dashboard.lightRefresh(*this, 2, lamp2state);
      dashboard.lightRefresh(*this, 3, lamp3state);
      dashboard.lightRefresh(*this, 4, lamp4state);
      dashboard.lightRefresh(*this, 5, lamp5state);
      dashboard.lightRefresh(*this, 6, lamp6state);
    }
    if (touch_x > 300 && touch_x < 500 && touch_y > 340 &&
        touch_y < 440) { // O/I button clicked
      mqttClient.beginMessage("lights/" + k);
      mqttClient.print("Switch");
      mqttClient.endMessage();
    }
  }
  return handled;
}
void Lights::setCoordinates(int *energyCo) {
  x = energyCo[0];
  y = energyCo[1];
  w = energyCo[2];
  h = energyCo[3];
  prime = energyCo[4];
}
bool Lights::clicked(int touch_x, int touch_y, int primePageNr) {
  return (touch_x > x && touch_x < x + w && touch_y > y && touch_y < y + h &&
          onHomePage(primePageNr));
}
bool Lights::onHomePage(int primePageNr) {
  return (prime != 0 && prime % primePageNr == 0);
}
#endif