#define Dasboard_cpp

#include "Dashboard.h"

#include "Arduino.h"
#include "Colors.h"
#include "config.h"

////// BASICS //////
bool Dashboard::begin() {
  display.begin();
  display.setRotation(3);
  display.setTextWrap(false);
  if (!sd.begin(SdSpiConfig(chipSelect, DEDICATED_SPI, SD_SCK_MHZ(27)))) {
    return false;
  }
  file = sd.open("Energy/blxmRD.bin");
  if (!file) {
    return false;  // SD init ok but file missing
  }
  file.close();
  display.cp437(true);
  return true;
}

void Dashboard::reInit(){
  display.begin();
  display.setRotation(3);
  display.setTextWrap(false);
  display.cp437(true);
}

int Dashboard::fillArc(int x, int y, int start_angle, int seg_count, int r,
                       int w, unsigned int colour) {
  byte seg = 3;  // Segments are 3 degrees wide = 120 segments for 360 degrees
  byte inc =
      3;  // Draw segments every 3 degrees, increase to 6 for segmented ring
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
void Dashboard::printTime(const String& time) {
  display.setCursor(20, 12);
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(3);
  display.print(time);
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
void Dashboard::fillScreen(int color) { display.fillScreen(color); }
////// LOGOS //////
void Dashboard::settingsLogo(int x, int y, int size, int color) {
  const int nodeSize = size / 10;
  const int barSize = size / 15;
  display.fillRoundRect(x - size / 2, y - size / 2, size, size, 2 * barSize,
                        color);
  display.fillRoundRect(x - 2 * size / 5, y - size / 4 - barSize / 2,
                        4 * size / 5, barSize, barSize / 2, publicBackg);
  display.fillRoundRect(x - 2 * size / 5, y - barSize / 2, 4 * size / 5,
                        barSize, barSize / 2, publicBackg);
  display.fillRoundRect(x - 2 * size / 5, y + size / 4 - barSize / 2,
                        4 * size / 5, barSize, barSize / 2, publicBackg);
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
  display.fillRect(x - 3 * size / 40, y - size / 6, size / 40, 3 * size / 6,
                   wire);
  display.fillRect(x + size / 20, y - size / 6, size / 40, 3 * size / 6, wire);
  fillArc(x - size / 20 - wireloop, y - size / 6, -180, 90, wireloop, size / 40,
          wire);
  fillArc(x + size / 20 + wireloop, y - size / 6, -90, 90, wireloop, size / 40,
          wire);
  display.fillRect(x - size / 20 - wireloop, y - size / 6 + size / 40, size / 5,
                   size / 40, wire);
}
void Dashboard::car(int x, int y, int size, int color) {
  // chassis
  display.fillRoundRect(x - size / 2, y, size, size / 4, size / 16, color);
  // roof
  display.fillRoundRect(x - size / 4, y - 3 * size / 16, 5 * size / 8, size / 2,
                        3 * size / 16, color);
  display.fillRect(x - size / 4, y + size / 4, 5 * size / 8, size / 12,
                   publicBackg);
  // windows
  fillArc(x - size / 4 + 3 * size / 16, y, -90, 30, size / 8, size / 8,
          publicBackg);
  display.fillRect(x - size / 4 + size / 4, y - size / 8, 3 * size / 16,
                   size / 8 + 1, publicBackg);
  fillArc(x + 3 * size / 16, y, 0, 30, size / 8, size / 8, publicBackg);
  display.fillRect(x + 3 * size / 16, y - size / 8, size / 16, size / 8 + 1,
                   color);
  // wheels
  display.fillCircle(x - size / 4, y + size / 4, size / 12, publicBackg);
  display.fillCircle(x + size / 4, y + size / 4, size / 12, publicBackg);
  display.fillCircle(x - size / 4, y + size / 4, size / 16, color);
  display.fillCircle(x + size / 4, y + size / 4, size / 16, color);
}
void Dashboard::warmtePomp(int x, int y, int size, int color) {
  const int pipeCurve = size / 20;
  // wall
  display.fillRect(x - 2 * size / 9, y - size / 3, 4 * size / 9, size / 3,
                   color);
  // door
  display.fillRect(x - size / 9, y - size / 5, size / 10, size / 5,
                   publicBackg);
  // roof
  display.fillTriangle(x, y - size / 2, x - size / 3, y - size / 3,
                       x + size / 3, y - size / 3, color);
  // pipes
  display.fillRect(x + size / 15, y, size / 30, size / 2 - pipeCurve, RED);
  display.fillRect(x + size / 15 + 2 * pipeCurve - size / 30, y, size / 30,
                   size / 2 - pipeCurve, HABlue);
  fillArc(x + size / 15 + pipeCurve, y + size / 2 - pipeCurve, -180, 30,
          pipeCurve, size / 30, RED);
  fillArc(x + size / 15 + pipeCurve, y + size / 2 - pipeCurve, 90, 30,
          pipeCurve, size / 30, HABlue);
}
void Dashboard::HALogo(int x, int y, int size, int color, int backg) {
  size = size / 2;
  x -= size;
  y -= size;
  // draw house
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
  display.fillRoundRect(x, y + 1 * size, size * 2, size + 9 * roundRadius / 16,
                        roundRadius, color);
  // draw roof
  display.fillTriangle(
      x + size, y, x + roundRadius / 4, y + size + roundRadius / 3,
      x + size * 2 - roundRadius / 4, y + size + roundRadius / 3, color);
  // round roof
  display.fillRect(x + size - roundRadius, y, 2 * roundRadius,
                   9 * roundRadius / 10, backg);
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
    display.drawLine(Lx, Ly, mNx + (Lx - lNx), y + 2 * size - (Lx - lNx),
                     backg);
    display.drawLine(Lx, Ly + 1, mNx + (Lx - lNx) - 1,
                     y + 2 * size - (Lx - lNx), backg);
    Lx--;
    Ly++;
  }
  Lx = rNx - pythLineWidth / 2;
  Ly = rNy - pythLineWidth / 2;
  for (int i = 0; i < pythLineWidth; i++) {
    display.drawLine(Lx, Ly, mNx + (Lx - rNx), y + 1.5 * size + (Lx - rNx),
                     backg);
    display.drawLine(Lx, Ly + 1, mNx + (Lx - rNx) + 1,
                     y + 1.5 * size + (Lx - rNx), backg);
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
////// IMAGES //////
void Dashboard::readLarge(FsFile& f, uint8_t* buf, uint32_t len) {
  uint32_t pos = 0;
  while (pos < len) {
    uint16_t chunk = (len - pos > 32000) ? 32000 : (len - pos);
    f.read(buf + pos, chunk);
    pos += chunk;
  }
}
void Dashboard::imageSimulator(int xInput, int yInput, int w, int h, int color) {
  display.fillRect(xInput - w / 2, yInput - h / 2, w, h, color);
}
// ************************** ventilation ******************************
////// HOME //////
void Dashboard::homeEmpty(int x, int y, int w, int h) {
  display.fillRoundRect(x + 1, y + 1, w - 1, h - 1, 10, publicBackg);
}
// **************************** image draw ***************************
////// DETAILS //////
void Dashboard::settings(bool darkMode, const String& time) {
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