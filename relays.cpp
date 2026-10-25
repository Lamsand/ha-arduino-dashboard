#include "Arduino.h"
#include "Colors.h"
#include "Dashboard.h"
#include "config.h"

#ifdef ENABLE_RELAYS
/*  relayX = name of relay X
    relayXtype = type of relay X
    -  "Automation"
    -  "Switch"
*/
const char *relay1 = "Quooker";
const char *relay1type = "Switch";

const char *relay2 = "Passieve koeling";
const char *relay2type = "Switch";

const char *relay3 = "Warmtepomp";
const char *relay3type = "Automation";

const char *relay4 = "Relay 4";
const char *relay4type = "Switch";

const char *relay5 = "Relay 5";
const char *relay5type = "Switch";

const char *relay6 = "Relay 6";
const char *relay6type = "Switch";

#ifdef RELAY6_ENABLED
const int relay1Co[4] = {1, 1, SCREEN_W / 3 - 1, SCREEN_H / 2 - 1};
const int relay2Co[4] = {SCREEN_W / 3 + 1, 1, SCREEN_W / 3 - 1,
                         SCREEN_H / 2 - 1};
const int relay3Co[4] = {2 * SCREEN_W / 3, 1, SCREEN_W / 3, SCREEN_H / 2 - 1};
const int relay4Co[4] = {1, SCREEN_H / 2 + 1, SCREEN_W / 3 - 1,
                         SCREEN_H / 2 - 2};
const int relay5Co[4] = {SCREEN_W / 3 + 1, SCREEN_H / 2 + 1, SCREEN_W / 3 - 1,
                         SCREEN_H / 2 - 2};
const int relay6Co[4] = {2 * SCREEN_W / 3, SCREEN_H / 2 + 1, SCREEN_W / 3,
                         SCREEN_H / 2 - 2};
#else
#ifdef RELAY5_ENABLED
const int relay1Co[4] = {1, 1, SCREEN_W / 3 - 1, SCREEN_H / 2 - 1};
const int relay2Co[4] = {SCREEN_W / 3 + 1, 1, SCREEN_W / 3 - 1,
                         SCREEN_H / 2 - 1};
const int relay3Co[4] = {2 * SCREEN_W / 3, 1, SCREEN_W / 3, SCREEN_H / 2 - 1};
const int relay4Co[4] = {1, SCREEN_H / 2 + 1, SCREEN_W / 3 - 1,
                         SCREEN_H / 2 - 2};
const int relay5Co[4] = {SCREEN_W / 2 + 1, SCREEN_H / 2 + 1, SCREEN_W / 2,
                         SCREEN_H / 2 - 2};
#else
#ifdef RELAY4_ENABLED
const int relay1Co[4] = {1, 1, SCREEN_W / 2 - 1, SCREEN_H / 2 - 1};
const int relay2Co[4] = {SCREEN_W / 2 + 1, 1, SCREEN_W / 2, SCREEN_H / 2 - 1};
const int relay3Co[4] = {1, SCREEN_H / 2 + 1, SCREEN_W / 2 - 1,
                         SCREEN_H / 2 - 2};
const int relay4Co[4] = {SCREEN_W / 2 + 1, SCREEN_H / 2 + 1, SCREEN_W / 2,
                         SCREEN_H / 2 - 2};
#else
#ifdef RELAY3_ENABLED
const int relay1Co[4] = {1, 1, SCREEN_W / 2 - 1, SCREEN_H / 2 - 1};
const int relay2Co[4] = {SCREEN_W / 2 + 1, 1, SCREEN_W / 2, SCREEN_H / 2 - 1};
const int relay3Co[4] = {1, SCREEN_H / 2 + 1, SCREEN_W, SCREEN_H / 2 - 2};
#else
#ifdef RELAY2_ENABLED
const int relay1Co[4] = {1, 1, SCREEN_W / 2, SCREEN_H / 2 - 1};
const int relay2Co[4] = {SCREEN_W / 2 + 1, 1, SCREEN_W / 2, SCREEN_H / 2 - 1};
#else
#ifdef RELAY1_ENABLED
const int relay1Co[4] = {1, 1, SCREEN_W, SCREEN_H};
#endif
#endif
#endif
#endif
#endif
#endif

void Dashboard::automationImg(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("Relays/automationDH.bin");
    } else {
      file = sd.open("Relays/automationLH.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("Relays/automationDL.bin");
    } else {
      file = sd.open("Relays/automationLL.bin");
    }
  }
  if (!file)
    return;
  file.seek(4); // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 187;
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
void Dashboard::switchImg(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("Relays/switchDH.bin");
    } else {
      file = sd.open("Relays/switchLH.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("Relays/switchDL.bin");
    } else {
      file = sd.open("Relays/switchLL.bin");
    }
  }
  if (!file)
    return;
  file.seek(4); // Skip the 4-byte header of the file
  int y = 0;
  const int h = 86;
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

