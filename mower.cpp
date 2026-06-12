#include "Dashboard.h"
#include "Arduino.h"
#include "Colors.h"

#ifdef ENABLE_MOWER
void Dashboard::mowerIconLarge(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("Mower/mowerD.bin");
  } else {
    file = sd.open("Mower/mowerL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 267;
  const int w = 424;
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
void Dashboard::mowerIcon(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("Mower/mowerDS.bin");
  } else {
    file = sd.open("Mower/mowerLS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 270;
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
void Dashboard::mdi_play(int xInput, int yInput, bool inverted) {
  if (inverted) {
    if (publicBackg == BLACK) {
      file = sd.open("Mower/Controls/playL.bin");
    } else {
      file = sd.open("Mower/Controls/playD.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("Mower/Controls/playD.bin");
    } else {
      file = sd.open("Mower/Controls/playL.bin");
    }
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
void Dashboard::mdi_pause(int xInput, int yInput, bool inverted) {
  if (inverted) {
    if (publicBackg == BLACK) {
      file = sd.open("Mower/Controls/pauseL.bin");
    } else {
      file = sd.open("Mower/Controls/pauseD.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("Mower/Controls/pauseD.bin");
    } else {
      file = sd.open("Mower/Controls/pauseL.bin");
    }
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
void Dashboard::mdi_returnHome(int xInput, int yInput, bool inverted) {
  if (inverted) {
    if (publicBackg == BLACK) {
      file = sd.open("Mower/Controls/returnHomeL.bin");
    } else {
      file = sd.open("Mower/Controls/returnHomeD.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("Mower/Controls/returnHomeD.bin");
    } else {
      file = sd.open("Mower/Controls/returnHomeL.bin");
    }
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

void Dashboard::homeMowerImg(int x, int y, int w, int h) {
  mowerIcon(x + w / 2, y + h / 2);
}

void Dashboard::mower(int battery, bool mowing) {
  display.fillScreen(publicAllBackg);
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  display.setTextColor(publicTEXT);
  display.setTextSize(10);
  display.setCursor(500, 120);
  display.print(battery);
  display.print("%");
  display.setCursor(500, 240);
  display.setTextSize(5);
  if (mowing) {
    display.print("Maaien...");
  } else {
    display.print("Gedockt");
  }
  if (mowing) {
    display.fillRoundRect(40, 360, 200, 80, 20, publicTEXT);
    display.fillRoundRect(264, 360, 200, 80, 20, publicTEXT);
    mdi_pause(140, 400, HIGH);
    mdi_returnHome(364, 400, HIGH);
  } else {
    display.fillRoundRect(40, 360, 424, 80, 20, publicTEXT);
    mdi_play(252, 400, HIGH);
  }
  mowerIconLarge(250, 200);
}
#endif