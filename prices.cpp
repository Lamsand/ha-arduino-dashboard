#include "Arduino.h"
#include "Colors.h"
#include "Dashboard.h"
#include "config.h"

#ifdef ENABLE_PRICES
float mapFloat(float x, float in_min, float in_max, float out_min,
               float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Dashboard::homePrices(int x, int y, int w, int h, float currentPrice) {
  display.fillRoundRect(x + 1, y + 1, w - 1, h - 1, 10, publicBackg);

  const int spacing = 12;

  char prStr[16];
  char unitStr[16];
  snprintf(prStr, sizeof(prStr), "%.1f", currentPrice);
  snprintf(unitStr, sizeof(unitStr), "ct/kWh");

  display.setTextSize(6);
  display.setTextColor(publicTEXT);

  int16_t tx, ty;
  uint16_t tw, th;
  display.getTextBounds(prStr, 0, 0, &tx, &ty, &tw, &th);

  display.setTextSize(4);
  int16_t ux, uy;
  uint16_t uw, uh;
  display.getTextBounds(unitStr, 0, 0, &ux, &uy, &uw, &uh);

  int16_t totalHeight = th + uh + spacing;
  int16_t startY = y + h / 2 - totalHeight / 2;

  display.setTextSize(6);
  display.setCursor(x + w / 2 - tw / 2, startY);
  display.print(prStr);

  display.setTextSize(4);
  display.setCursor(x + w / 2 - uw / 2, startY + th + spacing);
  display.print(unitStr);
}
void Dashboard::homePricesV(int x, int y, int w, int h, float currentPrice) {
  const int spacing = 12;
  char prStr[16];
  char unitStr[16];
  snprintf(prStr, sizeof(prStr), "%.1f", currentPrice);
  snprintf(unitStr, sizeof(unitStr), "ct/kWh");

  display.setTextSize(6);
  display.setTextColor(publicTEXT, publicBackg);

  int16_t tx, ty;
  uint16_t tw, th;
  display.getTextBounds(prStr, 0, 0, &tx, &ty, &tw, &th);

  display.setTextSize(4);
  int16_t ux, uy;
  uint16_t uw, uh;
  display.getTextBounds(unitStr, 0, 0, &ux, &uy, &uw, &uh);

  int16_t totalHeight = th + uh + spacing;
  int16_t startY = y + h / 2 - totalHeight / 2;

  display.setTextSize(6);
  display.setCursor(x + w / 2 - tw / 2, startY);
  display.print(prStr);

  display.setTextSize(4);
  display.setCursor(x + w / 2 - uw / 2, startY + th + spacing);
  display.print(unitStr);
}
void Dashboard::prices(float* epex, float* ecopower, float* tomorrowEpex,
                       float* tomorrowEcopower, float currentEpex,
                       float currentEcopower, bool tomorrowGraph, bool drawEpex,
                       bool drawEcopower, int clickedX) {
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  // pricesV(currentEpex, currentEcopower, clickedX);
  pricesGraph(epex, ecopower, tomorrowEpex, tomorrowEcopower, currentEpex,
              currentEcopower, tomorrowGraph, drawEpex, drawEcopower, clickedX);
}
void Dashboard::pricesGraph(float* epex, float* ecopower, float* tomorrowEpex,
                            float* tomorrowEcopower, float currentEpex,
                            float currentEcopower, bool tomorrowGraph,
                            bool drawEpex, bool drawEcopower, int clickedX) {
  if (drawEpex) {
    display.fillCircle(460, 370, 10, HABlue);
  } else {
    display.fillCircle(460, 370, 10, publicBackg);
    display.drawCircle(460, 370, 10, HABlue);
  }
  if (drawEcopower) {
    display.fillCircle(460, 420, 10, ORANGE);
  } else {
    display.fillCircle(460, 420, 10, publicBackg);
    display.drawCircle(460, 420, 10, ORANGE);
  }
  display.fillRoundRect(61, 41, 358, 398, 9, publicBackg);
  display.fillRoundRect(450, 40, 340, 44, 10, publicTEXT);

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
    display.print(i * 5);
    display.print(" ct");
  }

  if (tomorrowGraph) {
    display.fillRoundRect(625, 46, 160, 32, 5, publicBackg);
    display.setTextSize(3);
    display.setTextColor(publicBackg);
    display.setCursor(472, 50);
    display.print("Vandaag");
    display.setTextColor(publicTEXT);
    display.setCursor(651, 50);
    display.print("Morgen");

    if (tomorrowEpex[0] != -100) {  // If tomorrowEpex is available
      for (int i = 0; i < 95; i++) {
        if (drawEpex) {
          display.drawLine(60 + i * 3.75,
                           400 - mapFloat(tomorrowEpex[i] * 100.0f, 0.0f, 50.0f,
                                          0.0f, 360.0f),
                           60 + (i + 1) * 3.75,
                           400 - mapFloat(tomorrowEpex[i] * 100.0f, 0.0f, 50.0f,
                                          0.0f, 360.0f),
                           HABlue);
          display.drawLine(60 + (i + 1) * 3.75,
                           400 - mapFloat(tomorrowEpex[i] * 100.0f, 0.0f, 50.0f,
                                          0.0f, 360.0f),
                           60 + (i + 1) * 3.75,
                           400 - mapFloat(tomorrowEpex[i + 1] * 100.0f, 0.0f,
                                          50.0f, 0.0f, 360.0f),
                           HABlue);
        }
        if (drawEcopower) {
          display.drawLine(60 + i * 3.75,
                           400 - mapFloat(tomorrowEcopower[i] * 100.0f, 0.0f,
                                          50.0f, 0.0f, 360.0f),
                           60 + (i + 1) * 3.75,
                           400 - mapFloat(tomorrowEcopower[i] * 100.0f, 0.0f,
                                          50.0f, 0.0f, 360.0f),
                           ORANGE);
          display.drawLine(60 + (i + 1) * 3.75,
                           400 - mapFloat(tomorrowEcopower[i] * 100.0f, 0.0f,
                                          50.0f, 0.0f, 360.0f),
                           60 + (i + 1) * 3.75,
                           400 - mapFloat(tomorrowEcopower[i + 1] * 100.0f,
                                          0.0f, 50.0f, 0.0f, 360.0f),
                           ORANGE);
        }
      }
      if (drawEpex) {
        display.drawLine(60 + 95 * 3.75,
                         400 - mapFloat(tomorrowEpex[95] * 100.0f, 0.0f, 50.0f,
                                        0.0f, 360.0f),
                         419,
                         400 - mapFloat(tomorrowEpex[95] * 100.0f, 0.0f, 50.0f,
                                        0.0f, 360.0f),
                         HABlue);
      }
      if (drawEcopower) {
        display.drawLine(60 + 95 * 3.75,
                         400 - mapFloat(tomorrowEcopower[95] * 100.0f, 0.0f,
                                        50.0f, 0.0f, 360.0f),
                         419,
                         400 - mapFloat(tomorrowEcopower[95] * 100.0f, 0.0f,
                                        50.0f, 0.0f, 360.0f),
                         ORANGE);
      }
    }
  } else {
    display.fillRoundRect(455, 46, 160, 32, 5, publicBackg);
    display.setTextSize(3);
    display.setTextColor(publicTEXT);
    display.setCursor(472, 50);
    display.print("Vandaag");
    display.setTextColor(publicBackg);
    display.setCursor(651, 50);
    display.print("Morgen");

    for (int i = 0; i < 95; i++) {
      if (drawEpex) {
        display.drawLine(
            60 + i * 3.75,
            400 - mapFloat(epex[i] * 100.0f, 0.0f, 50.0f, 0.0f, 360.0f),
            60 + (i + 1) * 3.75,
            400 - mapFloat(epex[i] * 100.0f, 0.0f, 50.0f, 0.0f, 360.0f),
            HABlue);
        display.drawLine(
            60 + (i + 1) * 3.75,
            400 - mapFloat(epex[i] * 100.0f, 0.0f, 50.0f, 0.0f, 360.0f),
            60 + (i + 1) * 3.75,
            400 - mapFloat(epex[i + 1] * 100.0f, 0.0f, 50.0f, 0.0f, 360.0f),
            HABlue);
      }
      if (drawEcopower) {
        display.drawLine(
            60 + i * 3.75,
            400 - mapFloat(ecopower[i] * 100.0f, 0.0f, 50.0f, 0.0f, 360.0f),
            60 + (i + 1) * 3.75,
            400 - mapFloat(ecopower[i] * 100.0f, 0.0f, 50.0f, 0.0f, 360.0f),
            ORANGE);
        display.drawLine(
            60 + (i + 1) * 3.75,
            400 - mapFloat(ecopower[i] * 100.0f, 0.0f, 50.0f, 0.0f, 360.0f),
            60 + (i + 1) * 3.75,
            400 - mapFloat(ecopower[i + 1] * 100.0f, 0.0f, 50.0f, 0.0f, 360.0f),
            ORANGE);
      }
    }
    if (drawEpex) {
      display.drawLine(
          60 + 95 * 3.75,
          400 - mapFloat(epex[95] * 100.0f, 0.0f, 50.0f, 0.0f, 360.0f), 419,
          400 - mapFloat(epex[95] * 100.0f, 0.0f, 50.0f, 0.0f, 360.0f), HABlue);
    }
    if (drawEcopower) {
      display.drawLine(
          60 + 95 * 3.75,
          400 - mapFloat(ecopower[95] * 100.0f, 0.0f, 50.0f, 0.0f, 360.0f), 419,
          400 - mapFloat(ecopower[95] * 100.0f, 0.0f, 50.0f, 0.0f, 360.0f),
          ORANGE);
    }
  }
  display.fillRect(50, 444, 390, 20, publicBackg);

  if (clickedX != 0) {
    display.fillRect(535, 170, 170, 100, publicBackg);

    display.drawFastVLine(clickedX, 40, 400, publicTEXT);

    int clickedTime = map(clickedX, 60, 420, 0, 96);
    int clickedHour = clickedTime / 4;
    int clickedMinute = (clickedTime % 4) * 15;
    float clickedEpex = 0;
    float clickedEcopower = 0;
    if (tomorrowGraph) {
      clickedEpex = tomorrowEpex[clickedTime] * 100.0f;
      clickedEcopower = tomorrowEcopower[clickedTime] * 100.0f;
    } else {
      clickedEpex = epex[clickedTime] * 100.0f;
      clickedEcopower = ecopower[clickedTime] * 100.0f;
    }
    detailPrices(clickedHour, clickedMinute, clickedEpex, clickedEcopower);
  } else {
    display.fillRect(535, 124, 170, 76, publicBackg);
  }

  pricesV(currentEpex, currentEcopower, clickedX);

  display.drawRoundRect(60, 40, 360, 400, 10, publicTEXT);
}
void Dashboard::pricesV(float currentEpex, float currentEcopower,
                        int clickedX) {
  display.setTextSize(2);
  display.setTextColor(publicTEXT, publicBackg);
  display.setCursor(480, 364);
  display.print("EPEX-prijzen: ");
  display.print(currentEpex);
  display.print(" ct/kWh");
  display.setCursor(480, 414);
  display.print("Ecopower: ");
  display.print(currentEcopower);
  display.print(" ct/kWh");
  const int spacing = 12;
  const int x = 450;
  const int y = 124;
  const int w = 340;
  int h = 196;
  if (clickedX != 0) h = 75;

  char prStr[16];
  char unitStr[16];
  snprintf(prStr, sizeof(prStr), "%.1f", currentEpex);
  snprintf(unitStr, sizeof(unitStr), "ct/kWh");

  if (clickedX == 0) {
    display.setTextSize(6);
  } else {
    display.setTextSize(4);
  }
  display.setTextColor(publicTEXT, publicBackg);

  int16_t tx, ty;
  uint16_t tw, th;
  display.getTextBounds(prStr, 0, 0, &tx, &ty, &tw, &th);
  if (clickedX == 0) {
    display.setTextSize(4);
  } else {
    display.setTextSize(2);
  }
  int16_t ux, uy;
  uint16_t uw, uh;
  display.getTextBounds(unitStr, 0, 0, &ux, &uy, &uw, &uh);

  int16_t totalHeight = th + uh + spacing;
  int16_t startY = y + h / 2 - totalHeight / 2;

  if (clickedX == 0) {
    display.setTextSize(6);
  } else {
    display.setTextSize(4);
  }
  display.setCursor(x + w / 2 - tw / 2, startY);
  display.print(prStr);

  if (clickedX == 0) {
    display.setTextSize(4);
  } else {
    display.setTextSize(2);
  }
  display.setCursor(x + w / 2 - uw / 2, startY + th + spacing);
  display.print(unitStr);

  if (clickedX != 0) {
    display.drawFastHLine(x + w / 4, y + h, w / 2, GREY);
  }
}
void Dashboard::detailPrices(int clickedHour, int clickedMinute, float epex,
                             float ecopower) {
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(2);
  display.setCursor(500, 240);
  if (clickedHour < 10) display.print("0");
  display.print(clickedHour);
  display.print(":");
  if (clickedMinute < 10) display.print("0");
  display.print(clickedMinute);
  display.print(" : ");
  display.print(epex);
  display.print(" ct/kWh ");
  display.setCursor(500, 270);
  display.print("        ");
  display.print(ecopower);
  display.print(" ct/kWh ");
}
#endif