void Dashboard::homeRelaysImg(int x, int y, int w, int h) {
  switchImg(x + w / 2, y + h / 2, HIGH);
}

void Dashboard::detailRelays() {
#ifdef RELAY1_ENABLED
  // Relay 1
  display.fillRoundRect(relay1Co[0], relay1Co[1], relay1Co[2], relay1Co[3], 10,
                        publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(19 + relay1Co[0], 19 + relay1Co[1]);
  display.print(relay1);
#endif
#ifdef RELAY2_ENABLED
  // Relay 2
  display.fillRoundRect(relay2Co[0], relay2Co[1], relay2Co[2], relay2Co[3], 10,
                        publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(19 + relay2Co[0], 19 + relay2Co[1]);
  display.print(relay2);
#endif
#ifdef RELAY3_ENABLED
  // Relay 3
  display.fillRoundRect(relay3Co[0], relay3Co[1], relay3Co[2], relay3Co[3], 10,
                        publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(19 + relay3Co[0], 19 + relay3Co[1]);
  display.print(relay3);
#endif
#ifdef RELAY4_ENABLED
  // Relay 4
  display.fillRoundRect(relay4Co[0], relay4Co[1], relay4Co[2], relay4Co[3], 10,
                        publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(19 + relay4Co[0], 19 + relay4Co[1]);
  display.print(relay4);
#endif
#ifdef RELAY5_ENABLED
  // Relay 5
  display.fillRoundRect(relay5Co[0], relay5Co[1], relay5Co[2], relay5Co[3], 10,
                        publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(19 + relay5Co[0], 19 + relay5Co[1]);
  display.print(relay5);
#endif
#ifdef RELAY6_ENABLED
  // Relay 6
  display.fillRoundRect(relay6Co[0], relay6Co[1], relay6Co[2], relay6Co[3], 10,
                        publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(19 + relay6Co[0], 19 + relay6Co[1]);
  display.print(relay6);
#endif
}
void Dashboard::relayRefresh(int relaynumber, bool state) {
  switch (relaynumber) {
#ifdef RELAY1_ENABLED
  case 1:
    if (relay1type == "Automation") {
      automationImg((relay1Co[0] + relay1Co[2] / 2),
                    (relay1Co[1] + relay1Co[3] / 2 + 14), state);
    } else if (relay1type == "Switch") {
      switchImg((relay1Co[0] + relay1Co[2] / 2),
                (relay1Co[1] + relay1Co[3] / 2 + 14), state);
    }
    break;
#endif
#ifdef RELAY2_ENABLED
  case 2:
    if (relay2type == "Automation") {
      automationImg((relay2Co[0] + relay2Co[2] / 2),
                    (relay2Co[1] + relay2Co[3] / 2 + 14), state);
    } else if (relay2type == "Switch") {
      switchImg((relay2Co[0] + relay2Co[2] / 2),
                (relay2Co[1] + relay2Co[3] / 2 + 14), state);
    }
    break;
#endif
#ifdef RELAY3_ENABLED
  case 3:
    if (relay3type == "Automation") {
      automationImg((relay3Co[0] + relay3Co[2] / 2),
                    (relay3Co[1] + relay3Co[3] / 2 + 14), state);
    } else if (relay3type == "Switch") {
      switchImg((relay3Co[0] + relay3Co[2] / 2),
                (relay3Co[1] + relay3Co[3] / 2 + 14), state);
    }
    break;
#endif
#ifdef RELAY4_ENABLED
  case 4:
    if (relay4type == "Automation") {
      automationImg((relay4Co[0] + relay4Co[2] / 2),
                    (relay4Co[1] + relay4Co[3] / 2 + 14), state);
    } else if (relay4type == "Switch") {
      switchImg((relay4Co[0] + relay4Co[2] / 2),
                (relay4Co[1] + relay4Co[3] / 2 + 14), state);
    }
    break;
#endif
#ifdef RELAY5_ENABLED
  case 5:
    if (relay5type == "Automation") {
      automationImg((relay5Co[0] + relay5Co[2] / 2),
                    (relay5Co[1] + relay5Co[3] / 2 + 14), state);
    } else if (relay5type == "Switch") {
      switchImg((relay5Co[0] + relay5Co[2] / 2),
                (relay5Co[1] + relay5Co[3] / 2 + 14), state);
    }
    break;
#endif
#ifdef RELAY6_ENABLED
  case 6:
    if (relay6type == "Automation") {
      automationImg((relay6Co[0] + relay6Co[2] / 2),
                    (relay6Co[1] + relay6Co[3] / 2 + 14), state);
    } else if (relay6type == "Switch") {
      switchImg((relay6Co[0] + relay6Co[2] / 2),
                (relay6Co[1] + relay6Co[3] / 2 + 14), state);
    }
    break;
#endif
  }
}

#endif