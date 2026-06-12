#include "Dashboard.h"
#include "Arduino.h"
#include "Colors.h"

#ifdef ENABLE_MOWER
void Dashboard::ventiIcon(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("Ventilation/ventiHD.bin");
    } else {
      file = sd.open("Ventilation/ventiHL.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("Ventilation/ventiLD.bin");
    } else {
      file = sd.open("Ventilation/ventiLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 130;
  const int w = 130;
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
void Dashboard::ventiIconLarge(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("Ventilation/ventiHDL.bin");
    } else {
      file = sd.open("Ventilation/ventiHLL.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("Ventilation/ventiLDL.bin");
    } else {
      file = sd.open("Ventilation/ventiLLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
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
void Dashboard::clockIcon(int xInput, int yInput, int hours) {
  if (hours == 1) {
    if (publicBackg == BLACK) {
      file = sd.open("Ventilation/1hD.bin");
    } else {
      file = sd.open("Ventilation/1hD.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("Ventilation/3hD.bin");
    } else {
      file = sd.open("Ventilation/3hD.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
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
void Dashboard::ventiStopIcon(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("Ventilation/stopBoostD.bin");
  } else {
    file = sd.open("Ventilation/stopBoostL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
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

void Dashboard::homeVentiImg(int x, int y, int w, int h) {
  ventiIconLarge(x + w / 2, y + h / 2, LOW);
}

void Dashboard::ventilationV(bool k_state, bool b_state, int k_CO2, int b_VOC,
                             int k_hum, int b_hum, int k_qua, int b_qua,
                             int k_time, int b_time) {
  // display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H / 2 - 2, 10,
  // publicBackg); display.fillRoundRect(1, SCREEN_H / 2 + 1, SCREEN_W - 2,
  // SCREEN_H / 2 - 2, 10, publicBackg); Keuken ventiIcon(100, 140, k_state);
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(4);
  display.setCursor(35, 22);
  display.print("Keuken");
  // clockIcon(475, 120, 1);
  // clockIcon(680, 120, 3);
  display.fillRect(200, 181, 170, 30, BLACK);
  if (k_time > 0) {
    display.setTextSize(3);
    display.setCursor(200, 181);
    int uren = k_time / 3600;
    int minuten = (k_time % 3600) / 60;
    int seconden = k_time % 60;
    if (uren > 0) {
      display.print(uren);
      display.print("u ");
    }
    if (minuten > 0) {
      display.print(minuten);
      display.print("min ");
    }
    // if (seconden > 0 && uren == 0) {
    //   display.print(seconden);
    //   display.print("s");
    // }
  }
  display.setTextSize(3);
  display.setCursor(200, 80);
  if (k_qua < 15) {
    display.print("Uitstekend");
  } else if (k_qua < 35) {
    display.print("Goed");
  } else if (k_qua < 60) {
    display.print("Matig");
  } else if (k_qua < 80) {
    display.print("Slecht");
  } else {
    display.print("Zeer slecht");
  }
  display.setTextSize(2);
  display.setCursor(200, 120);
  display.print("CO2: ");
  display.print(k_CO2);
  display.print("ppm");
  display.setCursor(200, 144);
  display.print("Vocht: ");
  display.print(k_hum);
  display.print("%");
  // Kelder
  display.setTextSize(4);
  display.setCursor(35, 262);
  display.print("Kelder");
  // ventiIcon(100, 380, b_state);
  // clockIcon(475, 360, 1);
  // clockIcon(680, 360, 3);
  display.fillRect(200, 421, 170, 30, BLACK);
  if (b_time > 0) {
    display.setTextSize(3);
    display.setCursor(200, 421);
    int uren = b_time / 3600;
    int minuten = (b_time % 3600) / 60;
    int seconden = b_time % 60;
    if (uren > 0) {
      display.print(uren);
      display.print("u ");
    }
    if (minuten > 0) {
      display.print(minuten);
      display.print("min ");
    }
    if (seconden > 0 && uren == 0) {
      display.print(seconden);
      display.print("s");
    }
  }
  display.setTextSize(3);
  display.setCursor(200, 320);
  if (b_qua < 15) {
    display.print("Uitstekend");
  } else if (b_qua < 35) {
    display.print("Goed");
  } else if (b_qua < 60) {
    display.print("Matig");
  } else if (b_qua < 80) {
    display.print("Slecht");
  } else {
    display.print("Zeer slecht");
  }
  display.setTextSize(2);
  display.setCursor(200, 360);
  display.print("VOC: ");
  display.print(b_VOC);
  display.print("ppm");
  display.setCursor(200, 384);
  display.print("Vocht: ");
  display.print(k_hum);
  display.print("%");
}
void Dashboard::ventilationImg(bool k_state, bool b_state) {
  ventiIcon(100, 140, k_state);
  if (k_state) {
    imageSimulator(475, 120, 170, 170, BLACK);
    ventiStopIcon(680, 120);
  } else {
    clockIcon(475, 120, 1);
    clockIcon(680, 120, 3);
  }
  ventiIcon(100, 380, b_state);
  if (b_state) {
    imageSimulator(475, 360, 170, 170, BLACK);
    ventiStopIcon(680, 360);
  } else {
    clockIcon(475, 360, 1);
    clockIcon(680, 360, 3);
  }
}
#endif