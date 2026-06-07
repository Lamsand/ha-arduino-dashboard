#include "Arduino.h"
#include "Dashboard.h"
#include "Arduino_GigaDisplay_GFX.h"
#include "Colors.h"
#include "SdFat.h"

#define SCREEN_W 800
#define SCREEN_H 480

GigaDisplay_GFX display;

#define chipSelect 5
SdFat sd;
FsFile file;  // = sd.open("cLC.bin");
uint16_t rowBuffer[150000];
#define buffSize 150000

// int Dashboard::TEXT = WHITE;
// int Dashboard::Backg = BLACK;
// int Dashboard::AllBackg = WHITE;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////// BASICS ////////////////////////////////////////////////////////////////////////
bool Dashboard::begin() {
  display.begin();
  display.setRotation(3);
  display.setTextWrap(false);
  if (!sd.begin(SdSpiConfig(chipSelect, DEDICATED_SPI, SD_SCK_MHZ(27)))) {
    return false;
  }
  file = sd.open("blxmRD.bin");
  if (!file) {
    return false;  // SD init ok but file missing
  }
  file.close();
  display.cp437(true);
  return true;
}
int Dashboard::fillArc(int x, int y, int start_angle, int seg_count, int r, int w, unsigned int colour) {
  byte seg = 3;  // Segments are 3 degrees wide = 120 segments for 360 degrees
  byte inc = 3;  // Draw segments every 3 degrees, increase to 6 for segmented ring
  // Calculate first pair of coordinates for segment start
  float sx = cos((start_angle - 90) / 57.2957795);
  float sy = sin((start_angle - 90) / 57.2957795);
  uint16_t x0 = sx * (r - w) + x;
  uint16_t y0 = sy * (r - w) + y;
  uint16_t x1 = sx * r + x;
  uint16_t y1 = sy * r + y;
  // Draw colour blocks every inc degrees
  for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {
    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) / 57.2957795);
    float sy2 = sin((i + seg - 90) / 57.2957795);
    int x2 = sx2 * (r - w) + x;
    int y2 = sy2 * (r - w) + y;
    int x3 = sx2 * r + x;
    int y3 = sy2 * r + y;
    display.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
    display.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
    // Copy segment end to sgement start for next segment
    x0 = x2;
    y0 = y2;
    x1 = x3;
    y1 = y3;
  }
}
void Dashboard::home() {
  display.fillScreen(publicAllBackg);
  display.fillRoundRect(1, 1, SCREEN_W - 2, 43, 10, publicBackg);
  heatPumpIconSmall(610, 22);
  WaterTempIconSmall(470, 22);
  // HALogo(189, 22, 24, HABlue);
  // WiFiIcon(142, 22, 40);
  settingsLogo(768, 22, 24, publicTEXT);
}
void Dashboard::printTime(const String &time) {
  display.setCursor(20, 12);
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(3);
  display.print(time);
}
void Dashboard::printHeatPumpTime(const String &time) {
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
void Dashboard::printTemp(int temp) {
  display.setCursor(142, 12);
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(3);
  display.print(temp);
  display.write(0xF8);
  display.print("C ");
}
void Dashboard::setupWiFi() {
  display.fillScreen(WHITE);
  noConn(400, 200, 300);
  display.setTextSize(3);
  display.setTextColor(BLACK);
  display.setCursor(213, 410);
  display.print("Connecting to WiFi...");
}
void Dashboard::WiFiSetUp() {
  display.fillScreen(WHITE);
  WiFiIcon(400, 200, 400, BLACK);
  display.setTextSize(3);
  display.setTextColor(BLACK);
  display.setCursor(249, 410);
  display.print("Connected to WiFi");
}
void Dashboard::setupHA() {
  display.fillScreen(WHITE);
  HALogo(400, 200, 300, grey, WHITE);
  display.setTextSize(3);
  display.setTextColor(BLACK);
  display.setCursor(123, 410);
  display.print("Connecting to Home Assistant...");
}
void Dashboard::HASetUp() {
  display.fillScreen(WHITE);
  HALogo(400, 200, 300, HABlue, WHITE);
  display.setTextSize(3);
  display.setTextColor(BLACK);
  display.setCursor(159, 410);
  display.print("Connected to Home Assistant");
}
void Dashboard::fillScreen(int color) {
  display.fillScreen(color);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// LOGOS ////////////////////////////////////////////////////////////////////
void Dashboard::settingsLogo(int x, int y, int size, int color) {
  const int nodeSize = size / 10;
  const int barSize = size / 15;
  display.fillRoundRect(x - size / 2, y - size / 2, size, size, 2 * barSize, color);
  display.fillRoundRect(x - 2 * size / 5, y - size / 4 - barSize / 2, 4 * size / 5, barSize, barSize / 2, publicBackg);
  display.fillRoundRect(x - 2 * size / 5, y - barSize / 2, 4 * size / 5, barSize, barSize / 2, publicBackg);
  display.fillRoundRect(x - 2 * size / 5, y + size / 4 - barSize / 2, 4 * size / 5, barSize, barSize / 2, publicBackg);
  // nodes
  display.fillCircle(x, y - size / 4, nodeSize, publicBackg);
  display.fillCircle(x + 3 * size / 16, y, nodeSize, publicBackg);
  display.fillCircle(x - 3 * size / 16, y + size / 4, nodeSize, publicBackg);
}
void Dashboard::bulb(int x, int y, int size, bool state) {
  int color;
  int wire;
  const int wireloop = size / 20;
  if (state) {
    color = YELLOW;
    wire = 0xee4f;
  } else {
    color = grey;
    wire = GREY;
  }
  display.fillRoundRect(x - size / 10, y, size / 5, size / 2, size / 10, GREY);
  display.fillRoundRect(x - size / 8, y, size / 4, size / 3, size / 20, color);
  display.fillCircle(x, y - size / 6, size / 3, color);
  fillArc(x + size / 4, y + 2 * size / 9, -90, 20, size / 4, size / 8, color);
  fillArc(x - size / 4, y + 2 * size / 9, 30, 20, size / 4, size / 8, color);
  display.fillRect(x - 3 * size / 40, y - size / 6, size / 40, 3 * size / 6, wire);
  display.fillRect(x + size / 20, y - size / 6, size / 40, 3 * size / 6, wire);
  fillArc(x - size / 20 - wireloop, y - size / 6, -180, 90, wireloop, size / 40, wire);
  fillArc(x + size / 20 + wireloop, y - size / 6, -90, 90, wireloop, size / 40, wire);
  display.fillRect(x - size / 20 - wireloop, y - size / 6 + size / 40, size / 5, size / 40, wire);
}
void Dashboard::car(int x, int y, int size, int color) {
  // chassis
  display.fillRoundRect(x - size / 2, y, size, size / 4, size / 16, color);
  // roof
  display.fillRoundRect(x - size / 4, y - 3 * size / 16, 5 * size / 8, size / 2, 3 * size / 16, color);
  display.fillRect(x - size / 4, y + size / 4, 5 * size / 8, size / 12, publicBackg);
  // windows
  fillArc(x - size / 4 + 3 * size / 16, y, -90, 30, size / 8, size / 8, publicBackg);
  display.fillRect(x - size / 4 + size / 4, y - size / 8, 3 * size / 16, size / 8 + 1, publicBackg);
  fillArc(x + 3 * size / 16, y, 0, 30, size / 8, size / 8, publicBackg);
  display.fillRect(x + 3 * size / 16, y - size / 8, size / 16, size / 8 + 1, color);
  // wheels
  display.fillCircle(x - size / 4, y + size / 4, size / 12, publicBackg);
  display.fillCircle(x + size / 4, y + size / 4, size / 12, publicBackg);
  display.fillCircle(x - size / 4, y + size / 4, size / 16, color);
  display.fillCircle(x + size / 4, y + size / 4, size / 16, color);
}
void Dashboard::warmtePomp(int x, int y, int size, int color) {
  const int pipeCurve = size / 20;
  // wall
  display.fillRect(x - 2 * size / 9, y - size / 3, 4 * size / 9, size / 3, color);
  // door
  display.fillRect(x - size / 9, y - size / 5, size / 10, size / 5, publicBackg);
  // roof
  display.fillTriangle(x, y - size / 2, x - size / 3, y - size / 3, x + size / 3, y - size / 3, color);
  // pipes
  display.fillRect(x + size / 15, y, size / 30, size / 2 - pipeCurve, RED);
  display.fillRect(x + size / 15 + 2 * pipeCurve - size / 30, y, size / 30, size / 2 - pipeCurve, HABlue);
  fillArc(x + size / 15 + pipeCurve, y + size / 2 - pipeCurve, -180, 30, pipeCurve, size / 30, RED);
  fillArc(x + size / 15 + pipeCurve, y + size / 2 - pipeCurve, 90, 30, pipeCurve, size / 30, HABlue);
}
void Dashboard::HALogo(int x, int y, int size, int color, int backg) {
  size = size / 2;
  x -= size;
  y -= size;
  //draw house
  const int roundRadius = size / 4;
  const int nodeRadius = size / 5;
  const int lineWidth = size / 6;
  const int pythLineWidth = lineWidth * 0.70710678118;
  const int mNx = x + size;
  const int mNy = y + size / 2;
  const int lNx = x + size / 2;
  const int lNy = y + 3 * size / 2;
  const int rNx = x + 3 * size / 2;
  const int rNy = y + size;
  // display.drawRect(x, y, 2*size, 2*size, BLACK);
  y -= 9 * roundRadius / 16;
  display.fillRoundRect(x, y + 1 * size, size * 2, size + 9 * roundRadius / 16, roundRadius, color);
  // draw roof
  display.fillTriangle(x + size, y, x + roundRadius / 4, y + size + roundRadius / 3, x + size * 2 - roundRadius / 4, y + size + roundRadius / 3, color);
  //round roof
  display.fillRect(x + size - roundRadius, y, 2 * roundRadius, 9 * roundRadius / 10, backg);
  display.fillCircle(x + size, y + 100 * roundRadius / 64, roundRadius, color);
  y += 9 * roundRadius / 16;
  // draw nodes
  display.fillCircle(mNx, mNy, nodeRadius, backg);
  display.fillCircle(lNx, lNy, nodeRadius, backg);
  display.fillCircle(rNx, rNy, nodeRadius, backg);
  // connect nodes
  display.fillRect(mNx - lineWidth / 2, mNy, lineWidth, 3 * size / 2, backg);
  int Lx = lNx + pythLineWidth / 2;
  int Ly = lNy - pythLineWidth / 2;
  for (int i = 0; i < pythLineWidth; i++) {
    display.drawLine(Lx, Ly, mNx + (Lx - lNx), y + 2 * size - (Lx - lNx), backg);
    display.drawLine(Lx, Ly + 1, mNx + (Lx - lNx) - 1, y + 2 * size - (Lx - lNx), backg);
    Lx--;
    Ly++;
  }
  Lx = rNx - pythLineWidth / 2;
  Ly = rNy - pythLineWidth / 2;
  for (int i = 0; i < pythLineWidth; i++) {
    display.drawLine(Lx, Ly, mNx + (Lx - rNx), y + 1.5 * size + (Lx - rNx), backg);
    display.drawLine(Lx, Ly + 1, mNx + (Lx - rNx) + 1, y + 1.5 * size + (Lx - rNx), backg);
    Lx++;
    Ly++;
  }
}
void Dashboard::noConn(int x, int y, int size) {
  size = 3 * size / 60;
  /*  x = x + 10*size;
  y = y + 10*size; */
  display.fillCircle(x, y, 10 * size, BLACK);
  display.fillCircle(x, y, 9 * size, WHITE);
  display.fillEllipse(x, y, 5 * size, 10 * size, BLACK);
  display.fillEllipse(x, y, 4 * size, 9 * size, WHITE);
  display.fillRect(x - size / 2, y - 10 * size, size, 20 * size, BLACK);
  display.fillRect(x - 10 * size, y - size / 2, 20 * size, size, BLACK);
  display.fillCircle(x + 8 * size, y + 8 * size, 4 * size, RED);
  int Xx = x + 6 * size + size / 2;
  int Xy = y + 6 * size - size / 2;
  for (int i; i < size; i++) {
    display.drawLine(Xx, Xy, Xx + 4 * size, Xy + 4 * size, WHITE);
    display.drawLine(Xx, Xy + 1, Xx + 4 * size - 1, Xy + 4 * size, WHITE);
    Xx--;
    Xy++;
  }
  Xx = x + 6 * size + size / 2;
  Xy = y + 10 * size + size / 2;
  for (int i; i < size; i++) {
    display.drawLine(Xx, Xy, Xx + 4 * size, Xy - 4 * size, WHITE);
    display.drawLine(Xx, Xy + 1, Xx + 4 * size + 1, Xy - 4 * size, WHITE);
    Xx--;
    Xy--;
  }
}
void Dashboard::WiFiIcon(int x, int y, int size, int color) {
  size = 3 * size / 80;
  y += 10 * size;
  fillArc(x, y, -45, 30, 20 * size, 2 * size, color);
  fillArc(x, y, -45, 30, 14 * size, 2 * size, color);
  fillArc(x, y, -45, 30, 8 * size, 2 * size, color);
  display.fillCircle(x, y, 2 * size, color);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// IMAGES /////////////////////////////////////////////////////////////////
void readLarge(FsFile &f, uint8_t *buf, uint32_t len) {
  uint32_t pos = 0;
  while (pos < len) {
    uint16_t chunk = (len - pos > 32000) ? 32000 : (len - pos);
    f.read(buf + pos, chunk);
    pos += chunk;
  }
}
void imageSimulator(int xInput, int yInput, int w, int h, int color) {
  display.fillRect(xInput - w / 2, yInput - h / 2, w, h, color);
}
// ***************************** lights *****************************
void Dashboard::moonlampImg(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("mlHD.bin");
    } else {
      file = sd.open("mlHL.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("mlLD.bin");
    } else {
      file = sd.open("mlLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 162;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::bolImg(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("bHD.bin");
    } else {
      file = sd.open("bHL.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("bLD.bin");
    } else {
      file = sd.open("bLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 100;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::bergingImg(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("bgHD.bin");
    } else {
      file = sd.open("bgHL.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("bgLD.bin");
    } else {
      file = sd.open("bgLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 156;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::sfeerlichtjesImg(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("sfHD.bin");
    } else {
      file = sd.open("sfHL.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("sfLD.bin");
    } else {
      file = sd.open("sfLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 90;
  const int w = 170;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
// **************************** car ********************************
void Dashboard::carImageMini(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("cDC.bin");
  } else {
    file = sd.open("cLC.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 146;
  const int w = 300;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::carImageLarge(int xInput, int yInput) {
  int h = 267;
  int w = 550;
  if (publicBackg == BLACK) {
    file = sd.open("cDL.bin");
    int h = 267;
    int w = 550;
  } else {
    file = sd.open("cLL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
// ************************** music *********************************
void Dashboard::radio1(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("rd1D.bin");
  } else {
    file = sd.open("rd1L.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 300;
  const int w = 300;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::radio2(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("rd2D.bin");
  } else {
    file = sd.open("rd2L.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 300;
  const int w = 300;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::vuurland(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("vrlD.bin");
  } else {
    file = sd.open("vrlL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 300;
  const int w = 300;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::vrtnws(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("nwsD.bin");
  } else {
    file = sd.open("nwsL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 300;
  const int w = 300;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::spotify(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("sptfD.bin");
  } else {
    file = sd.open("sptfL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 210;
  const int w = 210;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::musicIcon(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("mcD.bin");
  } else {
    file = sd.open("mcL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 170;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::radio1S(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("rd1DS.bin");
  } else {
    file = sd.open("rd1LS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 210;
  const int w = 210;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::radio2S(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("rd2DS.bin");
  } else {
    file = sd.open("rd2LS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 210;
  const int w = 210;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::vuurlandS(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("vrlDS.bin");
  } else {
    file = sd.open("vrlLS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 210;
  const int w = 210;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::vrtnwsS(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("nwsDS.bin");
  } else {
    file = sd.open("nwsLS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 210;
  const int w = 210;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::PommS(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("PommDS.bin");
  } else {
    file = sd.open("PommLS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 210;
  const int w = 210;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::play(int xInput, int yInput) {
  file = sd.open("play.bin");
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 92;
  const int w = 92;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.startWrite();
    for (int i = 0; i < w * q; i++) {
      if (rowBuffer[i] != 0xf81f) {
        int px = (xInput - w / 2) + (i % w);
        int py = (yInput - h / 2) + (q * y) + (i / w);
        display.writePixel(px, py, rowBuffer[i]);
      }
    }
    display.endWrite();
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.startWrite();
  for (int i = 0; i < w * (h % q); i++) {
    if (rowBuffer[i] != 0xf81f) {
      int px = (xInput - w / 2) + (i % w);
      int py = (yInput - h / 2) + (q * y) + (i / w);
      display.writePixel(px, py, rowBuffer[i]);
    }
  }
  display.endWrite();
  file.close();
}
void Dashboard::pause(int xInput, int yInput) {
  file = sd.open("pau.bin");
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 92;
  const int w = 92;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.startWrite();
    for (int i = 0; i < w * q; i++) {
      if (rowBuffer[i] != 0xf81f) {
        int px = (xInput - w / 2) + (i % w);
        int py = (yInput - h / 2) + (q * y) + (i / w);
        display.writePixel(px, py, rowBuffer[i]);
      }
    }
    display.endWrite();
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.startWrite();
  for (int i = 0; i < w * (h % q); i++) {
    if (rowBuffer[i] != 0xf81f) {
      int px = (xInput - w / 2) + (i % w);
      int py = (yInput - h / 2) + (q * y) + (i / w);
      display.writePixel(px, py, rowBuffer[i]);
    }
  }
  display.endWrite();
  file.close();
}
// ************************** energy ***********************************
void Dashboard::bliksem(int xInput, int yInput, int state) {
  switch (state) {
    case 0:
      if (publicBackg == BLACK) {
        file = sd.open("blxmGD.bin");
      } else {
        file = sd.open("blxmGL.bin");
      }
      break;
    case 1:
      if (publicBackg == BLACK) {
        file = sd.open("blxmOD.bin");
      } else {
        file = sd.open("blxmOL.bin");
      }
      break;
    case 2:
      if (publicBackg == BLACK) {
        file = sd.open("blxmRD.bin");
      } else {
        file = sd.open("blxmRL.bin");
      }
      break;
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 132;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::evccIcon(int xInput, int yInput, int state) {
  switch (state) {
    case 0:
      if (publicBackg == BLACK) {
        file = sd.open("sunD.bin");
      } else {
        file = sd.open("sunL.bin");
      }
      break;
    case 1:
      if (publicBackg == BLACK) {
        file = sd.open("pylonD.bin");
      } else {
        file = sd.open("pylonL.bin");
      }
      break;
    case 2:
      if (publicBackg == BLACK) {
        file = sd.open("homeD.bin");
      } else {
        file = sd.open("homeL.bin");
      }
      break;
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 30;
  const int w = 30;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
// ************************** waste ************************************
void Dashboard::wasteIcon(int xInput, int yInput, int wasteType) {
  //   Wastetype:  Rest = 0; GFT = 1;  PMD = 2;  Papier = 3;  Rest + GFT = 4
  if (publicBackg == BLACK) {
    switch (wasteType) {
      case 0:
        file = sd.open("RestD.bin");
        break;
      case 1:
        file = sd.open("GFTD.bin");
        break;
      case 2:
        file = sd.open("PMDD.bin");
        break;
      case 3:
        file = sd.open("PaperD.bin");
        break;
      case 4:
        file = sd.open("GFTRestD.bin");
        break;
    }

  } else {
    switch (wasteType) {
      case 0:
        file = sd.open("RestL.bin");
        break;
      case 1:
        file = sd.open("GFTL.bin");
        break;
      case 2:
        file = sd.open("PMDL.bin");
        break;
      case 3:
        file = sd.open("PaperL.bin");
        break;
      case 4:
        file = sd.open("GFTRestL.bin");
        break;
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int w = 179;
  const int h = 134;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);
    y++;
  }
  readLarge(file, (uint8_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);
  file.close();
}
// ************************** ventilation ******************************
void Dashboard::ventiIcon(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("ventiHD.bin");
    } else {
      file = sd.open("ventiHL.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("ventiLD.bin");
    } else {
      file = sd.open("ventiLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 130;
  const int w = 130;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::ventiIconLarge(int xInput, int yInput, bool state) {
  if (state == HIGH) {
    if (publicBackg == BLACK) {
      file = sd.open("ventiHDL.bin");
    } else {
      file = sd.open("ventiHLL.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("ventiLDL.bin");
    } else {
      file = sd.open("ventiLLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 170;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::clockIcon(int xInput, int yInput, int hours) {
  if (hours == 1) {
    if (publicBackg == BLACK) {
      file = sd.open("1hD.bin");
    } else {
      file = sd.open("1hD.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("3hD.bin");
    } else {
      file = sd.open("3hD.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 170;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::ventiStopIcon(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("stopBoostD.bin");
  } else {
    file = sd.open("stopBoostL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 170;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
// ************************** heat pump ********************************
void Dashboard::heatPumpIcon(int xInput, int yInput, bool state) {
  if (publicBackg == BLACK) {
    if (state) {
      file = sd.open("heatPumpDH.bin");
    } else {
      file = sd.open("heatPumpDL.bin");
    }
  } else {
    if (state) {
      file = sd.open("heatPumpLH.bin");
    } else {
      file = sd.open("heatPumpLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 120;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::heatPumpIconSmall(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("heatPumpDLS.bin");
  } else {
    file = sd.open("heatPumpLLS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 24;
  const int w = 15;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::heatPumpIconLarge(int xInput, int yInput, bool state) {
  if (publicBackg == BLACK) {
    if (state) {
      file = sd.open("heatPumpDHL.bin");
    } else {
      file = sd.open("heatPumpDLL.bin");
    }
  } else {
    if (state) {
      file = sd.open("heatPumpLHL.bin");
    } else {
      file = sd.open("heatPumpLLL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 350;
  const int w = 250;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::WaterTempIcon(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("WaterTempD.bin");
  } else {
    file = sd.open("WaterTempL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 80;
  const int w = 80;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::WaterTempIconSmall(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("WaterTempDS.bin");
  } else {
    file = sd.open("WaterTempLS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 24;
  const int w = 29;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
// ************************** mower ***********************************
void Dashboard::mowerIconLarge(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("JorisD.bin");
  } else {
    file = sd.open("JorisL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 267;
  const int w = 424;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::mowerIcon(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("JorisDS.bin");
  } else {
    file = sd.open("JorisLS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 270;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::mdi_play(int xInput, int yInput, bool inverted) {
  if (inverted) {
    if (publicBackg == BLACK) {
      file = sd.open("playL.bin");
    } else {
      file = sd.open("playD.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("playD.bin");
    } else {
      file = sd.open("playL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 80;
  const int w = 80;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::mdi_pause(int xInput, int yInput, bool inverted) {
  if (inverted) {
    if (publicBackg == BLACK) {
      file = sd.open("pauseL.bin");
    } else {
      file = sd.open("pauseD.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("pauseD.bin");
    } else {
      file = sd.open("pauseL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 80;
  const int w = 80;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::mdi_returnHome(int xInput, int yInput, bool inverted) {
  if (inverted) {
    if (publicBackg == BLACK) {
      file = sd.open("returnHomeL.bin");
    } else {
      file = sd.open("returnHomeD.bin");
    }
  } else {
    if (publicBackg == BLACK) {
      file = sd.open("returnHomeD.bin");
    } else {
      file = sd.open("returnHomeL.bin");
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 80;
  const int w = 80;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t *)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t *)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w, h % q);  // Draw one row at a time
  file.close();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// HOME ///////////////////////////////////////////////////////////////////
void Dashboard::homeEnergy(int x, int y, int w, int h, int state, int value) {
  display.fillRoundRect(x + 1, y + 1, w - 1, h - 1, 10, publicBackg);
  // bliksem(x + w / 4, y + h / 2, state);
  if (value <= -1000) {
    display.setCursor(x + w / 2 - 36, y + h / 2 - 24);
  } else {
    display.setCursor(x + w / 2, y + h / 2 - 24);
  }
  display.setTextColor(publicTEXT);
  display.setTextSize(6);
  display.print(value);
  display.print("W");
}
void Dashboard::homeCar(int x, int y, int w, int h, int battery, float chargingCapacity, int target) {
  display.fillRoundRect(x + 1, y + 1, w - 1, h - 1, 10, publicBackg);
  // int carsize = w - w / 8 - w / 24 - w / 12;
  // if ((h - h / 12 - 3 * w / 24) * 2 < carsize) {
  //   carsize = (h - h / 12 - 3 * w / 24) * 2;
  // }
  // car(x + (w - w / 8 - w / 24) / 2, y + w / 24 + (h - h / 12 - 4 * w / 24) / 2, carsize, publicTEXT);
  // battery percentage
  display.drawRoundRect(x + w / 24 - 1, y + h - h / 12 - w / 24 - 1, 3 * w / 4 + 2, h / 12 + 2, h / 24 + 1, publicBackg);
  display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24, 3 * w / 4, h / 12, h / 24, BLACK);
  if (chargingCapacity > 0.0) {
    for (int i = 0; i < target * 3 * w / 400 - h / 12; i = i + 10) {
      display.drawRoundRect(x + w / 24, y + h - h / 12 - w / 24, target * 3 * w / 400 - i, h / 12, h / 24, HABlue);
    }
    if (battery > 20) {
      display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24, battery * 3 * w / 400, h / 12, h / 24, evccGREEN);
    } else {
      display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24, battery * 3 * w / 400, h / 12, h / 24, RED);
    }
  } else {
    display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24, battery * 3 * w / 400, h / 12, h / 24, grey);
  }
  display.drawRoundRect(x + w / 24, y + h - h / 12 - w / 24, 3 * w / 4, h / 12, h / 24, publicTEXT);
  // charging power
  display.drawRoundRect(x + w - w / 8 - w / 24 - 1, y + w / 24 - 1, w / 8 + 2, h - 2 * w / 24 + 2, h / 24 + 1, publicBackg);
  display.fillRoundRect(x + w - w / 8 - w / 24, y + w / 24, w / 8, h - 2 * w / 24, h / 24, BLACK);
  display.fillRoundRect(x + w - w / 8 - w / 24, y + w / 24 + (11 - chargingCapacity) * (h - 2 * w / 24) / 11 + 1, w / 8, chargingCapacity * (h - 2 * w / 24) / 11, h / 24, HABlue);
  display.drawRoundRect(x + w - w / 8 - w / 24, y + w / 24, w / 8, h - 2 * w / 24, h / 24, publicTEXT);
  // carImageMini(x + (w - w / 8 - w / 24) / 2, y + w / 24 + (h - h / 12 - 4 * w / 24) / 2);
}
void Dashboard::homeEmpty(int x, int y, int w, int h) {
  display.fillRoundRect(x + 1, y + 1, w - 1, h - 1, 10, publicBackg);
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
// **************************** value refresh *************************
void Dashboard::homeEnergyV(int x, int y, int w, int h, int state, int value) {
  display.fillRect(x + w / 2, y + h / 4, w / 2, h / 2, publicBackg);
  display.fillRect(x + w, y + h / 4, 1, h / 2, publicTEXT);
  display.fillRect(x + w + 2, y + h / 4, 30, h / 2, publicBackg);
  display.fillRect(x + w / 2 - 36, y + h / 4, 36, h / 2, publicBackg);
  if (value <= -1000) {
    display.setCursor(x + w / 2 - 36, y + h / 2 - 24);
  } else {
    display.setCursor(x + w / 2, y + h / 2 - 24);
  }
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(6);
  display.print(value);
  display.print("W");
}
void Dashboard::homeCarV(int x, int y, int w, int h, int battery, float chargingCapacity, int target) {
  display.drawRoundRect(x + w / 24 - 1, y + h - h / 12 - w / 24 - 1, 3 * w / 4 + 2, h / 12 + 2, h / 24 + 1, publicBackg);
  display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24, 3 * w / 4, h / 12, h / 24, BLACK);
  if (chargingCapacity > 0.0) {
    for (int i = 0; i < target * 3 * w / 400 - h / 12; i = i + 10) {
      display.drawRoundRect(x + w / 24, y + h - h / 12 - w / 24, target * 3 * w / 400 - i, h / 12, h / 24, HABlue);
    }
    if (battery > 20) {
      display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24, battery * 3 * w / 400, h / 12, h / 24, evccGREEN);
    } else {
      display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24, battery * 3 * w / 400, h / 12, h / 24, RED);
    }
  } else {
    display.fillRoundRect(x + w / 24, y + h - h / 12 - w / 24, battery * 3 * w / 400, h / 12, h / 24, grey);
  }
  display.drawRoundRect(x + w / 24, y + h - h / 12 - w / 24, 3 * w / 4, h / 12, h / 24, publicTEXT);
  // charging power
  display.drawRoundRect(x + w - w / 8 - w / 24 - 1, y + w / 24 - 1, w / 8 + 2, h - 2 * w / 24 + 2, h / 24 + 1, publicBackg);
  display.fillRoundRect(x + w - w / 8 - w / 24, y + w / 24, w / 8, h - 2 * w / 24, h / 24, BLACK);
  if (chargingCapacity > 0.0) {
    display.fillRoundRect(x + w - w / 8 - w / 24, y + w / 24 + (11 - chargingCapacity) * (h - 2 * w / 24) / 11 + 1, w / 8, chargingCapacity * (h - 2 * w / 24) / 11, h / 24, HABlue);
  }
  display.drawRoundRect(x + w - w / 8 - w / 24, y + w / 24, w / 8, h - 2 * w / 24, h / 24, publicTEXT);
}
void Dashboard::homeHeatPumpV(int x, int y, int w, int h, int DHWtemp) {
  display.setCursor(x + w / 2, y + h / 2 - 24);
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(6);
  display.print(DHWtemp);
  display.write(0xF8);
  display.print("C ");
}
// **************************** image draw ***************************
void Dashboard::homeEnergyImg(int x, int y, int w, int h, int state) {
  bliksem(x + w / 4, y + h / 2, state);
}
void Dashboard::homeLightsImg(int x, int y, int w, int h) {
  bolImg(x + w / 2, y + h / 2, HIGH);
}
void Dashboard::homeCarImg(int x, int y, int w, int h) {
  carImageMini(x + (w - w / 8 - w / 24) / 2, y + (h - h / 12 - w / 24) / 2);
}
void Dashboard::homeMusicImg(int x, int y, int w, int h) {
  musicIcon(x + w / 2, y + h / 2);
}
void Dashboard::homeWasteImg(int x, int y, int w, int h, int wasteType) {
  wasteIcon(x + w / 2, y + h / 2, wasteType);
}
void Dashboard::homeVentiImg(int x, int y, int w, int h) {
  ventiIconLarge(x + w / 2, y + h / 2, LOW);
}
void Dashboard::homeHeatPumpImg(int x, int y, int w, int h, bool state) {
  heatPumpIcon(x + w / 4, y + h / 2, state);
}
void Dashboard::homeMowerImg(int x, int y, int w, int h) {
  mowerIcon(x + w / 2, y + h / 2);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// DETAILS //////////////////////////////////////////////////
// ** energy **
int median(int *arr, int i, int range) {
  int buf[range * 2 + 1];
  int count = 0;
  for (int j = i - range; j <= i + range; j++) {
    if (j >= 0 && j < 360) buf[count++] = arr[j];
  }
  // Simpele bubblesort
  for (int a = 0; a < count - 1; a++)
    for (int b = 0; b < count - a - 1; b++)
      if (buf[b] > buf[b + 1]) {
        int t = buf[b];
        buf[b] = buf[b + 1];
        buf[b + 1] = t;
      }
  return buf[count / 2];
}
void Dashboard::energy(int *import, int *prod, int *cons, int *longImport, int *longProd, int *longCons, int currentImport, int currentProduction, int currentConsumption, int *TIME, int *LONGTIME, int state, bool longGraph, bool rounded, bool drawImport, bool drawProduction, bool drawConsumption) {
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  energyV(currentImport, currentProduction, currentConsumption);
  energyGraph(import, prod, cons, longImport, longProd, longCons, TIME, LONGTIME, longGraph, rounded, drawImport, drawProduction, drawConsumption);
  bliksem(450 + 340 / 4, 124 + 156 / 2, state);
}
void Dashboard::energyGraph(int *import, int *prod, int *cons, int *longImport, int *longProd, int *longCons, int *TIME, int *LONGTIME, bool longGraph, bool rounded, bool drawImport, bool drawProduction, bool drawConsumption) {
  if (drawConsumption) {
    display.fillCircle(460, 330, 10, HABlue);
  } else {
    display.fillCircle(460, 330, 10, publicBackg);
    display.drawCircle(460, 330, 10, HABlue);
  }
  if (drawImport) {
    display.fillCircle(460, 375, 10, ORANGE);
  } else {
    display.fillCircle(460, 375, 10, publicBackg);
    display.drawCircle(460, 375, 10, ORANGE);
  }
  if (drawProduction) {
    display.fillCircle(460, 420, 10, evccGREEN);
  } else {
    display.fillCircle(460, 420, 10, publicBackg);
    display.drawCircle(460, 420, 10, evccGREEN);
  }
  display.fillRoundRect(61, 41, 358, 398, 9, publicBackg);
  display.fillRoundRect(450, 40, 340, 44, 10, publicTEXT);
  if (longGraph) {
    display.fillRoundRect(625, 46, 160, 32, 5, publicBackg);
    display.setTextSize(3);
    display.setTextColor(publicBackg);
    display.setCursor(487, 50);
    display.print("30 min");
    display.setTextColor(publicTEXT);
    display.setCursor(687, 50);
    display.print("3u");
    for (int i = -1; i < 11; i++) {
      if (i != 10 && i != -1) { display.drawFastHLine(60, 400 - i * 36, 360, GREY); }
      display.setTextColor(GREY);
      display.setTextSize(1);
      display.setCursor(15, 396 - i * 36);
      display.print(i);
      display.print(" kW");
    }
    for (int i = 0; i < 359; i++) {
      if (drawConsumption) { display.drawLine(60 + i, 400 - map(median(longCons, i, 3), 0, 10000, 0, 360), 61 + i, 400 - map(median(longCons, i + 1, 3), 0, 10000, 0, 360), HABlue); }
      if (drawImport) { display.drawLine(60 + i, 400 - map(median(longImport, i, 3), 0, 10000, 0, 360), 61 + i, 400 - map(median(longImport, i + 1, 3), 0, 10000, 0, 360), ORANGE); }
      if (drawProduction) { display.drawLine(60 + i, 400 - map(median(longProd, i, 3), 0, 10000, 0, 360), 61 + i, 400 - map(median(longProd, i + 1, 3), 0, 10000, 0, 360), evccGREEN); }
    }
    display.fillRect(50, 444, 390, 20, publicBackg);
    for (int i = 0; i < 359; i++) {
      if (LONGTIME[i] > 0) {
        for (int x = 0; x < 20; x++) {
          display.drawFastVLine(60 + i, 50 + x * 20, 5, GREY);
        }
        display.setTextColor(GREY);
        display.setTextSize(1);
        if (LONGTIME[i] < 10) {
          display.setCursor(54 + i, 450);
        } else {
          display.setCursor(51 + i, 450);
        }
        display.print(LONGTIME[i]);
        display.print("u");
      }
    }
  } else {
    display.fillRoundRect(455, 46, 160, 32, 5, publicBackg);
    display.setTextSize(3);
    display.setTextColor(publicTEXT);
    display.setCursor(487, 50);
    display.print("30 min");
    display.setTextColor(publicBackg);
    display.setCursor(687, 50);
    display.print("3u");
    for (int i = -1; i < 11; i++) {
      if (i != 10 && i != -1) { display.drawFastHLine(60, 400 - i * 36, 360, GREY); }
      display.setTextColor(GREY);
      display.setTextSize(1);
      display.setCursor(15, 396 - i * 36);
      display.print(i);
      display.print(" kW");
    }
    if (rounded) {
      for (int i = 0; i < 359; i++) {
        if (drawConsumption) { display.drawLine(60 + i, 400 - map(median(cons, i, 6), 0, 10000, 0, 360), 61 + i, 400 - map(median(cons, i + 1, 6), 0, 10000, 0, 360), HABlue); }
        if (drawImport) { display.drawLine(60 + i, 400 - map(median(import, i, 6), 0, 10000, 0, 360), 61 + i, 400 - map(median(import, i + 1, 6), 0, 10000, 0, 360), ORANGE); }
        if (drawProduction) { display.drawLine(60 + i, 400 - map(median(prod, i, 6), 0, 10000, 0, 360), 61 + i, 400 - map(median(prod, i + 1, 6), 0, 10000, 0, 360), evccGREEN); }
      }
    } else {
      for (int i = 0; i < 359; i++) {
        if (drawConsumption) { display.drawLine(60 + i, 400 - map(cons[i], 0, 10000, 0, 360), 61 + i, 400 - map(cons[i + 1], 0, 10000, 0, 360), HABlue); }
        if (drawImport) { display.drawLine(60 + i, 400 - map(import[i], 0, 10000, 0, 360), 61 + i, 400 - map(import[i + 1], 0, 10000, 0, 360), ORANGE); }
        if (drawProduction) { display.drawLine(60 + i, 400 - map(prod[i], 0, 10000, 0, 360), 61 + i, 400 - map(prod[i + 1], 0, 10000, 0, 360), evccGREEN); }
      }
    }
    display.fillRect(50, 444, 390, 20, publicBackg);
    for (int i = 0; i < 359; i++) {
      if (TIME[i] > 0) {
        for (int x = 0; x < 20; x++) {
          display.drawFastVLine(60 + i, 50 + x * 20, 5, GREY);
        }
        display.setTextColor(GREY);
        display.setTextSize(1);
        if (TIME[i] < 10) {
          display.setCursor(54 + i, 450);
        } else {
          display.setCursor(51 + i, 450);
        }
        display.print(TIME[i]);
        display.print("u");
      }
    }
  }
  display.drawRoundRect(60, 40, 360, 400, 10, publicTEXT);
}
void Dashboard::energyV(int currentImport, int currentProduction, int currentConsumption) {
  display.setTextSize(3);
  display.setTextColor(publicTEXT, publicBackg);
  display.setCursor(480, 320);
  display.print("Verbruik: ");
  display.print(currentConsumption);
  display.print("W");
  display.setCursor(480, 365);
  display.print("Import: ");
  display.print(currentImport);
  display.print("W");
  display.setCursor(480, 410);
  display.print("Opbrengst: ");
  display.print(currentProduction);
  display.print("W");

  // Overzicht
  if (currentImport <= -1000) {
    display.setCursor(450 + 340 / 2 - 36, 124 + 156 / 2 - 24);
  } else {
    display.setCursor(450 + 340 / 2, 124 + 156 / 2 - 24);
  }
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(6);
  display.print(currentImport);
  display.print("W");
  display.fillRect(799, 0, 2, 480, WHITE);
}

void Dashboard::settings(bool darkMode, const String &time) {
  display.fillScreen(publicAllBackg);
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  settingsLogo(56, 56, 56, publicTEXT);
  display.setTextColor(publicTEXT);
  display.setTextSize(8);
  display.setCursor(104, 28);
  display.print("Instelllingen");
  display.setTextSize(3);
  display.setTextColor(publicTEXT);
  display.setCursor(28, 124);
  display.print("Donkere modus:");
  display.drawRoundRect(300, 124, 45, 22, 11, publicTEXT);
  display.fillCircle(311 + 23 * darkMode, 135, 15, publicTEXT);

  display.setCursor(28, 431);
  display.print("Reset:");
  display.fillCircle(150, 442, 15, publicTEXT);
  display.fillCircle(150, 442, 10, publicBackg);
  display.fillRect(150, 422, 20, 20, publicBackg);
  display.fillTriangle(150, 423, 150, 437, 155, 430, publicTEXT);

  display.setCursor(469, 431);
  display.print("Last reset: ");
  display.print(time);
}

// ** car **
void Dashboard::detailCar(int battery, float chargingCapacity, int chargingSpeed, int target, int range) {
  const int spacing = 33;
  const int batteryH = 60;
  const int powerW = 150;
  // const int carsize = SCREEN_W - powerW - 3 * spacing;
  display.fillScreen(publicAllBackg);
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  // car((SCREEN_W - powerW - spacing) / 2, (SCREEN_H - batteryH - 3 * spacing) / 2, carsize);
  // battery percentage
  display.drawRoundRect(spacing - 1, SCREEN_H - batteryH - spacing - 1, SCREEN_W - 3 * spacing - powerW + 2, batteryH + 2, batteryH / 2 + 1, publicBackg);
  display.fillRoundRect(spacing, SCREEN_H - batteryH - spacing, SCREEN_W - 3 * spacing - powerW, batteryH, batteryH / 2, BLACK);
  if (chargingCapacity > 0.0) {
    for (int i = 0; i < target * (SCREEN_W - 3 * spacing - powerW) / 100 - batteryH; i = i + 20) {
      display.drawRoundRect(spacing, SCREEN_H - batteryH - spacing, target * (SCREEN_W - 3 * spacing - powerW) / 100 - i, batteryH, batteryH / 2, HABlue);
    }
    if (battery > 20) {
      display.fillRoundRect(spacing, SCREEN_H - batteryH - spacing, battery * (SCREEN_W - 3 * spacing - powerW) / 100, batteryH, batteryH / 2, evccGREEN);
    } else {
      display.fillRoundRect(spacing, SCREEN_H - batteryH - spacing, battery * (SCREEN_W - 3 * spacing - powerW) / 100, batteryH, batteryH / 2, RED);
    }
  } else {
    display.fillRoundRect(spacing, SCREEN_H - batteryH - spacing, battery * (SCREEN_W - 3 * spacing - powerW) / 100, batteryH, batteryH / 2, grey);
  }
  display.drawRoundRect(spacing, SCREEN_H - batteryH - spacing, SCREEN_W - 3 * spacing - powerW, batteryH, batteryH / 2, publicTEXT);
  // charging power
  display.drawRoundRect(SCREEN_W - powerW - spacing - 1, spacing - 1, powerW + 2, SCREEN_H - 2 * spacing + 2, batteryH / 2 + 1, publicBackg);
  display.fillRoundRect(SCREEN_W - powerW - spacing, spacing, powerW, SCREEN_H - 2 * spacing, batteryH / 2, BLACK);
  display.fillRoundRect(SCREEN_W - powerW - spacing, spacing + (11 - chargingCapacity) * (SCREEN_H - 2 * spacing) / 11 + 1, powerW, chargingCapacity * (SCREEN_H - 2 * spacing) / 11, batteryH / 2, HABlue);
  display.drawRoundRect(SCREEN_W - powerW - spacing, spacing, powerW, SCREEN_H - 2 * spacing, batteryH / 2, publicTEXT);

  // Values
  const String batteryText = String(String(battery) + "% " + String(range) + "km");
  const int batteryTextW = batteryText.length() * 18 - 3;
  display.setTextSize(3);
  display.setTextColor(BLACK);
  if (battery > 30) {
    display.setCursor(spacing + (battery * (SCREEN_W - 3 * spacing - powerW) / 100) / 2 - batteryTextW / 2, SCREEN_H - batteryH / 2 - spacing - 13);
    display.print(batteryText);
  } else {
    display.setTextColor(WHITE);
    display.setCursor(SCREEN_W - 2 * spacing - powerW - ((100 - battery) * (SCREEN_W - 3 * spacing - powerW) / 100) / 2 - batteryTextW / 2, SCREEN_H - batteryH / 2 - spacing - 13);
    display.print(batteryText);
  }

  const String powerText = String(String(chargingCapacity) + "kW");
  const int powerTextW = powerText.length() * 18 - 3;
  display.setTextSize(3);
  if (chargingCapacity > 1.5) {
    display.setTextColor(BLACK);
    display.setCursor(SCREEN_W - spacing - powerW / 2 - powerTextW / 2, SCREEN_H - spacing - (chargingCapacity * (SCREEN_H - 2 * spacing) / 11) / 2 - 12);
    display.print(powerText);
  } else {
    display.setTextColor(WHITE);
    display.setCursor(SCREEN_W - spacing - powerW / 2 - powerTextW / 2, spacing + ((11 - chargingCapacity) * (SCREEN_H - 2 * spacing) / 11) / 2 - 12);
    display.print(powerText);
  }
  carImageLarge(spacing + (SCREEN_W - powerW - 2 * spacing) / 2, spacing + (SCREEN_H - batteryH - 3 * spacing) / 2);
}
void Dashboard::evcc(int PV, int grid, int consumption, float toCar) {
  int toHome = max(consumption - (int)(toCar * 1000.0f), 0);
  int selfUse = min(PV, toHome);
  int gridUse = max(grid, 0);
  int toGrid = max(-grid, 0);

// ── Layout ────────────────────────────────────────────────
  const int16_t regX = 20;
  const int16_t regY = 10;
  const int16_t regW = SCREEN_W - 40;
  const int16_t barX = regX;
  const int16_t barY = regY + 68;
  const int16_t barW = regW;
  const int16_t barH = 44;
  const int16_t barR = 6;
  const int16_t bbotY = barY + barH;
  const int16_t bbotH = 16;

// Icon centres: vertically centred on their bracket line
  const int16_t sunY = barY - bbotH;    // centre of top bracket line
  const int16_t iconY = bbotY + bbotH;  // centre of bottom bracket line

// Bar pixel widths
  int total = selfUse + gridUse + toGrid;
  if (total <= 0) total = 1;
  int greenW = constrain((int)((long)selfUse * barW / total), 0, barW);
  int greyW = constrain((int)((long)gridUse * barW / total), 0, barW - greenW);
  int yellowW = barW - greenW - greyW;
  // snap rounding remainders to 0
  if (toGrid == 0) yellowW = 0, greyW = barW - greenW;
  if (gridUse == 0) greyW = 0, yellowW = barW - greenW;

// ── 0. Background (no fillScreen) ─────────────────────────
  // display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  display.fillRect(regX - 15, regY+35, regW +15, 110, publicBackg);

// ── 1. Title ──────────────────────────────────────────────
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(2);
  display.setCursor(regX, regY + 8);
  display.print("EVCC");

// ── 2. PV value (top centre) ──────────────────────────────
  display.setTextColor(GREY, publicBackg);
  display.setTextSize(2);
  char pvStr[16];
  if (PV < 1000) snprintf(pvStr, sizeof(pvStr), "%d W", PV);
  else snprintf(pvStr, sizeof(pvStr), "%.1f kW", PV / 1000.0f);
  int16_t tx, ty;
  uint16_t tw, th;
  display.getTextBounds(pvStr, 0, 0, &tx, &ty, &tw, &th);
  display.setCursor(regX + regW / 2 - tw / 2, regY + 8);
  display.print(pvStr);

// ── 3. Grid value (top right) ─────────────────────────────
  char gridStr[20];
  if (grid >= 0) {
    if (grid < 1000) snprintf(gridStr, sizeof(gridStr), "+%d W", grid);
    else snprintf(gridStr, sizeof(gridStr), "+%.1f kW", grid / 1000.0f);
    display.setTextColor(red, publicBackg);
  } else {
    if (-grid < 1000) snprintf(gridStr, sizeof(gridStr), "%d W", grid);
    else snprintf(gridStr, sizeof(gridStr), "%.1f kW", grid / 1000.0f);
    display.setTextColor(evccGREEN, publicBackg);
  }
  display.setTextSize(2);
  display.getTextBounds(gridStr, 0, 0, &tx, &ty, &tw, &th);
  display.setCursor(regX + regW - tw, regY + 8);
  display.print(gridStr);

// ── 4. "In" / "Out" labels (vertical, right edge) ─────────
  display.setTextSize(1);
  display.setTextColor(GREY, publicBackg);
  display.setCursor(regX + regW + 6, barY);
  display.print("I");
  display.setCursor(regX + regW + 6, barY + 9);
  display.print("n");
  display.setCursor(regX + regW + 6, bbotY + 2);
  display.print("O");
  display.setCursor(regX + regW + 6, bbotY + 11);
  display.print("u");
  display.setCursor(regX + regW + 6, bbotY + 20);
  display.print("t");

// ── 5. Top bracket (open-bottom) ──────────────────────────
  display.drawRoundRect(barX, barY - bbotH, barW, barR + 40, barR, GREY);
  display.fillRect(barX, barY - 8, barW, 16, publicBackg);

// ── 6. Bottom bracket (open-top) ──────────────────────────
  display.drawRoundRect(barX, bbotY - 4, barW, barR + bbotH, barR, GREY);
  display.fillRect(barX, bbotY - 8, barW, 16, publicBackg);

// ── 7. Three-segment bar ───────────────────────────────────
  if (yellowW > 0)
    display.fillRoundRect(barX, barY, barW, barH, barR, YELLOW);
  else if (greyW > 0)
    display.fillRoundRect(barX, barY, barW, barH, barR, GREY);
  else
    display.fillRoundRect(barX, barY, barW, barH, barR, evccGREEN);

  if (greyW > 0 && yellowW > 0) {
    display.fillRoundRect(barX, barY, greenW + greyW, barH, barR, GREY);
    display.fillRect(barX + greenW + greyW, barY, barR, barH, GREY);
  }

  if (greenW > 0) {
    display.fillRoundRect(barX, barY, greenW, barH, barR, evccGREEN);
    if (greyW > 0 || yellowW > 0)
      display.fillRect(barX + greenW, barY, barR, barH,
                       greyW > 0 ? (uint16_t)GREY : (uint16_t)YELLOW);
  }

// ── 8. Labels inside bars ─────────────────────────────────
  char wStr[16];
  display.setTextSize(2);

  if (greenW > 40) {
    if (selfUse < 1000) snprintf(wStr, sizeof(wStr), "%d W", selfUse);
    else snprintf(wStr, sizeof(wStr), "%.1f kW", selfUse / 1000.0f);
    uint16_t lw, lh;
    int16_t lx, ly;
    display.getTextBounds(wStr, 0, 0, &lx, &ly, &lw, &lh);
    display.setTextColor(BLACK, publicBackg);
    display.setCursor(barX + (greenW - lw) / 2, barY + (barH - lh) / 2);
    display.print(wStr);
  }

  if (greyW > 40) {
    if (gridUse < 1000) snprintf(wStr, sizeof(wStr), "%d W", gridUse);
    else snprintf(wStr, sizeof(wStr), "%.1f kW", gridUse / 1000.0f);
    uint16_t lw, lh;
    int16_t lx, ly;
    display.getTextBounds(wStr, 0, 0, &lx, &ly, &lw, &lh);
    display.setTextColor(publicTEXT);
    display.setCursor(barX + greenW + (greyW - lw) / 2, barY + (barH - lh) / 2);
    display.print(wStr);
  }

  if (yellowW > 40) {
    if (toGrid < 1000) snprintf(wStr, sizeof(wStr), "%d W", toGrid);
    else snprintf(wStr, sizeof(wStr), "%.1f kW", toGrid / 1000.0f);
    uint16_t lw, lh;
    int16_t lx, ly;
    display.getTextBounds(wStr, 0, 0, &lx, &ly, &lw, &lh);
    display.setTextColor(BLACK);
    display.setCursor(barX + greenW + greyW + (yellowW - lw) / 2, barY + (barH - lh) / 2);
    display.print(wStr);
  }

// ── 9. Divider ticks ──────────────────────────────────────
  if (greyW > 1)
    display.drawLine(barX + greenW, bbotY + 4, barX + greenW, bbotY + bbotH - 4, GREY);
  if (yellowW > 1)
    display.drawLine(barX + greenW + greyW, bbotY + 4, barX + greenW + greyW, bbotY + bbotH - 4, GREY);

// ── 10. Sun icon — centred on top bracket line, black box ──
  if (greenW + yellowW > 1){
    const int16_t sx = regX + (greenW + yellowW) / 2;
    const int16_t sy = sunY;
    evccIcon(sx, sy, 0);
  }

// Pylon on top bracket, centred in the grey (import) zone
  if (greyW > 0) {
    const int16_t gx = barX + greenW + greyW / 2;
    const int16_t gy = sunY;
    evccIcon(gx, gy, 1);
  }

// ── 11. Home icon — centred on bottom bracket line, black box
  {
    const int16_t hx = barX + (greenW + greyW) / 2;
    const int16_t hy = iconY;
    const int16_t pad = 18;
    display.fillRect(hx - pad * 5 / 6, hy - pad * 5 / 6, pad * 2 * 5 / 6, pad * 2 * 5 / 6, BLACK);
    // roof
    display.drawLine(hx - 14 * 5 / 6, hy - 2 * 5 / 6, hx, hy - 16 * 5 / 6, grey);
    display.drawLine(hx, hy - 16 * 5 / 6, hx + 14 * 5 / 6, hy - 2 * 5 / 6, grey);
    display.drawLine(hx - 14 * 5 / 6, hy - 2 * 5 / 6, hx + 14 * 5 / 6, hy - 2 * 5 / 6, grey);
    // walls
    display.drawRect(hx - 10 * 5 / 6, hy - 2 * 5 / 6, 21 * 5 / 6, 14 * 5 / 6, grey);
    // door
    display.fillRect(hx - 4 * 5 / 6, hy + 4 * 5 / 6, 8 * 5 / 6, 8 * 5 / 6, grey);
  }

// ── 12. Pylon icon — centred on bottom bracket line, black box
  if (yellowW > 0) {
    const int16_t gx = barX + greenW + greyW + yellowW / 2;
    const int16_t gy = iconY;
    const int16_t pad = 18;
    display.fillRect(gx - pad * 5 / 6, gy - pad * 5 / 6, pad * 2 * 5 / 6, pad * 2 * 5 / 6, BLACK);
    display.drawLine(gx, gy - 18 * 5 / 6, gx, gy + 6 * 5 / 6, grey);
    display.drawLine(gx - 14 * 5 / 6, gy - 14 * 5 / 6, gx + 14 * 5 / 6, gy - 14 * 5 / 6, grey);
    display.drawLine(gx - 14 * 5 / 6, gy - 14 * 5 / 6, gx, gy - 18 * 5 / 6, grey);
    display.drawLine(gx + 14 * 5 / 6, gy - 14 * 5 / 6, gx, gy - 18 * 5 / 6, grey);
    display.drawLine(gx - 10 * 5 / 6, gy - 6 * 5 / 6, gx + 10 * 5 / 6, gy - 6 * 5 / 6, grey);
    display.drawLine(gx - 10 * 5 / 6, gy - 6 * 5 / 6, gx, gy - 14 * 5 / 6, grey);
    display.drawLine(gx + 10 * 5 / 6, gy - 6 * 5 / 6, gx, gy - 14 * 5 / 6, grey);
    display.drawLine(gx, gy + 6 * 5 / 6, gx - 12 * 5 / 6, gy + 6 * 5 / 6, grey);
    display.drawLine(gx, gy + 6 * 5 / 6, gx + 12 * 5 / 6, gy + 6 * 5 / 6, grey);
  }

// ── 13. Car charging section ───────────────────────────────
  if (toCar > 0.0f) {
    const int16_t carY = bbotY + bbotH + 20;
    const int16_t carBarH = 44;
    const int16_t carBarW = barW;
    const float maxCar = 11.0f;
    const int16_t cbbotY = carY + carBarH;
    int carFill = constrain((int)(toCar / maxCar * carBarW), 0, carBarW);

    display.drawRoundRect(barX, carY - bbotH, carBarW, barR + 40, barR, GREY);
    display.fillRect(barX, carY - 8, carBarW, 16, publicBackg);
    display.drawRoundRect(barX, cbbotY - 4, carBarW, barR + bbotH, barR, GREY);
    display.fillRect(barX, cbbotY - 8, carBarW, 16, publicBackg);

    if (carFill > 0)
      display.fillRoundRect(barX, carY, carFill, carBarH, barR, HABlue);
    if (carFill < carBarW)
      display.fillRoundRect(barX + carFill, carY, carBarW - carFill, carBarH, barR, GREY);
    if (carFill > 0 && carFill < carBarW) {
      display.fillRect(barX + carFill, carY, barR, carBarH, GREY);
      display.fillRect(barX + carFill - barR, carY, barR, carBarH, HABlue);
    }

    char carStr[16];
    if (toCar < 1.0f) snprintf(carStr, sizeof(carStr), "%.0f W", toCar * 1000.0f);
    else snprintf(carStr, sizeof(carStr), "%.1f kW", toCar);
    uint16_t lw, lh;
    int16_t lx, ly;
    display.setTextSize(2);
    display.getTextBounds(carStr, 0, 0, &lx, &ly, &lw, &lh);
    if (carFill > (int)lw + 8) {
      display.setTextColor(publicTEXT, publicBackg);
      display.setCursor(barX + (carFill - lw) / 2, carY + (carBarH - lh) / 2);
    } else {
      display.setTextColor(publicTEXT, publicBackg);
      display.setCursor(barX + carFill + 6, carY + (carBarH - lh) / 2);
    }
    display.print(carStr);

    // Car icon with black box, centred on bottom bracket line
    const int16_t carIconY = cbbotY + bbotH / 2;
    const int16_t carIconX = barX + carBarW / 2;
    display.fillRect(carIconX - 18, carIconY - 18, 36, 36, BLACK);
    car(carIconX, carIconY, 60, grey);
  }
// display.setCursor(300, 300);
// display.setTextColor(WHITE);
// display.print(yellowW);
}

// ** lights **
void Dashboard::detailLights(bool sfeerlichtjes, bool groteBol, bool glazenBol, bool berging, bool maanlamp, bool raamversiering) {
  // Sfeerlichtjes
  display.fillRoundRect(1, 1, SCREEN_W / 3 - 1, SCREEN_H / 2 - 1, 10, publicBackg);
  // bulb(SCREEN_W / 6, SCREEN_H / 4 + 16, 170, sfeerlichtjes);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(20, 20);
  display.print("Sfeerlichtjes");
  // sfeerlichtjesImg(SCREEN_W / 6, SCREEN_H / 4 + 16, sfeerlichtjes);
  // Grote bol
  display.fillRoundRect(SCREEN_W / 3 + 1, 1, SCREEN_W / 3 - 1, SCREEN_H / 2 - 1, 10, publicBackg);
  // bulb(3 * SCREEN_W / 6, SCREEN_H / 4 + 16, 170, groteBol);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(21 + SCREEN_W / 3, 20);
  display.print("Grote bol");
  // bolImg(3 * SCREEN_W / 6, SCREEN_H / 4 + 16, groteBol);
  // Glazen bol
  display.fillRoundRect(2 * SCREEN_W / 3, 1, SCREEN_W / 3, SCREEN_H / 2 - 1, 10, publicBackg);
  // bulb(5 * SCREEN_W / 6, SCREEN_H / 4 + 16, 170, glazenBol);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(20 + 2 * SCREEN_W / 3, 20);
  display.print("Glazen bol");
  // bolImg(5 * SCREEN_W / 6, SCREEN_H / 4 + 16, glazenBol);
  // Berging
  display.fillRoundRect(1, SCREEN_H / 2 + 1, SCREEN_W / 2 - 1, SCREEN_H / 2 - 2, 10, publicBackg);
  // bulb(SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, 170, berging);
  display.setTextColor(publicTEXT);
  display.setTextSize(2);
  display.setCursor(20, 260);
  display.print("Maanlamp");
  // moonlampImg(SCREEN_W / 4, 3 * SCREEN_H / 4 + 16, maanlamp);
  // Maanlamp
  display.fillRoundRect(SCREEN_W / 2 + 1, SCREEN_H / 2 + 1, SCREEN_W / 2 - 2, SCREEN_H / 2 - 2, 10, publicBackg);
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
    display.fillRoundRect(305, 40 + 275 - 270 * br / 100, 190, 270 * br / 100, 45, YELLOW);
  } else if (br != 0) {
    display.fillRoundRect(305, 40 + 275 - 270 * 33 / 100, 190, 270 * 33 / 100, 45, YELLOW);
    display.fillRoundRect(305, 45, 190, 270 * (100 - br) / 100, 45, publicBackg);
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
// ** music **
void Dashboard::music(const String &k_kanaal, const String &k_title, bool k_state, const String &s_kanaal, const String &s_title, bool s_state) {
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  String displayName = k_kanaal;
  //******************* Keuken ************************
  if (k_kanaal == "VRT Radio 1" || k_kanaal == "Radio 1") {
    radio1(200, 200);
    displayName = "Radio 1";
  } else if (k_kanaal == "VRT Studio Brussel Vuurland" || k_kanaal == "Studio Brussel Vuurland" || k_kanaal == "Vuurland") {
    vuurland(200, 200);
    displayName = "Vuurland";
  } else if (k_kanaal == "VRT NWS") {
    vrtnws(200, 200);
  } else if (k_kanaal == "Radio 2 vlaams-brabant" || k_kanaal == "Radio 2") {
    radio2(200, 200);
    displayName = "Radio 2";
  } else if (k_kanaal == "Spotify Connect" || k_kanaal == "" || k_kanaal == "Spotify") {
    spotify(200, 200);
    displayName = "Spotify";
  }
  if (!k_state) {
    play(300, 350);
  } else {
    pause(300, 350);
  }
  display.setTextColor(publicTEXT);
  display.setTextSize(3);
  display.setCursor(50, 360);
  display.print(k_kanaal);
  display.setTextSize(2);
  display.setCursor(50, 394);
  display.print(trimTitle(k_title, 20));
  //******************* Speelkamer ************************
  displayName = s_kanaal;
  if (s_kanaal == "Radio 1" || s_kanaal == "Radio 1") {
    radio1(600, 200);
  } else if (s_kanaal == "VRT Studio Brussel Vuurland" || s_kanaal == "Studio Brussel Vuurland" || s_kanaal == "Vuurland") {
    vuurland(600, 200);
    displayName = "Vuurland";
  } else if (s_kanaal == "VRT NWS") {
    vrtnws(600, 200);
  } else if (s_kanaal == "Radio 2 vlaams-brabant" || s_kanaal == "Radio 2") {
    radio2(600, 200);
    displayName = "Radio 2";
  } else if (s_kanaal == "Spotify Connect" || s_kanaal == "" || s_kanaal == "Spotify") {
    spotify(600, 200);
    displayName = "Spotify";
  }
  if (!s_state) {
    play(700, 350);
  } else {
    pause(700, 350);
  }
  display.setTextColor(publicTEXT);
  display.setTextSize(3);
  display.setCursor(450, 360);
  display.print(s_kanaal);
  display.setTextSize(2);
  display.setCursor(450, 394);
  display.print(trimTitle(s_title, 20));
}
void Dashboard::musicV(const String &k_kanaal, const String &k_title, const String &s_kanaal, const String &s_title) {
  //******************* Keuken ************************
  // display.fillRect(50, 355, 380, 50, publicBackg);
  // Clear
  display.setTextColor(publicBackg, publicBackg);
  display.setTextSize(3);
  display.setCursor(50, 360);
  display.print("Vuurland");

  display.setTextColor(publicTEXT, publicBackg);
  display.setCursor(50, 360);
  display.print(k_kanaal);
  display.setTextSize(2);
  // Clear
  display.setTextColor(publicBackg, publicBackg);
  display.setCursor(50, 394);
  display.print("                      ");

  display.setTextColor(publicTEXT, publicBackg);
  display.setCursor(50, 394);
  display.print(trimTitle(k_title, 20));
  //******************* Speelkamer ************************
  // display.fillRect(450, 355, 380, 50, publicBackg);
  // Clear
  display.setTextColor(publicBackg, publicBackg);
  display.setTextSize(3);
  display.setCursor(450, 360);
  display.print("Vuurland");

  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(3);
  display.setCursor(450, 360);
  display.print(s_kanaal);
  display.setTextSize(2);
  // Clear
  display.setTextColor(publicBackg, publicBackg);
  display.setCursor(450, 394);
  display.print("                      ");

  display.setTextColor(publicTEXT, publicBackg);
  display.setCursor(450, 394);
  display.print(trimTitle(s_title, 20));
}
void Dashboard::k_kanaal(const String &k_kanaal, bool k_state) {
  String displayName = k_kanaal;
  if (k_kanaal == "VRT Radio 1" || k_kanaal == "Radio 1") {
    radio1(200, 200);
    displayName = "Radio 1";
  } else if (k_kanaal == "VRT Studio Brussel Vuurland" || k_kanaal == "Studio Brussel Vuurland" || k_kanaal == "Vuurland") {
    vuurland(200, 200);
    displayName = "Vuurland";
  } else if (k_kanaal == "VRT NWS") {
    vrtnws(200, 200);
  } else if (k_kanaal == "Radio 2 vlaams-brabant" || k_kanaal == "Radio 2") {
    radio2(200, 200);
    displayName = "Radio 2";
  } else if (k_kanaal == "Spotify Connect" || k_kanaal == "" || k_kanaal == "Spotify") {
    spotify(200, 200);
    displayName = "Spotify";
  }
  if (!k_state) {
    play(300, 350);
  } else {
    pause(300, 350);
  }
}
void Dashboard::s_kanaal(const String &s_kanaal, bool s_state) {
  String displayName = s_kanaal;
  if (s_kanaal == "Radio 1" || s_kanaal == "Radio 1") {
    radio1(600, 200);
  } else if (s_kanaal == "VRT Studio Brussel Vuurland" || s_kanaal == "Studio Brussel Vuurland" || s_kanaal == "Vuurland") {
    vuurland(600, 200);
    displayName = "Vuurland";
  } else if (s_kanaal == "VRT NWS") {
    vrtnws(600, 200);
  } else if (s_kanaal == "Radio 2 vlaams-brabant" || s_kanaal == "Radio 2") {
    radio2(600, 200);
    displayName = "Radio 2";
  } else if (s_kanaal == "Spotify Connect" || s_kanaal == "" || s_kanaal == "Spotify") {
    spotify(600, 200);
    displayName = "Spotify";
  }
  if (!s_state) {
    play(700, 350);
  } else {
    pause(700, 350);
  }
}
void Dashboard::k_state(bool k_state) {
  if (!k_state) {
    play(300, 350);
  } else {
    pause(300, 350);
  }
}
void Dashboard::s_state(bool s_state) {
  if (!s_state) {
    play(700, 350);
  } else {
    pause(700, 350);
  }
}
String Dashboard::trimTitle(const String &title, int maxChars) {
  if (title.length() > maxChars) {
    return title.substring(0, maxChars - 3) + "...";
  }
  return title;
}
void Dashboard::chooseChannel() {
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  radio1S(150, 125);
  radio2S(400, 125);
  PommS(650, 125);
  vrtnwsS(275, 363);
  vuurlandS(525, 363);
}
// ** ventilation **
void Dashboard::ventilationV(bool k_state, bool b_state, int k_CO2, int b_VOC, int k_hum, int b_hum, int k_qua, int b_qua, int k_time, int b_time) {
  // display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H / 2 - 2, 10, publicBackg);
  // display.fillRoundRect(1, SCREEN_H / 2 + 1, SCREEN_W - 2, SCREEN_H / 2 - 2, 10, publicBackg);
  // Keuken
  // ventiIcon(100, 140, k_state);
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

// ** heat pump **
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

// ** mower **
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
