#include "Dashboard.h"
#include "Arduino.h"
#include "Colors.h"

#ifdef ENABLE_evcc
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

  // Icon centres: vertically centlightRed on their bracket line
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
  display.fillRect(regX - 15, regY + 35, regW + 15, 110, publicBackg);

  // ── 1. Title ──────────────────────────────────────────────
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(2);
  display.setCursor(regX, regY + 8);
  display.print("EVCC");

  // ── 2. PV value (top centre) ──────────────────────────────
  display.setTextColor(GREY, publicBackg);
  display.setTextSize(2);
  char pvStr[16];
  if (PV < 1000)
    snprintf(pvStr, sizeof(pvStr), "%d W", PV);
  else
    snprintf(pvStr, sizeof(pvStr), "%.1f kW", PV / 1000.0f);
  int16_t tx, ty;
  uint16_t tw, th;
  display.getTextBounds(pvStr, 0, 0, &tx, &ty, &tw, &th);
  display.setCursor(regX + regW / 2 - tw / 2, regY + 8);
  display.print(pvStr);

  // ── 3. Grid value (top right) ─────────────────────────────
  char gridStr[20];
  if (grid >= 0) {
    if (grid < 1000)
      snprintf(gridStr, sizeof(gridStr), "+%d W", grid);
    else
      snprintf(gridStr, sizeof(gridStr), "+%.1f kW", grid / 1000.0f);
    display.setTextColor(lightRed, publicBackg);
  } else {
    if (-grid < 1000)
      snprintf(gridStr, sizeof(gridStr), "%d W", grid);
    else
      snprintf(gridStr, sizeof(gridStr), "%.1f kW", grid / 1000.0f);
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
    if (selfUse < 1000)
      snprintf(wStr, sizeof(wStr), "%d W", selfUse);
    else
      snprintf(wStr, sizeof(wStr), "%.1f kW", selfUse / 1000.0f);
    uint16_t lw, lh;
    int16_t lx, ly;
    display.getTextBounds(wStr, 0, 0, &lx, &ly, &lw, &lh);
    display.setTextColor(BLACK, publicBackg);
    display.setCursor(barX + (greenW - lw) / 2, barY + (barH - lh) / 2);
    display.print(wStr);
  }

  if (greyW > 40) {
    if (gridUse < 1000)
      snprintf(wStr, sizeof(wStr), "%d W", gridUse);
    else
      snprintf(wStr, sizeof(wStr), "%.1f kW", gridUse / 1000.0f);
    uint16_t lw, lh;
    int16_t lx, ly;
    display.getTextBounds(wStr, 0, 0, &lx, &ly, &lw, &lh);
    display.setTextColor(publicTEXT);
    display.setCursor(barX + greenW + (greyW - lw) / 2, barY + (barH - lh) / 2);
    display.print(wStr);
  }

  if (yellowW > 40) {
    if (toGrid < 1000)
      snprintf(wStr, sizeof(wStr), "%d W", toGrid);
    else
      snprintf(wStr, sizeof(wStr), "%.1f kW", toGrid / 1000.0f);
    uint16_t lw, lh;
    int16_t lx, ly;
    display.getTextBounds(wStr, 0, 0, &lx, &ly, &lw, &lh);
    display.setTextColor(BLACK);
    display.setCursor(barX + greenW + greyW + (yellowW - lw) / 2,
                      barY + (barH - lh) / 2);
    display.print(wStr);
  }

  // ── 9. Divider ticks ──────────────────────────────────────
  if (greyW > 1)
    display.drawLine(barX + greenW, sunY + 4, barX + greenW, sunY + 12, GREY);
  if (yellowW > 1)
    display.drawLine(barX + greenW + greyW, bbotY + 4, barX + greenW + greyW,
                     bbotY + bbotH - 4, GREY);

  // ── 10. Sun icon — centlightRed on top bracket line, black box ──
  if (greenW + yellowW > 1) {
    const int16_t sx = regX + (greenW + yellowW) / 2;
    const int16_t sy = sunY;
    evccIcon(sx, sy, 0);
  }

  // Pylon on top bracket, centlightRed in the grey (import) zone
  if (greyW > 0) {
    const int16_t gx = barX + greenW + greyW / 2;
    const int16_t gy = sunY;
    evccIcon(gx, gy, 1);
  }

  // // ── 11. Home icon — centlightRed on bottom bracket line, black box
  // {
  //   const int16_t hx = barX + (greenW + greyW) / 2;
  //   const int16_t hy = iconY;
  //   evccIcon(hx, hy, 2);
  // }

  // ── 12. Pylon icon — centlightRed on bottom bracket line, black box
  if (yellowW > 0) {
    const int16_t gx = barX + greenW + greyW + yellowW / 2;
    const int16_t gy = iconY;
    evccIcon(gx, gy, 1);
  }

  // ── 13. Car zone on bottom bracket ────────────────────────
  // The bottom bracket has up to 3 zones: home | car | grid(export)
  // Car zone width derived from toCar relative to total consumption
  int carW = 0;
  if (toCar > 0.0f) {
    int carWatts = (int)(toCar * 1000.0f);
    int botTotal = toHome + (yellowW > 0 ? toGrid : 0);
    if (botTotal <= 0) botTotal = 1;
    carW = constrain((int)((long)carWatts * barW / botTotal), 0, barW - 10);
  }

  // home zone = greenW + greyW - carW, car zone = carW, grid zone = yellowW
  // home icon: centlightRed in home zone
  {
    const int16_t hx = barX + (greenW + greyW - carW) / 2;
    const int16_t hy = iconY;
    evccIcon(hx, hy, 2);
  }

  // car icon: centlightRed in car zone (right of home, left of grid)
  if (carW > 0) {
    // divider tick between home and car
    display.drawLine(barX + greenW + greyW - carW, bbotY + 4,
                     barX + greenW + greyW - carW, bbotY + bbotH - 4, GREY);
    const int16_t cx = barX + greenW + greyW - carW + carW / 2;
    evccIcon(cx, iconY, 3);
  }
  // display.setCursor(300, 300);
  // display.setTextColor(WHITE);
  // display.print(yellowW);
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
    case 3:
      if (publicBackg == BLACK) {
        file = sd.open("carD.bin");
      } else {
        file = sd.open("carL.bin");
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
#endif